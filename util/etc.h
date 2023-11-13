#ifndef ETC_H
#define ETC_H

#include <stdio.h>
#include "uart1.h"

#define print_log(...) do { \
  uint8_t fmt_buf[128]; \
  size_t size; \
  \
  size = sprintf(fmt_buf, __VA_ARGS__); \
  uart1_send_arr(fmt_buf, size); \ 
} while(0)

#define GREATER(x, y) ((x) > (y) ? (x): (y) )
#define SMALLER(x, y) ((x) < (y) ? (x): (y) )
#define ARR_SIZE(arr) ((sizeof(arr)) / (sizeof(arr[0])))
#define ABS(x) ((x) > 0 ? (x) : -(x))

#endif