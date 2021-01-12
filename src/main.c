#include "MKL05Z4.h"
#include "../inc/keyboard.h"
#include "../inc/lcd1602.h"
#include "../inc/pit.h"
#include "../inc/calculate.h"
#include "../inc/rtc.h"
#include "../inc/buttons.h"
#include "../inc/dma.h"
#include "../inc/adc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void Init_all();

int main(void)
{
  Init_all();

  int avg = 0;
  char buff[20] = "\0";

  LCD1602_PrintXY("Time:\0", 0, 1);

  while (1)
  {
    if (irqPIT)
    {
      // loop();
      irqPIT = 0;
    }
    if (irqRTC)
    {
      display_time();
      irqRTC = 0;
    }

    if (irqPIT2)
    {
      float adc_volt_coeff = ((float)(((float)2.91) / 4095));
      float wynik = 0;
      float Ut25 = 0.716;
      float m = 0.00162;
      wynik = 25.0 - (((DMAvalue[0] & 0xFFFF) * adc_volt_coeff) - Ut25) / m;
      snprintf(buff, 20, "T=%0.1f%cC   ", wynik, 0xDF);
      LCD1602_PrintXY(buff, 0, 0);
      irqPIT2 = 0;
    }

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
}
