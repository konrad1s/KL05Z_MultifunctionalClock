#include "MKL05Z4.h"
#ifndef mode_h
#define mode_h

#define TEMPERATURE_COMMAND "TEMPERATURE"

#define RX_STR_SIZE 30
#define TX_STR_SIZE 30

extern uint8_t mode;
extern uint8_t RTC_mode;

void chooseMode();

void uartMode();
void calculatorMode();
void temperatureMode();

void chooseModeRTC();
void ledModeOn();

void defaultRTCMode();
void setHours();
void setMinutes();
void setSeconds();
void setHoursAlarm();
void setMinutesAlarm();
void setSecondsAlarm();

#endif