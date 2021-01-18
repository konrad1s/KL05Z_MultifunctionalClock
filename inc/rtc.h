/**
 * @file rtc.h
 * @author Konrad Sikora
 * @date Jan 2021
 */

#include "MKL05Z4.h"
#ifndef rtc_h
#define rtc_h

extern uint8_t rtc_irq;
extern uint8_t rtc_alarm_irq;

extern uint32_t rtc_seconds_counter;
extern uint32_t rtc_hours, rtc_minutes, rtc_seconds;

extern uint32_t rtc_seconds_alarm_counter;
extern uint32_t rtc_alarm_hours, rtc_alarm_minutes, rtc_alarm_seconds;

void RTC_init(void);
void RTC_save(void);

void RTC_display_time(void);
void RTC_display_alarm(void);
void RTC_alarm_uart_send(void);
void RTC_time_uart_send(void);

#endif