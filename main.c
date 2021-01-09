#include "MKL05Z4.h"
#include "keyboard.h"
#include "lcd1602.h"
#include "pit.h"
#include "calculate.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//typedef bool in calculate.h
bool irqPIT = 0;
uint32_t rtc_seconds;

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
    rtc_seconds++;
}

int main(void)
{
    LCD1602_Init(); // initialize LCD
    LCD1602_Backlight(TRUE);
    KB_init();  // initialize Keyboard
    PIT_Init(); // initialize PIT

    while (1)
    {
        if (irqPIT)
        {
            loop();
            irqPIT = 0;
        }

        __wfi(); // save energy and wait for interrupt
    }
}
