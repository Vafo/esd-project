#ifndef PANEL_H
#define PANEL_H

#include "edges.h"

typedef enum {
    IS_HIT,
    PLAYER_ID0
} panel_flag_t;

typedef struct {
    pos_t pos;
    uint8_t flag;
    edge_t edges[4];
} panel_t;

// played_id = 0 | 1
void panel_init(panel_t* panel, pos_t* pos, uint8_t player_id);
void panel_move(panel_t* panel, pos_t* pos);
uint8_t panel_check_hit(panel_t* panel, pos_t* ball_pos, pos_t* ball_vel);
void panel_render(panel_t* panel);

#endif