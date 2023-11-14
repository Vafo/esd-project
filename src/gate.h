#ifndef GATE_H
#define GATE_H

#include "edges.h"
#include "vector.h"

struct gate_s;

typedef void (*on_gate_hit)(struct gate_s* gate);

typedef struct gate_s {
    uint8_t num_hits;
    uint8_t player_id;
    edge_t edges[3];

    on_gate_hit cb;
} gate_t;


void gate_init(on_gate_hit cb1, on_gate_hit cb2);
uint8_t gate_check_hits(vector_t* ball_pos);
void gate_render();

#endif