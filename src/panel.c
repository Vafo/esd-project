#include "panel.h"
#include "config.h"
#include "etc.h"

#include "_glcd.h"
#include "_buzzer.h"

#include <avr/interrupt.h>

void panel_init(panel_t* panel, vector_t* pos, uint8_t player_id) {
    panel_move(panel, pos);
    edges_init(panel->edges, ARR_SIZE(panel->edges));

    cli();
    panel->flag = player_id;
    sei();
}

void panel_move(panel_t* panel, vector_t* pos) {
    pos->x = GREATER(pos->x, PANEL_LENGTH/2);
    pos->x = SMALLER(pos->x, LCD_WIDTH - PANEL_LENGTH/2);

    pos->y = GREATER(pos->y, PANEL_LENGTH/2);
    pos->y = SMALLER(pos->y, LCD_HEIGHT - PANEL_LENGTH/2);

    panel->pos.x = pos->x;
    panel->pos.y = pos->y;

    /*top*/
    panel->edges[0].start.x = pos->x - (PANEL_LENGTH)/2;
    panel->edges[0].start.y = pos->y - (PANEL_LENGTH)/2;

    panel->edges[0].end.x = pos->x + (PANEL_LENGTH)/2;
    panel->edges[0].end.y = pos->y - (PANEL_LENGTH)/2;

    /*right*/
    panel->edges[1].start.x = pos->x + (PANEL_LENGTH)/2;
    panel->edges[1].start.y = pos->y - (PANEL_LENGTH)/2;

    panel->edges[1].end.x = pos->x + (PANEL_LENGTH)/2;
    panel->edges[1].end.y = pos->y + (PANEL_LENGTH)/2;

    /*bottom*/
    panel->edges[2].start.x = pos->x + (PANEL_LENGTH)/2;
    panel->edges[2].start.y = pos->y + (PANEL_LENGTH)/2;

    panel->edges[2].end.x = pos->x - (PANEL_LENGTH)/2;
    panel->edges[2].end.y = pos->y + (PANEL_LENGTH)/2;

    /*left*/
    panel->edges[3].start.x = pos->x - (PANEL_LENGTH)/2;
    panel->edges[3].start.y = pos->y + (PANEL_LENGTH)/2;

    panel->edges[3].end.x = pos->x - (PANEL_LENGTH)/2;
    panel->edges[3].end.y = pos->y - (PANEL_LENGTH)/2;
}

uint8_t panel_check_hit(
    panel_t* panel, vector_t* pan_vel,
    vector_t* ball_pos, vector_t* ball_vel
) {
    uint8_t is_hit;

    // If panel already hit ball, ignore
    if(panel->no_hit_cnt > 0) {
        --panel->no_hit_cnt;
        return 0;
    }

    vector_t vel_tmp = {
        .x = ball_vel->x,
        .y = ball_vel->y
    };

    is_hit = edges_check_hits(
        ball_pos, &vel_tmp,
        PANEL_EDGE_RADIUS/*radius*/, panel->edges, ARR_SIZE(panel->edges)
    );

    if(is_hit) {
        panel->no_hit_cnt = PANEL_NO_HIT_CNT;

        if( !((pan_vel->x == 0) && (pan_vel->y == 0)) ) { // None of vel components is zero
            float sum = (BALL_WEIGHT+PANEL_WEIGHT);
            vel_tmp.x = 
                (BALL_WEIGHT - PANEL_WEIGHT)/sum * ball_vel->x +
                (2 * PANEL_WEIGHT)/sum * pan_vel->x;

            vel_tmp.y = 
                (BALL_WEIGHT - PANEL_WEIGHT)/sum * ball_vel->y +
                (2 * PANEL_WEIGHT)/sum * pan_vel->y;
        }

        ball_vel->x = vel_tmp.x;
        ball_vel->y = vel_tmp.y;

        if(panel->flag == PLAYER_ID0) {
            Sound(BB,HSi);
        } else {
            Sound(SB,HLa);
        }

    }

    return is_hit;
}

void panel_render(panel_t* panel) {
    edges_render(panel->edges, ARR_SIZE(panel->edges));

    if(panel->flag == PLAYER_ID0) {
        // draw circle for player 1
        GLCD_Circle(panel->pos.y, panel->pos.x, 2);
    } else {
        // draw cross for player 0
        GLCD_Line(
            panel->pos.y - PANEL_LENGTH/2, panel->pos.x - PANEL_LENGTH/2,
            panel->pos.y + PANEL_LENGTH/2, panel->pos.x + PANEL_LENGTH/2
        );

        GLCD_Line(
            panel->pos.y + PANEL_LENGTH/2, panel->pos.x - PANEL_LENGTH/2,
            panel->pos.y - PANEL_LENGTH/2, panel->pos.x + PANEL_LENGTH/2
        );
    }
}