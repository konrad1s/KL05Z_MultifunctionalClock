#include "MKL05Z4.h"
#include "keyboard.h"
#include "lcd1602.h"
#include "pit.h"
#include "calculate.h"
#include "rtc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//typedef bool in calculate.h
bool irqPIT = 0;
bool irqRTC=1;
	
volatile uint32_t rtc_seconds_counter = 0;
uint32_t rtc_hours = 0, rtc_minutes = 0, rtc_seconds = 0;

void display_time(void);

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
	irqRTC=1;
}

int main(void)
{
    LCD1602_Init(); // initialize LCD
    LCD1602_Backlight(TRUE);
    KB_init();  // initialize Keyboard
    PIT_Init(); // initialize PIT
		RTC_init(); // initialize RTC
	
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
					irqRTC=0;
				}
				
        __wfi(); // save energy and wait for interrupt
    }
}

void display_time(void)
{
		LCD1602_SetCursor(0,1);
		char time_print[20] = { "00:00:00" };
    rtc_seconds = rtc_seconds_counter % 60;
    rtc_minutes = rtc_seconds_counter / 60;
    rtc_hours = rtc_seconds_counter / 3600;
    rtc_minutes %= 60;
    rtc_hours %= 3600;
		sprintf(time_print, "Time: %2i:%02i:%02i", rtc_hours, rtc_minutes,	rtc_seconds);
		LCD1602_Print(time_print);
}