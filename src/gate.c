#include "gate.h"

#include "panel.h"

#include "_buzzer.h"
#include "_glcd.h"
#include "etc.h"

static gate_t m_gates[] = {
    /*Gate for player 0*/
    {
        .edges = {
          {
            .start = {
                .x = 0,
                .y = LCD_HEIGHT/2 - GATE_HEIGHT/2
            },
            .end = {
                .x = GATE_WIDTH,
                .y = LCD_HEIGHT/2 - GATE_HEIGHT/2
            }
          },
          {
            .start = {
                .x = GATE_WIDTH,
                .y = LCD_HEIGHT/2 - GATE_HEIGHT/2
            },
            .end = {
                .x = GATE_WIDTH,
                .y = LCD_HEIGHT/2 + GATE_HEIGHT/2
            }
          },
          {
            .start = {
                .x = GATE_WIDTH,
                .y = LCD_HEIGHT/2 + GATE_HEIGHT/2
            },
            .end = {
                .x = 0,
                .y = LCD_HEIGHT/2 + GATE_HEIGHT/2
            }
          }
        },
        .player_id = PLAYER_ID0,
    },
    /*Gate for player 1*/
    {
        .edges = {
          {
            .start = {
                .x = LCD_WIDTH-1,
                .y = LCD_HEIGHT/2 - GATE_HEIGHT/2
            },
            .end = {
                .x = LCD_WIDTH-1 - GATE_WIDTH,
                .y = LCD_HEIGHT/2 - GATE_HEIGHT/2
            }
          },
          {
            .start = {
                .x = LCD_WIDTH-1 - GATE_WIDTH,
                .y = LCD_HEIGHT/2 - GATE_HEIGHT/2
            },
            .end = {
                .x = LCD_WIDTH-1 - GATE_WIDTH,
                .y = LCD_HEIGHT/2 + GATE_HEIGHT/2
            }
          },
          {
            .start = {
                .x = LCD_WIDTH-1 - GATE_WIDTH,
                .y = LCD_HEIGHT/2 + GATE_HEIGHT/2
            },
            .end = {
                .x = LCD_WIDTH-1,
                .y = LCD_HEIGHT/2 + GATE_HEIGHT/2
            }
          }
        },
        .player_id = PLAYER_ID1,
    },
    
};


static void s_gate_init(gate_t* gate, on_gate_hit callback) {
    edges_init(gate->edges, ARR_SIZE(gate->edges));
    
    gate->num_hits = 0;
    gate->cb = callback;
}

static uint8_t s_gate_check_hits(gate_t* gate, vector_t* ball_pos) {
    int8_t is_hit;
    is_hit = edges_is_hit(ball_pos, GATE_EDGE_RADIUS, gate->edges, ARR_SIZE(gate->edges));
    // print_log("ISHIT %d GATE %d\r\n", is_hit, gate->player_id);
    if(is_hit != -1) {
        ++gate->num_hits;
        // print_log("GATE %d HIT X: %f Y: %f\r\n", gate->player_id, ball_pos->x, ball_pos->y);
        if(gate->player_id == PLAYER_ID0) {
            S_S1();
        } else {
            S_S2();
        }
        
        if(gate->cb != NULL)
            gate->cb(gate);
        return 1;
    }

    return 0;
}

static void s_gate_render(gate_t* gate) {
    edges_render(gate->edges, ARR_SIZE(gate->edges));
}

void gate_init(on_gate_hit cb1, on_gate_hit cb2) {
    s_gate_init(&m_gates[0], cb1);
    // s_gate_init(&m_gates[1], cb2);
}

uint8_t gate_check_hits(vector_t* ball_pos) {
    uint8_t res = 0;
    size_t i;
    for(i = 0; i < ARR_SIZE(m_gates); ++i) {
        res = res || s_gate_check_hits(&m_gates[i], ball_pos);
    }

    return res;
}

void gate_render() {
    size_t i;
    for(i = 0; i < ARR_SIZE(m_gates); ++i) {
        s_gate_render(&m_gates[i]);
    }

    lcd_xy(4, 0);
    GLCD_1DigitDecimal(m_gates[1].num_hits, 1);

    lcd_xy(4, 19);
    GLCD_1DigitDecimal(m_gates[0].num_hits, 1);

}