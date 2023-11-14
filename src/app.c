// #define __AVR_ATmega128__
#include "config.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "uart0.h"
#include "uart1.h"

#include "vector.h"
#include "edges.h"
#include "panel.h"
#include "etc.h"

#include "_adc.h"
#include "_glcd.h"


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

void port_init() {
  PORTA = 0x00;   DDRA = 0xFF;                    // PA: D
  PORTB = 0xFF;   DDRB = 0xFF;                    // PB: LED
  PORTD = 0x00;   DDRD = 0x00;                    // PD: button
  PORTE = 0x00;   DDRE = 0xFF;                    // PE4: R/S, PE5: E, PE6: CS2, PE7: CS1
  PORTG = 0x00;   DDRG = 0xFF;                    // PG4: Buzzer
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

void input_keyboard(
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

int main() {
  // USB I/O
  uart1_init();
  // ADC for Joystick
  adc_init();
  // Init ports for LCD
  port_init();
  // Init LCD
  lcd_init();
  
  _delay_ms(500);
	sei();								//never forget	

  // Ball parameters
  vector_t ball_pos = {
    .x = LCD_WIDTH / 2,
    .y = LCD_HEIGHT / 2  
  };

  vector_t ball_vel = {
    .x = -4,
    .y = -6
  };

  // Pan parameters
  panel_t panel0;
  vector_t pan0_pos = {
    .x = 10,
    .y = LCD_HEIGHT / 2
  };
  vector_t pan0_vel = {
    .x = 0,
    .y = 0
  };

  panel_t panel1;
  vector_t pan1_pos = {
    .x = LCD_WIDTH - 10,
    .y = LCD_HEIGHT / 2
  };
  vector_t pan1_vel = {
    .x = 0,
    .y = 0
  };


  borders_init();
  panel_init(&panel0, &pan0_pos, PLAYER_ID0);
  _delay_ms(100);
  panel_init(&panel1, &pan1_pos, PLAYER_ID1);

  while (1)
  {
    lcd_clear();
    ScreenBuffer_clear();

    input_joystick(
      JOYSTICK_X, JOYSTICK_Y,
      &panel0, &pan0_pos, &pan0_vel
    );

    input_keyboard(&panel1, &pan1_pos, &pan1_vel);

    // /*Drag of keyboard*/
    // vector_add(&pan1_vel, &pan1_vel, -0.8);

    // print_log("X: %d\tY: %d\r\n", ball_pos.x, ball_pos.y);

    /*Check if ball hits something*/
    borders_check_hits(&ball_pos, &ball_vel);
    panel_check_hit(&panel0, &pan0_vel, &ball_pos, &ball_vel);
    panel_check_hit(&panel1, &pan1_vel, &ball_pos, &ball_vel);
    // vector_norm(&ball_vel);
    add_pos_comp(&ball_pos, &ball_vel, 1);

    /*Drag of board*/
    vector_add(&ball_vel, &ball_vel, -DRAG_COEFF);

    /*Render objects*/
    borders_render();
    panel_render(&panel0);
    panel_render(&panel1);
    draw_circle(ball_pos.x, ball_pos.y);
    
    _delay_ms(100);
  }
  
  
  /*ESP*/
  // uart0_init(ESP_BAUD);
  // uart1_init(USB_BAUD);
  // uint8_t buf[128];
  // size_t size;
  
  // size_t i;
  // while(1) {
  //   size = uart1_recv_arr(buf, 128);
  //   uart0_send_arr(buf, size);
  //   size = uart0_recv_arr(buf, 128);
  //   uart1_send_arr(buf, size);
  // }
  
  return 0;
}
