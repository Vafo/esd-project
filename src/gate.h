#ifndef GATE_H
#define GATE_H

#include "edges.h"
#include "vector.h"

typedef struct {
    uint8_t num_hits;
    edge_t edges[3];
} gate_t;

void gate_init(gate_t* gate);

uint8_t gate_check_hits(gate_t* gate, vector_t* ball_pos);

#endif