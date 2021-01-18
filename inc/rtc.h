/**
 * @file rtc.h
 * @author Konrad Sikora
 * @date Jan 2021
 */

#include "MKL05Z4.h"
#ifndef rtc_h
#define rtc_h

typedef struct RTC_time
{
    uint32_t seconds_counter;
    uint32_t hours;
    uint32_t minutes;
    uint32_t seconds;
} RTC_time;

extern uint8_t rtc_irq;
extern uint8_t rtc_alarm_irq;

void RTC_init(void);
void RTC_save(void);

void RTC_display_time(void);
void RTC_display_alarm(void);
void RTC_alarm_uart_send(void);
void RTC_time_uart_send(void);

void RTC_set_hours(void);
void RTC_set_minutes(void);
void RTC_set_seconds(void);
void RTC_alarm_set_hours(void);
void RTC_alarm_set_minutes(void);
void RTC_alarm_set_seconds(void);

#endif