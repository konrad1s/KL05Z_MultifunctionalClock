#include "MKL05Z4.h"
#include "../inc/mode.h"
#include "../inc/pit.h"
#include "../inc/dma.h"
#include "../inc/lcd1602.h"
#include "../inc/rtc.h"
#include "../inc/buttons.h"
#include "../inc/uart.h"

uint8_t mode = 0;
uint8_t RTC_mode = 0;
uint8_t prev_mode = 0;

float temperature_result = 0;

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

void uartMode()
{
	static char rx_str[RX_STR_SIZE]="\0";
	static uint8_t i=0;
	uint8_t tx_str[TX_STR_SIZE]="\0";
  {
    // check rx buffor
    if (CB_buff_empty(&rx_buffor) == buffor_not_empty)
    {
      // read data
      uint8_t data = CB_read_data(&rx_buffor);

      if (data!='\r') 
      {
				rx_str[i]=data;
        i++;
      }
			
			// if \r is read
			else 
			{
				if(strcmp (rx_str,TEMPERATURE_COMMAND)==0)
				{
					temperatureMode();
					mode=1;
					snprintf(tx_str, TX_STR_SIZE, "Temperature=%0.1f%cC \r\n", temperature_result, 0xBA);
					uart_send((uint8_t*)tx_str);
					memset(rx_str,0,RX_STR_SIZE * sizeof(uint8_t));
					i=0;
				}
					else
			{
					snprintf(tx_str, TX_STR_SIZE, "Bad comment \r\n");
					uart_send((uint8_t*)tx_str);
					memset(rx_str,0,RX_STR_SIZE * sizeof(uint8_t));
					i=0;
			}
			}
		
    }
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
  float Ut25 = 0.716;
  float m = 0.00162;

  if (irqPIT2)
  {
    temperature_result = 25.0 - (((DMAvalue[0] & 0xFFFF) * adc_volt_coeff) - Ut25) / m;
    snprintf(buff, 20, "T=%0.1f%cC   ", temperature_result, 0xDF);
    LCD1602_PrintXY(buff, 0, 0);
    irqPIT2 = 0;
  }
}

void chooseModeRTC()
{
  switch (RTC_mode)
  {
  case 0:
    defaultRTCMode();
    break;
  case 1:
    LCD1602_Blink_On();
    LCD1602_SetCursor(7, 1);
    setHours();
    break;
  case 2:
    LCD1602_SetCursor(10, 1);
    setMinutes();
    break;
  case 3:
    LCD1602_SetCursor(13, 1);
    setSeconds();
    break;
  default:
    RTC->SR |= RTC_SR_TCE_MASK; //enable RTC
    LCD1602_Blink_Off();
    RTC_mode = 0;
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
    LCD1602_SetCursor(7, 1);
    but3 = 0;
  }
}

void setMinutes()
{
  if (but3)
  {
    rtc_seconds_counter += 60;
    display_time();
    LCD1602_SetCursor(10, 1);
    but3 = 0;
  }
}

void setSeconds()
{
  if (but3)
  {
    rtc_seconds_counter++;
    display_time();
    LCD1602_SetCursor(13, 1);
    but3 = 0;
  }
}