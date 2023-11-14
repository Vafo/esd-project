#ifndef GAME_H
#define GAME_H

#include "panel.h"

typedef struct {
    // Ball parameters
    vector_t ball_pos;
    vector_t ball_vel;

    // Pan parameters
    // Panel 0
    panel_t panel0;
    vector_t pan0_pos;
    vector_t pan0_vel;
    
    // Panel 1
    panel_t panel1;
    vector_t pan1_pos;
    vector_t pan1_vel;
} game_instance_t;

void game_init(game_instance_t* game);
void game_start_state(game_instance_t* game);

void game_loop(game_instance_t* game);

#endif