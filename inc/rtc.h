#include "MKL05Z4.h"
#ifndef rtc_h
#define rtc_h

extern uint8_t irqRTC;

extern uint32_t rtc_seconds_counter;
extern uint32_t rtc_hours , rtc_minutes , rtc_seconds;

void RTC_init(void);
void display_time(void);

#endif