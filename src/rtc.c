/**
 * @file rtc.c
 * @author Konrad Sikora
 * @date Jan 2021
 */

#include "../inc/rtc.h"
#include "../inc/lcd1602.h"
#include "../inc/uart.h"
#include "../inc/buttons.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

uint8_t rtc_irq = 1;
uint8_t rtc_alarm_irq = 0;

RTC_time rtc_time;
RTC_time rtc_alarm_time;

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
  rtc_time.seconds = rtc_time.seconds_counter % 60;
  rtc_time.minutes = rtc_time.seconds_counter / 60;
  rtc_time.hours = rtc_time.seconds_counter / 3600;
  rtc_time.minutes %= 60;
  rtc_time.hours %= 24;
  sprintf(time_print, "%2i:%02i:%02i  \0", rtc_time.hours, rtc_time.minutes, rtc_time.seconds);
  LCD1602_PrintXY(time_print, 6, 1);
}

/**
 * @brief Display alarm time on LCD
 */
void RTC_display_alarm()
{
  char time_print[20] = {"00:00:00"};
  rtc_alarm_time.seconds = rtc_alarm_time.seconds_counter % 60;
  rtc_alarm_time.minutes = rtc_alarm_time.seconds_counter / 60;
  rtc_alarm_time.hours = rtc_alarm_time.seconds_counter / 3600;
  rtc_alarm_time.minutes %= 60;
  rtc_alarm_time.hours %= 24;
  sprintf(time_print, "%2i:%02i:%02i  \0", rtc_alarm_time.hours, rtc_alarm_time.minutes, rtc_alarm_time.seconds);
  LCD1602_PrintXY(time_print, 6, 1);
}

/**
 * @brief Save values to RTC registers
 */
void RTC_save(void)
{
  RTC->SR &= ~RTC_SR_TCE_MASK;

  //set rtc secounds counter
  rtc_time.seconds_counter = rtc_time.hours * 3600 + rtc_time.minutes * 60 + rtc_time.seconds;
  //set rtc alarm secounds counter
  rtc_alarm_time.seconds_counter = rtc_alarm_time.hours * 3600 + rtc_alarm_time.minutes * 60 + rtc_alarm_time.seconds;

  // set TSR value
  RTC->TSR = rtc_time.seconds_counter;
  // set TAR value
  RTC->TAR = rtc_alarm_time.seconds_counter;

  //enable RTC
  RTC->SR |= RTC_SR_TCE_MASK;
}

/**
 * @brief Send alarm time to uart
 */
void RTC_alarm_uart_send(void)
{

  uint8_t tx_str[50] = "\0";
  snprintf(tx_str, 50, "Alarm has been set: (hh:mm:ss) %2i:%02i:%02i\r\n", rtc_alarm_time.hours, rtc_alarm_time.minutes, rtc_alarm_time.seconds);
  UART0_send((uint8_t *)tx_str);
}

/**
 * @brief Send time to uart
 */
void RTC_time_uart_send(void)
{

  uint8_t tx_str[50] = "\0";
  snprintf(tx_str, 50, "Time: (hh:mm:ss) %2i:%02i:%02i\r\n", rtc_time.hours, rtc_time.minutes, rtc_time.seconds);
  UART0_send((uint8_t *)tx_str);
}

/**
 * @brief Set rtc hours
 */
void RTC_set_hours(void)
{
  if (but3_irq)
  {
    RTC->SR &= ~RTC_SR_TCE_MASK; //disable RTC
    rtc_time.seconds_counter += 3600;
    RTC_display_time();
    LCD1602_SetCursor(7, 1);
    but3_irq = 0;
  }
}

/**
 * @brief Set rtc minutes
 */
void RTC_set_minutes(void)
{
  if (but3_irq)
  {
    rtc_time.seconds_counter += 60;
    RTC_display_time();
    LCD1602_SetCursor(10, 1);
    but3_irq = 0;
  }
}

/**
 * @brief Set rtc seconds
 */
void RTC_set_seconds(void)
{
  if (but3_irq)
  {
    rtc_time.seconds_counter++;
    RTC_display_time();
    LCD1602_SetCursor(13, 1);
    but3_irq = 0;
  }
}

/**
 * @brief Set rtc alarm hours
 */
void RTC_alarm_set_hours(void)
{
  if (but3_irq)
  {
    rtc_alarm_time.seconds_counter += 3600;
    RTC_display_alarm();
    LCD1602_SetCursor(7, 1);
    but3_irq = 0;
  }
}

/**
 * @brief Set rtc alarm minutes
 */
void RTC_alarm_set_minutes(void)
{
  if (but3_irq)
  {
    rtc_alarm_time.seconds_counter += 60;
    RTC_display_alarm();
    LCD1602_SetCursor(10, 1);
    but3_irq = 0;
  }
}

/**
 * @brief Set rtc alarm seconds
 */
void RTC_alarm_set_seconds(void)
{
  if (but3_irq)
  {
    rtc_alarm_time.seconds_counter++;
    RTC_display_alarm();
    LCD1602_SetCursor(13, 1);
    but3_irq = 0;
  }
}

/**
 * @brief RTC irq handler
 */
void RTC_Seconds_IRQHandler(void)
{
  // increment sec counter
  rtc_time.seconds_counter++;
  rtc_irq = 1;
}

/**
 * @brief RTC alarm irq handler
 */
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
