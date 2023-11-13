#ifndef PANEL_H
#define PANEL_H

#include "edges.h"

typedef struct {
    pos_t pos;

    edge_t edges[4];
} panel_t;

void panel_init(panel_t* panel, pos_t* pos);
void panel_move(panel_t* panel, pos_t* pos);
uint8_t panel_check_hit(panel_t* panel, pos_t* ball_pos, pos_t* ball_vel);
void panel_render(panel_t* panel);

#endif