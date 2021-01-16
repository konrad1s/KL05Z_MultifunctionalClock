#ifndef UART_H
#define UART_H

#include "MKL05Z4.h"
#include "../inc/circular_buff.h"

#define BAUDRATE 9600
#define DEFAULT_SYSTEM_CLOCK 41943040u

void UART0_init(void);
void uart_send(uint8_t *text);

extern circular_buff tx_buffor, rx_buffor;

#endif