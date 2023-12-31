#ifndef PANEL_H
#define PANEL_H

#include "edges.h"

typedef enum {
    PLAYER_ID0,
    PLAYER_ID1
} panel_flag_t;

typedef struct {
    edge_t edges[4];
    uint8_t flag;
    pos_t pos;
    
    uint8_t no_hit_cnt;
} panel_t;

// played_id = 0 | 1
void panel_init(panel_t* panel, vector_t* pos, uint8_t player_id);
void panel_move(panel_t* panel, vector_t* pos);
uint8_t panel_check_hit(
    panel_t* panel, vector_t* pan_vel,
    vector_t* ball_pos, vector_t* ball_vel
);
void panel_render(panel_t* panel);

#endif