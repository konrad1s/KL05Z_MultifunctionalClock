#include "MKL05Z4.h"
#ifndef rtc_h
#define rtc_h

extern uint8_t irqRTC;
extern uint8_t alarmRTC;

extern uint32_t rtc_seconds_counter;
extern uint32_t rtc_hours, rtc_minutes, rtc_seconds;

extern uint32_t rtc_seconds_alarm_counter;
extern uint32_t rtc_alarm_hours, rtc_alarm_minutes, rtc_alarm_seconds;

void RTC_init(void);
void RTC_save(void);
void display_time(void);
void alarm_uart_send(void);
void rtc_uart_send(void);

#endif