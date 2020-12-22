#include "MKL05Z4.h"
#include "keyboard.h"
#include "lcd1602.h"
#include "pit.h"
#include "calculate.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool irqPIT = 0;

void PIT_IRQHandler()
{
    if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK)
    {
        if (irqPIT == 0)
            irqPIT = 1;
        PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
    }
    NVIC_ClearPendingIRQ(PIT_IRQn);
}

int main(void)
{
    LCD1602_Init(); /* initialize LCD */
    LCD1602_Backlight(TRUE);
    KB_init();
    PIT_Init();

    while (1)
    {
        if (irqPIT)
        {
            loop();
            irqPIT = 0;
        }

        __wfi(); // Save energy and wait for interrupt
    }
}


