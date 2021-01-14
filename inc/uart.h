#ifndef UART_H
#define UART_H

#include "MKL05Z4.h"
#include "../inc/circular_buff.h"

#define BAUDRATE 9600

void UART0_Init(void);
void UART0_IRQHandler();

circular_buf tx_buffor, rx_buffor;

#endif