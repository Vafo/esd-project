#include "panel.h"
#include "config.h"
#include "etc.h"
#include "_glcd.h"

void panel_init(panel_t* panel, pos_t* pos, uint8_t player_id) {
    panel_move(panel, pos);
    edges_init(panel->edges, ARR_SIZE(panel->edges));

    panel->flag |= (player_id & 1) << PLAYER_ID0;
}

void panel_move(panel_t* panel, pos_t* pos) {
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

uint8_t panel_check_hit(panel_t* panel, pos_t* ball_pos, pos_t* ball_vel) {
    uint8_t is_hit;

    // If panel already hit ball, ignore
    if(panel->flag & (1<<IS_HIT)) {
        panel->flag &= ~(1<<IS_HIT);
        return 0;
    }
    
    is_hit = edges_check_hits(
        ball_pos, ball_vel,
        5/*radius*/, panel->edges, ARR_SIZE(panel->edges)
    );

    if(is_hit)
        panel->flag |= (1<<IS_HIT);

    return is_hit;
}

void panel_render(panel_t* panel) {
    edges_render(panel->edges, ARR_SIZE(panel->edges));

    if(panel->flag & (1<<PLAYER_ID0)) {
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