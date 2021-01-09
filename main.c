#include "MKL05Z4.h"
#include "keyboard.h"
#include "lcd1602.h"
#include "pit.h"
#include "calculate.h"
#include "rtc.h"
#include "buttons.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool irqPIT = 0;
bool irqRTC = 1;

uint32_t rtc_seconds_counter = 0;
uint32_t rtc_hours = 0, rtc_minutes = 0, rtc_seconds = 0;

void PIT_IRQHandler()
{
  if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK)
  {
    if (irqPIT == 0)
      irqPIT = 1;
    PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK; // clear the timer interrupt flag
  }
  NVIC_ClearPendingIRQ(PIT_IRQn);
}

void RTC_Seconds_IRQHandler(void)
{
  rtc_seconds_counter++;
  irqRTC = 1;
}

void PORTB_IRQHandler(void)
{
	if(PORTB->ISFR &(1<<BUT1)) 
	{
		PORTB->PCR[BUT1] |= PORT_PCR_ISF_MASK;	//clear interrupt
	}
	
		if(PORTB->ISFR &(1<<BUT2)) 
	{
		PORTB->PCR[BUT2] |= PORT_PCR_ISF_MASK;	//clear interrupt
	}
	
		if(PORTB->ISFR &(1<<BUT3)) 
	{
		PORTB->PCR[BUT3] |= PORT_PCR_ISF_MASK;	//clear interrupt
	}
}

int main(void)
{
  LCD1602_Init(); // initialize LCD
  LCD1602_Backlight(TRUE);
  KB_init();  // initialize Keyboard
  PIT_Init(); // initialize PIT
  RTC_init(); // initialize RTC

  LCD1602_PrintXY("Time:\0", 0, 1);

  while (1)
  {
    if (irqPIT)
    {
      loop();
      irqPIT = 0;
    }
    if (irqRTC)
    {
      display_time();
      irqRTC = 0;
    }

    __wfi(); // save energy and wait for interrupt
  }
}
