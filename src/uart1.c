#include "uart1.h"
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
    // UCSR1B |= (1<<UDRIE1); // enable interrupt
    UDR1 = buf_pop(tx_buf, &tx_beg);
    m_state |= (1 << tx_in_progress);
}

// TX: Single byte 
SIGNAL(USART1_TX_vect) {
    PORTB ^= (1 << PORTB5);
    // UDR1 = 'A';
    if(tx_beg != tx_end) {
        send_n_incr();
    } else {
        // UCSR1B &= ~(1<<UDRIE1); // disable intr
        m_state &= ~(1 << tx_in_progress); // turn off tx_in_progress
    }
}

SIGNAL(USART1_RX_vect) {
    PORTB ^= (1 << PORTB6);
    // while( 
    //         ((UCSR1A) & (1 << RXC1)) // while there is data
    //         && (circular_increment(rx_end, 1) != rx_beg)
    // ) {
        buf_append(UDR1, rx_buf, &rx_end);
    // }
}

void uart1_init(unsigned long baud_rate) {
	// UART1
	// Baudrate
    UBRR1H = ((F_CPU/16)/baud_rate - 1)>>8;		// UBRR
	UBRR1L = (F_CPU/16)/baud_rate-1;
    // Control Registers
	UCSR1B = 
        (1<<RXCIE1) | (1<<TXCIE1) /*| (1 <<UDRIE1)*/   // RX and TX INT enable
        |(1<<RXEN1) | (1<<TXEN1);   // receiver and transmitter
	UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);	// 8-bit
}

uint8_t uart1_send_byte(uint8_t byte) {
    if( circular_increment(tx_end, 1) == tx_beg ) {
        // No space left to put bytes
        return -1;
    }

    buf_append(byte, tx_buf, &tx_end);

    if( !(m_state & (1 << tx_in_progress)) ) // No transmission is in progress
        send_n_incr();
        // UCSR1B |= (1<<UDRIE1); // enable int

    return 0;
}

uint8_t uart1_recv_byte(uint8_t* res) {
    if( rx_beg == rx_end ) {
        // There are no bytes to read
        return -1;
    }

    *res = buf_pop(rx_buf, &rx_beg);
    return 0;
}

uint8_t uart1_send_arr(uint8_t* src_arr, size_t size) {
    int i;
    for(i = 0; i < size; ++i)
        if( uart1_send_byte(*src_arr++) != 0 )
            break;
    
    return i;
}

uint8_t uart1_recv_arr(uint8_t* dst_arr, size_t size) {
    int i;
    uint8_t tmp;

    for(i = 0; i < size; ++i) {
        if(uart1_recv_byte(&tmp) != 0)
            break;
        *dst_arr++ = tmp;
    }
    
    return i;
}