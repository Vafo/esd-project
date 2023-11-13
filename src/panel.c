#include "panel.h"
#include "config.h"
#include "etc.h"

void panel_init(panel_t* panel, pos_t* pos) {
    panel_move(panel, pos);
    edges_init(panel->edges, ARR_SIZE(panel->edges));
}

void panel_move(panel_t* panel, pos_t* pos) {
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
    edges_check_hits(ball_pos, ball_vel, 3/*radius*/, panel->edges, ARR_SIZE(panel->edges));
    return 0;
}

void panel_render(panel_t* panel) {
    edges_render(panel->edges, ARR_SIZE(panel->edges));
}