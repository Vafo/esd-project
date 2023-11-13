#ifndef UART1_H
#define UART1_H

#include <config.h>
#include <stdint.h>
#include <stddef.h>


void uart1_init();

uint8_t uart1_send_byte(uint8_t byte);
uint8_t uart1_recv_byte(uint8_t* res);

uint8_t uart1_send_arr(uint8_t* src_arr, size_t size);
uint8_t uart1_recv_arr(uint8_t* dst_arr, size_t capacity);

#endif