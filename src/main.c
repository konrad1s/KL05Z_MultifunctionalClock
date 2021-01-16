#include "MKL05Z4.h"
#include "../inc/keyboard.h"
#include "../inc/lcd1602.h"
#include "../inc/pit.h"
#include "../inc/calculate.h"
#include "../inc/rtc.h"
#include "../inc/buttons.h"
#include "../inc/dma.h"
#include "../inc/adc.h"
#include "../inc/uart.h"
#include "../inc/mode.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void Init_all();

int main(void)
{
  Init_all();
  LCD1602_PrintXY("Time:\0", 0, 1);
	
	uint8_t tx_str[10]="test\0";
	uart_log((uint8_t*)tx_str);
	
  while (1)
  {
    chooseModeRTC();
    chooseMode();
    __wfi(); // save energy and wait for interrupt
  }
}

void Init_all()
{
  LCD1602_Init(); // initialize LCD
  LCD1602_Backlight(TRUE);
  KB_init();       // initialize Keyboard
  PIT_init();      // initialize PIT
  RTC_init();      // initialize RTC
  BUTTOONS_init(); // initialize buttons
  ADC_init();      // initialize ADC
  DMA_init();      // initialize DMA
	UART0_Init();
}
