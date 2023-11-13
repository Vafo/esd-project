#include "uart0.h"
#include "config.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define BUF_SIZE 256 // At least 2 
#if ((BUF_SIZE - 1) & BUF_SIZE) != 0
    #error "BUF_SIZE is not of power of 2"
#endif

static volatile uint8_t tx_buf[BUF_SIZE];
static volatile uint8_t rx_buf[BUF_SIZE];
static volatile int16_t
    tx_beg = 0,
    tx_end = 0;
static volatile int16_t 
    rx_beg = 0, /*first valid data*/
    rx_end = 0; /*one past last valid data*/

static volatile uint8_t m_state = 0;

typedef enum {
    tx_in_progress
} state_bit_t;

inline int16_t circular_increment(int16_t cur_pos, int16_t inc) {
    return (cur_pos + inc) & (BUF_SIZE - 1) /*mask*/;
}

inline void buf_append(uint8_t item, uint8_t* buf, int16_t* pos) {
    buf[*pos] = item;
    *pos = circular_increment(*pos, 1);
}

inline uint8_t buf_pop(uint8_t* buf, int16_t* pos) {
    uint8_t tmp = buf[*pos];
    *pos = circular_increment(*pos, 1);
    return tmp;
}

inline void send_n_incr() {
    // UCSR0B |= (1<<UDRIE0); // enable interrupt
    UDR0 = buf_pop(tx_buf, &tx_beg);
    m_state |= (1 << tx_in_progress);
}

// TX: Single byte 
SIGNAL(USART0_TX_vect) {
    PORTB ^= (1 << PORTB7);
    // UDR1 = 'A';
    if(tx_beg != tx_end) {
        send_n_incr();
    } else {
        // UCSR1B &= ~(1<<UDRIE1); // disable intr
        m_state &= ~(1 << tx_in_progress); // turn off tx_in_progress
    }
}

SIGNAL(USART0_RX_vect) {
    // PORTB ^= (1 << PORTB6);
    // while( 
    //         ((UCSR1A) & (1 << RXC1)) // while there is data
    //         && (circular_increment(rx_end, 1) != rx_beg)
    // ) {
        buf_append(UDR0, rx_buf, &rx_end);
    // }
}

void uart0_init() {
	// UART0
	// Baudrate 115200
    UBRR0H = ESP_UBRR >> 8;
    UBRR0L = ESP_UBRR;
    // Control Registers
	UCSR0B = 
        (1<<RXCIE0) | (1<<TXCIE0) /*| (1 <<UDRIE0)*/   // RX and TX INT enable
        |(1<<RXEN0) | (1<<TXEN0);   // receiver and transmitter
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);	// 8-bit
}

uint8_t uart0_send_byte(uint8_t byte) {
    if( circular_increment(tx_end, 1) == tx_beg ) {
        // No space left to put bytes
        return -1;
    }

    buf_append(byte, tx_buf, &tx_end);

    if( !(m_state & (1 << tx_in_progress)) ) // No transmission is in progress
        send_n_incr();
        // UCSR0B |= (1<<UDRIE0); // enable int

    return 0;
}

uint8_t uart0_recv_byte(uint8_t* res) {
    if( rx_beg == rx_end ) {
        // There are no bytes to read
        return -1;
    }

    *res = buf_pop(rx_buf, &rx_beg);
    return 0;
}

uint8_t uart0_send_arr(uint8_t* src_arr, size_t size) {
    int i;
    for(i = 0; i < size; ++i)
        if( uart0_send_byte(*src_arr++) != 0 )
            break;
    
    return i;
}

uint8_t uart0_recv_arr(uint8_t* dst_arr, size_t size) {
    int i;
    uint8_t tmp;

    for(i = 0; i < size; ++i) {
        if(uart0_recv_byte(&tmp) != 0)
            break;
        *dst_arr++ = tmp;
    }
    
    return i;
}