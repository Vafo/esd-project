#include "config.h"
#include "game.h"

#include <assert.h>
#include <avr/io.h>

#include "gate.h"
#include "edges.h"

#include "_glcd.h"
#include "uart1.h"
#include "etc.h"

// Add to cur dot pos, so that it does not exceed edges 
void add_pos_comp(vector_t* pos, vector_t* dir, float scale) {
  float x = pos->x + dir->x * scale;
  float y = pos->y + dir->y * scale;
  
  x = GREATER(x, BALL_RADIUS);
  x = SMALLER(x, LCD_WIDTH - BALL_RADIUS);
  
  y = GREATER(y, BALL_RADIUS);
  y = SMALLER(y, LCD_HEIGHT - BALL_RADIUS);
  
  pos->x = x;
  pos->y = y;
}

void draw_circle(int x, int y) {
  GLCD_Circle(y, x, BALL_RADIUS);
  GLCD_Circle(y, x, BALL_RADIUS-1);
}

void input_joystick(
  uint8_t joy_x,
  uint8_t joy_y,
  panel_t* panel,
  vector_t* panel_pos,
  vector_t* panel_vel
) {
  float x_fl = ( (float) adc_read(joy_x)) / 1000;
  x_fl = 1 - x_fl - 0.4;
  float y_fl = ( (float) adc_read(joy_y)) / 1000 - 0.5;
  if(ABS(x_fl) < 0.1)
    x_fl = 0;
  if(ABS(y_fl) < 0.1)
    y_fl = 0;

  // print_log("raw X: %f\tY: %f\r\n", x_fl, y_fl);
  panel_vel->x = x_fl;
  panel_vel->y = y_fl;
  vector_norm(panel_vel);
  vector_scale(panel_vel, PANEL_SPEED);
  // print_log("vel X: %f\tY: %f\r\n", pan0_vel.x, pan0_vel.y);
  add_pos_comp(panel_pos, panel_vel, 1);
  panel_move(panel, panel_pos);
  // x = x_fl * LCD_WIDTH;
  // y = y_fl * LCD_HEIGHT;
}

static void input_keyboard(
  panel_t* panel,
  vector_t* panel_pos,
  vector_t* panel_vel
) {
  uint8_t keybrd_char;

  uart1_recv_byte(&keybrd_char);
  // pan1_vel.x = 0;
  // pan1_vel.y = 0;
  vector_norm(panel_vel);
  if(keybrd_char == 'w')
    panel_vel->y = -0.5;
  if(keybrd_char == 's')
    panel_vel->y = 0.5;
  if(keybrd_char == 'a')
    panel_vel->x = -0.5;
  if(keybrd_char == 'd')
    panel_vel->x = 0.5;
  if(keybrd_char == ' ') {
    panel_vel->x = 0;
    panel_vel->y = 0;
  }

  vector_norm(panel_vel);
  vector_scale(panel_vel, PANEL_SPEED);
  add_pos_comp(panel_pos, panel_vel, 1);
  panel_move(panel, panel_pos);
}

void reinit_game_cb(void* ctx) {
    assert(ctx != NULL);
    
    game_instance_t* game = (game_instance_t*) ctx; 
    game_start_state(game);
}

void game_start_state(game_instance_t* game) {
    assert(game != NULL);
    // Ball parameters
    // Ball pos
    game->ball_pos.x = LCD_WIDTH / 2;
    game->ball_pos.y = LCD_HEIGHT / 2;

    // Ball velocity
    game->ball_vel.x = 0;
    game->ball_vel.y = 0;

    // Pan parameters
    // Panel 0
    game->pan0_pos.x = 10;
    game->pan0_pos.y = LCD_HEIGHT / 2;
    
    game->pan0_vel.x = 0;
    game->pan0_vel.y = 0;

    // Panel 1
    game->pan1_pos.x = LCD_WIDTH - 10;
    game->pan1_pos.y = LCD_HEIGHT / 2;
    
    game->pan1_vel.x = 0;
    game->pan1_vel.y = 0;

    panel_move(&game->panel0, &game->pan0_pos);
    panel_move(&game->panel1, &game->pan1_pos);
}

void game_init(game_instance_t* game) {
    assert(game != NULL);

    game_start_state(game);
    
    borders_init();
    panel_init(&game->panel0, &game->pan0_pos, PLAYER_ID0);
    panel_init(&game->panel1, &game->pan1_pos, PLAYER_ID1);
    gate_init(reinit_game_cb, game, reinit_game_cb, game);
}

void game_loop_logic(game_instance_t* game) {
    // Panel 0 input
    input_joystick(
      JOYSTICK_X, JOYSTICK_Y,
      &game->panel0, &game->pan0_pos, &game->pan0_vel
    );

    // Panel 1 input
    input_keyboard(&game->panel1, &game->pan1_pos, &game->pan1_vel);

    // /*Drag of keyboard*/
    // vector_add(&pan1_vel, &pan1_vel, -0.8);

    // print_log("X: %d\tY: %d\r\n", ball_pos.x, ball_pos.y);

    /*Check if ball hits something*/
    borders_check_hits(&game->ball_pos, &game->ball_vel);
    panel_check_hit(&game->panel0, &game->pan0_vel, &game->ball_pos, &game->ball_vel);
    panel_check_hit(&game->panel1, &game->pan1_vel, &game->ball_pos, &game->ball_vel);
    // vector_norm(&ball_vel);
    add_pos_comp(&game->ball_pos, &game->ball_vel, 1);

    /*Check if ball hits*/
    gate_check_hits(&game->ball_pos);

    /*Drag of board*/
    vector_add(&game->ball_vel, &game->ball_vel, -DRAG_COEFF);
}

void game_loop_render(game_instance_t* game) {
    lcd_clear();
    ScreenBuffer_clear();

    borders_render();
    
    panel_render(&game->panel0);
    panel_render(&game->panel1);
    
    gate_render();

    draw_circle(game->ball_pos.x, game->ball_pos.y);
}