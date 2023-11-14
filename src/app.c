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

#include "game.h"

#include "_adc.h"
#include "_glcd.h"


void port_init() {
  PORTA = 0x00;   DDRA = 0xFF;                    // PA: D
  PORTB = 0xFF;   DDRB = 0xFF;                    // PB: LED
  PORTD = 0x00;   DDRD = 0x00;                    // PD: button
  PORTE = 0x00;   DDRE = 0xFF;                    // PE4: R/S, PE5: E, PE6: CS2, PE7: CS1
  PORTG = 0x00;   DDRG = 0xFF;                    // PG4: Buzzer
}

game_instance_t game;

int main() {
  // USB I/O
  uart1_init();
  // ADC for Joystick
  adc_init();
  // Init ports for LCD
  port_init();
  // Init LCD
  lcd_init();
  

  game_init(&game);
	sei();								//never forget	

  _delay_ms(500);
  while (1)
  {
    game_loop(&game);
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
