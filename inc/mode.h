#include "MKL05Z4.h"
#ifndef mode_h
#define mode_h

extern uint8_t mode;
extern uint8_t RTC_mode;

void chooseMode();
void calculatorMode();
void temperatureMode();

void chooseModeRTC();

void defaultRTCMode();
void setHours();
void setMinutes();
void setSeconds();


#endif