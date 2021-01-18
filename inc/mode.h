/**
 * @file mode.h
 * @author Konrad Sikora
 * @date Jan 2021
 */

#include "MKL05Z4.h"
#ifndef mode_h
#define mode_h

#define TEMPERATURE_COMMAND "TEMPERATURE"

#define RX_STR_SIZE 30
#define TX_STR_SIZE 30

extern uint8_t mode;
extern uint8_t mode_rtc;

void choose_mode();

void uart_mode();
void calculator_mode();
void temperature_mode();

void RTC_choose_mode();
void RTC_alarm_mode();
void RTC_default_mode();

void LED_on_mode();

#endif