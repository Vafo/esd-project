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
  adc_init();
  port_init();
  lcd_init();
  
  _delay_ms(500);
	sei();								//never forget	

  pos_t cur_pos = {
    .x = LCD_WIDTH / 2,
    .y = LCD_HEIGHT / 2  
  };

  vector_t velocity = {
    .x = -4,
    .y = -6
  };

  borders_init();
  while (1)
  {
    lcd_clear();
    ScreenBuffer_clear();


    /*Joystick input*/
    // float x_fl = ( (float) adc_read(JOYSTICK_X)) / 1000;
    // x_fl = 1 - x_fl - 0.4;
    // float y_fl = ( (float) adc_read(JOYSTICK_Y)) / 1000 - 0.5 + 0.1;
    
    // fmt_size = sprintf(fmt_buf, "vel X: %f\tY: %f\t", x_fl, y_fl);
    // uart1_send_arr(fmt_buf, fmt_size);
    // velocity.x = x_fl;
    // velocity.y = y_fl;

    
    // x = x_fl * LCD_WIDTH;
    // y = y_fl * LCD_HEIGHT;

    // print_log("X: %d\tY: %d\r\n", cur_pos.x, cur_pos.y);

    borders_check_hits(&cur_pos, &velocity);
    add_pos_comp(&cur_pos, &velocity, 1);

    borders_render();
    draw_circle(cur_pos.x, cur_pos.y);
    
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
