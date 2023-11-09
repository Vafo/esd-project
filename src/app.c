// #define __AVR_ATmega128__
#include "config.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "uart0.h"
#include "uart1.h"

#define USB_BAUD 9600
#define BL_BAUD 38400

int main() {
	unsigned char data;
	// UBRR1H = (F_CPU/16/USB_BAUD-1)>>8;		// UBRR
	// UBRR1L = F_CPU/16/USB_BAUD-1;		
	// UCSR1B = (1<<RXEN1)|(1<<TXEN1);		// receiver and transmitter
	// UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);	// 8-bit

	// UART0
    // uart0_init(BL_BAUD);
		
	DDRB = 0xFF;
	PORTB = 0x0F;
    sei();								//never forget
	
    // UART1: 9600 8N1
    uart1_init(USB_BAUD);
	
    uint8_t jalap_buf[128];
    size_t size = 0;
    // UDR1 = 'A';
    while (1){
        uart1_send_byte('A');

        // size = uart1_recv_arr(jalap_buf, 128);
        uart1_send_arr("Received", 8);
        // uart0_send_arr(jalap_buf, size);
        // size = uart0_recv_arr(jalap_buf, 128);
        // uart1_send_arr(jalap_buf, size);

        _delay_ms(500);
        PORTB ^= ( 1 << PORTB4 );
	}

}
