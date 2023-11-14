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
void add_pos_comp(pos_t* pos, vector_t* dir, float scale) {
  int x = pos->x + dir->x * scale;
  int y = pos->y + dir->y * scale;
  
  x = GREATER(x, 0);
  x = SMALLER(x, LCD_WIDTH);
  
  y = GREATER(y, 0);
  y = SMALLER(y, LCD_HEIGHT);
  
  pos->x = x;
  pos->y = y;
}

void draw_circle(int x, int y) {
  GLCD_Circle(y, x, 2);
  GLCD_Circle(y, x, 1);
}

void port_init() {
  PORTA = 0x00;   DDRA = 0xFF;                    // PA: D
  PORTB = 0xFF;   DDRB = 0xFF;                    // PB: LED
  PORTD = 0x00;   DDRD = 0x00;                    // PD: button
  PORTE = 0x00;   DDRE = 0xFF;                    // PE4: R/S, PE5: E, PE6: CS2, PE7: CS1
  PORTG = 0x00;   DDRG = 0xFF;                    // PG4: Buzzer
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
  pos_t ball_pos = {
    .x = LCD_WIDTH / 2,
    .y = LCD_HEIGHT / 2  
  };

  vector_t ball_vel = {
    .x = -4,
    .y = -6
  };

  // Pan parameters
  panel_t panel0;
  pos_t pan0_pos = {
    .x = 10,
    .y = LCD_HEIGHT / 2
  };
  vector_t pan0_vel = {
    .x = 0,
    .y = 0
  };

  panel_t panel1;
  pos_t pan1_pos = {
    .x = LCD_WIDTH - 10,
    .y = LCD_HEIGHT / 2
  };
  vector_t pan1_vel = {
    .x = 0,
    .y = 0
  };


  borders_init();
  panel_init(&panel0, &pan0_pos, 0);
  panel_init(&panel1, &pan1_pos, 1);

  uint8_t keybrd_char;

  while (1)
  {
    lcd_clear();
    ScreenBuffer_clear();


    /*Joystick input*/
    float x_fl = ( (float) adc_read(JOYSTICK_X)) / 1000;
    x_fl = 1 - x_fl - 0.4;
    float y_fl = ( (float) adc_read(JOYSTICK_Y)) / 1000 - 0.5;
    if(ABS(x_fl) < 0.1)
      x_fl = 0;
    if(ABS(y_fl) < 0.1)
      y_fl = 0;

    // print_log("raw X: %f\tY: %f\r\n", x_fl, y_fl);
    pan0_vel.x = x_fl;
    pan0_vel.y = y_fl;
    vector_norm(&pan0_vel);
    // print_log("vel X: %f\tY: %f\r\n", pan0_vel.x, pan0_vel.y);
    add_pos_comp(&pan0_pos, &pan0_vel, PANEL_SPEED);
    panel_move(&panel0, &pan0_pos);
    // x = x_fl * LCD_WIDTH;
    // y = y_fl * LCD_HEIGHT;

    uart1_recv_byte(&keybrd_char);
    // pan1_vel.x = 0;
    // pan1_vel.y = 0;
    if(keybrd_char == 'w')
      pan1_vel.y = -0.5;
    if(keybrd_char == 's')
      pan1_vel.y = 0.5;
    if(keybrd_char == 'a')
      pan1_vel.x = -0.5;
    if(keybrd_char == 'd')
      pan1_vel.x = 0.5;
    if(keybrd_char == ' ') {
      pan1_vel.x = 0;
      pan1_vel.y = 0;
    }

    vector_norm(&pan0_vel);
    add_pos_comp(&pan1_pos, &pan1_vel, PANEL_SPEED);
    panel_move(&panel1, &pan1_pos);

    // print_log("X: %d\tY: %d\r\n", ball_pos.x, ball_pos.y);

    borders_check_hits(&ball_pos, &ball_vel);
    panel_check_hit(&panel0, &ball_pos, &ball_vel);
    panel_check_hit(&panel1, &ball_pos, &ball_vel);
    vector_norm(&ball_vel);
    add_pos_comp(&ball_pos, &ball_vel, BALL_SPEED);

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
