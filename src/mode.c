/**
 * @file mode.c
 * @author Konrad Sikora
 * @date Jan 2021
 */

#include "MKL05Z4.h"
#include "../inc/mode.h"
#include "../inc/pit.h"
#include "../inc/dma.h"
#include "../inc/lcd1602.h"
#include "../inc/rtc.h"
#include "../inc/buttons.h"
#include "../inc/uart.h"
#include "../inc/led.h"

uint8_t mode = 0;
uint8_t mode_rtc = 0;
uint8_t prev_mode = 0;

float temperature_result = 0;

void choose_mode()
{
  switch (mode)
  {
  case 0:
    calculator_mode();
    break;
  case 1:
    temperature_mode();
    break;
  case 2:
    break;
  }
  if (prev_mode != mode)
  {
    LCD1602_ClearRow(0);
    Calculator_reset();
    LEDs_off();
    LED_on_mode();
    prev_mode = mode;
  }
}

void uart_mode()
{
  static char rx_str[RX_STR_SIZE] = "\0";
  static uint8_t i = 0;
  uint8_t tx_str[TX_STR_SIZE] = "\0";
  {
    // check rx buffor
    if (CB_buff_empty(&rx_buffor) == buffor_not_empty)
    {
      // read data
      uint8_t data = CB_read_data(&rx_buffor);

      if (data != 0x0A)
      {
        rx_str[i] = data;
        i++;
      }

      // if \n is read
      else
      {
        if (strcmp(rx_str, TEMPERATURE_COMMAND) == 0)
        {
          temperature_mode();
          mode = 1;
          snprintf(tx_str, TX_STR_SIZE, "Temperature=%0.1f%cC \r\n", temperature_result, 0xBA);
          UART0_send((uint8_t *)tx_str);
          memset(rx_str, 0, RX_STR_SIZE * sizeof(uint8_t));
          i = 0;
        }
        else
        {
          if (mode == 2)
          {
            snprintf(tx_str, TX_STR_SIZE, "%s", rx_str);
            LCD1602_ClearRow(0);
            LCD1602_PrintXY(tx_str, 0, 0);
          }
          memset(rx_str, 0, RX_STR_SIZE * sizeof(uint8_t));
          i = 0;
        }
      }
    }
  }
}

void calculator_mode()
{
  if (pit_irq)
  {
    Calculator_loop();
    pit_irq = 0;
  }
}

void temperature_mode()
{
  char buff[20] = "\0";
  float adc_volt_coeff = ((float)(((float)2.91) / 4095));
  float Ut25 = 0.716;
  float m = 0.00162;

  if (pit2_irq)
  {
    temperature_result = 25.0 - (((DMA_value[0] & 0xFFFF) * adc_volt_coeff) - Ut25) / m;
    snprintf(buff, 20, "T=%0.1f%cC   ", temperature_result, 0xDF);
    LCD1602_PrintXY(buff, 0, 0);
    pit2_irq = 0;
  }
}

void RTC_choose_mode()
{
  switch (mode_rtc)
  {
  case 0:
    RTC_default_mode();
    break;
  case 1:
    LCD1602_Blink_On();
    LCD1602_SetCursor(7, 1);
    RTC_set_hours();
    break;
  case 2:
    LCD1602_SetCursor(10, 1);
    RTC_set_minutes();
    break;
  case 3:
    LCD1602_SetCursor(13, 1);
    RTC_set_seconds();
    break;
  case 4:
    LCD1602_SetCursor(7, 1);
    RTC_alarm_set_hours();
    break;
  case 5:
    LCD1602_SetCursor(10, 1);
    RTC_alarm_set_minutes();
    break;
  case 6:
    LCD1602_SetCursor(13, 1);
    RTC_alarm_set_seconds();
    break;
  default:
    RTC_save();
    RTC->SR |= RTC_SR_TCE_MASK; //enable RTC
    LCD1602_Blink_Off();
    RTC_alarm_uart_send();
    mode_rtc = 0;
    break;
  }
}

void RTC_alarm_mode()
{
  static uint8_t rtc_alarm_prev = 0;

  if (rtc_alarm_irq == 1)
  {
    buzzer_on();
    UART0_send("ALARM\r\n");
    rtc_alarm_prev = 1;
    rtc_alarm_irq = 2;
  }

  else if (rtc_alarm_irq == 0 && rtc_alarm_prev)
  {
    buzzer_off();
    UART0_send("Alarm has been cleared. ");
    RTC_time_uart_send();
    rtc_alarm_prev = 0;
    rtc_alarm_irq = 2;
  }
}

void RTC_default_mode()
{
  if (rtc_irq)
  {
    RTC_display_time();
    but3_irq = 0;
    rtc_irq = 0;
  }
}

void LED_on_mode()
{
  if (mode == 0)
    LED_on(RED_LED);
  else if (mode == 1)
    LED_on(GREEN_LED);
  else if (mode == 2)
    LED_on(BLUE_LED);
}
