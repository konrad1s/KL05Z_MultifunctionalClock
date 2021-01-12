#include "MKL05Z4.h"
#include "../inc/mode.h"
#include "../inc/pit.h"
#include "../inc/dma.h"
#include "../inc/lcd1602.h"
#include "../inc/rtc.h"
#include "../inc/buttons.h"

uint8_t mode = 0;
uint8_t RTC_mode = 0;
uint8_t prev_mode = 0;

void chooseMode()
{
  switch (mode)
  {
  case 0:
    calculatorMode();
    break;
  case 1:
    temperatureMode();
    break;
  }
  if (prev_mode != mode)
  {
    LCD1602_ClearRow(0);
    CalculatorReset();
    prev_mode = mode;
  }
}

void calculatorMode()
{
  if (irqPIT)
  {
    CalculatorLoop();
    irqPIT = 0;
  }
}
void temperatureMode()
{
  char buff[20] = "\0";
  float adc_volt_coeff = ((float)(((float)2.91) / 4095));
  float result = 0;
  float Ut25 = 0.716;
  float m = 0.00162;

  if (irqPIT2)
  {
    result = 25.0 - (((DMAvalue[0] & 0xFFFF) * adc_volt_coeff) - Ut25) / m;
    snprintf(buff, 20, "T=%0.1f%cC   ", result, 0xDF);
    LCD1602_PrintXY(buff, 0, 0);
    irqPIT2 = 0;
  }
}

void chooseModeRTC()
{
  switch (RTC_mode)
  {
  case 0:
    RTC->SR |= RTC_SR_TCE_MASK; //enable RTC
    defaultRTCMode();
    break;
  case 1:
    setHours();
    break;
  case 2:
    setMinutes();
    break;
  case 3:
    setSeconds();
    break;
  }
}

void defaultRTCMode()
{
  if (irqRTC)
  {
    display_time();
    but3 = 0;
    irqRTC = 0;
  }
}

void setHours()
{
  if (but3)
  {
    RTC->SR &= ~RTC_SR_TCE_MASK; //disable RTC
    rtc_seconds_counter += 3600;
    display_time();

    but3 = 0;
  }
}

void setMinutes()
{
  if (but3)
  {
    rtc_seconds_counter += 60;
    display_time();
    but3 = 0;
  }
}

void setSeconds()
{
  if (but3)
  {
    rtc_seconds_counter++;
    display_time();
    but3 = 0;
  }
}