#include "gate.h"

#include "etc.h"

void gate_init(gate_t* gate) {
    edges_init(gate->edges, ARR_SIZE(gate->edges));
    gate->num_hits = 0;
}

uint8_t gate_check_hits(gate_t* gate, vector_t* ball_pos) {
    
    

}