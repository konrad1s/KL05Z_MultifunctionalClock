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