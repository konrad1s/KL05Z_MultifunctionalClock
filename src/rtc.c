/**
 * @file rtc.c
 * @author Konrad Sikora
 * @date Jan 2021
 */

#include "../inc/rtc.h"
#include "../inc/lcd1602.h"
#include "../inc/uart.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

uint8_t rtc_irq = 1;
uint8_t rtc_alarm_irq = 0;

uint32_t rtc_seconds_counter = 0;
uint32_t rtc_hours = 0, rtc_minutes = 0, rtc_seconds = 0;

uint32_t rtc_seconds_alarm_counter = 0;
uint32_t rtc_alarm_hours = 0, rtc_alarm_minutes = 0, rtc_alarm_seconds = 0;

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

  //enable alarm interrupt
  RTC->IER |= RTC_IER_TAIE_MASK;

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

  //enable RTC alarm interrupt
  NVIC_ClearPendingIRQ(RTC_IRQn);
  NVIC_EnableIRQ(RTC_IRQn);
}

/**
 * @brief Display time on LCD
 */
void RTC_display_time(void)
{
  char time_print[20] = {"00:00:00"};
  rtc_seconds = rtc_seconds_counter % 60;
  rtc_minutes = rtc_seconds_counter / 60;
  rtc_hours = rtc_seconds_counter / 3600;
  rtc_minutes %= 60;
  rtc_hours %= 24;
  sprintf(time_print, "%2i:%02i:%02i  \0", rtc_hours, rtc_minutes, rtc_seconds);
  LCD1602_PrintXY(time_print, 6, 1);
}

/**
 * @brief Display alarm time on LCD
 */
void RTC_display_alarm()
{
  char time_print[20] = {"00:00:00"};
  rtc_alarm_seconds = rtc_seconds_alarm_counter % 60;
  rtc_alarm_minutes = rtc_seconds_alarm_counter / 60;
  rtc_alarm_hours = rtc_seconds_alarm_counter / 3600;
  rtc_alarm_minutes %= 60;
  rtc_alarm_hours %= 24;
  sprintf(time_print, "%2i:%02i:%02i  \0", rtc_alarm_hours, rtc_alarm_minutes, rtc_alarm_seconds);
  LCD1602_PrintXY(time_print, 6, 1);
}

/**
 * @brief Save values to RTC registers
 */
void RTC_save(void)
{
  RTC->SR &= ~RTC_SR_TCE_MASK;

  //set rtc secounds counter
  rtc_seconds_counter = rtc_hours * 3600 + rtc_minutes * 60 + rtc_seconds;
  //set rtc alarm secounds counter
  rtc_seconds_alarm_counter = rtc_alarm_hours * 3600 + rtc_alarm_minutes * 60 + rtc_alarm_seconds;

  // set TSR value
  RTC->TSR = rtc_seconds_counter;
  // set TAR value
  RTC->TAR = rtc_seconds_alarm_counter;

  //enable RTC
  RTC->SR |= RTC_SR_TCE_MASK;
}

/**
 * @brief Send alarm time to uart
 */
void RTC_alarm_uart_send(void)
{

  uint8_t tx_str[50] = "\0";
  snprintf(tx_str, 50, "Alarm has been set: (hh:mm:ss) %2i:%02i:%02i\r\n", rtc_alarm_hours, rtc_alarm_minutes, rtc_alarm_seconds);
  UART0_send((uint8_t *)tx_str);
}

/**
 * @brief Send time to uart
 */
void RTC_time_uart_send(void)
{

  uint8_t tx_str[50] = "\0";
  snprintf(tx_str, 50, "time: (hh:mm:ss) %2i:%02i:%02i\r\n", rtc_hours, rtc_minutes, rtc_seconds);
  UART0_send((uint8_t *)tx_str);
}

void RTC_Seconds_IRQHandler(void)
{
  // increment sec counter
  rtc_seconds_counter++;
  rtc_irq = 1;
}

void RTC_IRQHandler(void)
{
  //if RTC alarm flag is set
  if (RTC->SR & RTC_SR_TAF_MASK)
  {
    rtc_alarm_irq = 1;
    //disable alarm interrupt, write new value
    RTC->TAR = RTC->TAR;
  }
}
