#ifndef UART0_H
#define UART0_H

#include <config.h>
#include <stdint.h>
#include <stddef.h>


void uart0_init();

uint8_t uart0_send_byte(uint8_t byte);
uint8_t uart0_recv_byte(uint8_t* res);

uint8_t uart0_send_arr(uint8_t* src_arr, size_t size);
uint8_t uart0_recv_arr(uint8_t* dst_arr, size_t capacity);

#endif