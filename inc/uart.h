/**
 * @file uart.h
 * @author Konrad Sikora
 * @date Jan 2021
 */

#ifndef uart_h
#define uart_h

#include "MKL05Z4.h"
#include "../inc/circular_buff.h"

#define BAUDRATE 9600
#define DEFAULT_SYSTEM_CLOCK 41943040u

void UART0_init(void);
void uart_send(uint8_t *text);

extern circular_buff tx_buffor, rx_buffor;

#endif