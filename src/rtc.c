#include "MKL05Z4.h"
#include "../inc/rtc.h"
#include "../inc/lcd1602.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


extern uint32_t rtc_seconds_counter;
extern uint32_t rtc_hours , rtc_minutes , rtc_seconds;

void RTC_init(void)
{
  int i;
  // enable the clock to SRTC module
  SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;

  // select RTC clock, system oscillator (OSC32KCLK)
  SIM->SOPT1 &= ~SIM_SOPT1_OSC32KSEL_MASK;
  SIM->SOPT1 |= SIM_SOPT1_OSC32KSEL(0);

  // reset RTC_CR bit
  RTC->CR |= RTC_CR_SWR_MASK;
  RTC->CR &= ~RTC_CR_SWR_MASK;

  //  clear TIF (Time Invalid Flag)
  if (RTC->SR & RTC_SR_TIF_MASK)
  {
    RTC->TSR = 0x00000000;
  }

  //enable time seconds interrupt
  RTC->IER |= RTC_IER_TSIE_MASK;

  //set value
  RTC->TSR = 1;

  //enable oscilator and wait for stabilization
  RTC->CR |= RTC_CR_OSCE_MASK | RTC_CR_SC16P_MASK;
  for (i = 0; i < 0x600000; i++)
    ;

  // enable rtc
  RTC->SR |= RTC_SR_TCE_MASK;

  //enable RTC seconds interrupt
  NVIC_ClearPendingIRQ(RTC_Seconds_IRQn);
  NVIC_EnableIRQ(RTC_Seconds_IRQn);
}

void display_time(void)
{
		char time_print[20] = { "00:00:00" };
    rtc_seconds = rtc_seconds_counter % 60;
    rtc_minutes = rtc_seconds_counter / 60;
    rtc_hours = rtc_seconds_counter / 3600;
    rtc_minutes %= 60;
    rtc_hours %= 3600;
		sprintf(time_print, "%2i:%02i:%02i  \0", rtc_hours, rtc_minutes,	rtc_seconds);
		LCD1602_PrintXY(time_print,6,1);
}