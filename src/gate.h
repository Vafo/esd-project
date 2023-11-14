#ifndef GATE_H
#define GATE_H

#include "edges.h"
#include "vector.h"

typedef void (*on_gate_hit)(void* ctx);

typedef struct {
    uint8_t num_hits;
    uint8_t player_id;
    edge_t edges[3];

    on_gate_hit cb;
    void* ctx;
} gate_t;


void gate_init(
    on_gate_hit cb1, void* ctx1,
    on_gate_hit cb2, void* ctx2
);
uint8_t gate_check_hits(vector_t* ball_pos);
void gate_render();

#endif