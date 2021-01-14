#include "MKL05Z4.h"

#ifndef dma_h
#define dma_h

#define CB_MAX_LEN 30

struct circular_buf
{
    uint8_t *head;
    uint8_t *tail;
    uint8_t *data;
    uint8_t *buffor;
    uint16_t new_len;
};

typedef enum
{
    buffor_full,
    buffor_not_full,
    buffor_empty,
    buffor_not_empty
} CB_state;

uint8_t CB_init(circular_buf *cbuff);
void CB_free(circular_buf *cbuff);
CB_state CB_buff_full(circular_buf *cbuff);
CB_state CB_buff_empty(circular_buf *cbuff);
uint8_t CB_read_data(circular_buf *cbuff);
uint8_t CB_add_data(circular_buf *cbuff, uint8_t data);

#endif