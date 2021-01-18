/**
 * @file circular_buff.h
 * @author Konrad Sikora
 * @date Jan 2021
 */

#ifndef circular_buff_h
#define circular_buff_h

#include "MKL05Z4.h"

#define CB_MAX_LEN 50

typedef struct circular_buff
{
    uint8_t *head;
    uint8_t *tail;
    uint8_t *data;
    uint8_t *buffor;
    uint16_t new_len;
} circular_buff;

typedef enum
{
    buffor_full,
    buffor_not_full,
    buffor_empty,
    buffor_not_empty
} CB_state;

uint8_t *CB_init(circular_buff *cbuff);
void CB_free(circular_buff *cbuff);
CB_state CB_buff_full(circular_buff *cbuff);
CB_state CB_buff_empty(circular_buff *cbuff);
uint8_t CB_read_data(circular_buff *cbuff);
uint8_t CB_add_data(circular_buff *cbuff, uint8_t data);

#endif