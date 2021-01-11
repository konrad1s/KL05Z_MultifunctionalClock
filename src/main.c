#include "MKL05Z4.h"
#include "../inc/keyboard.h"
#include "../inc/lcd1602.h"
#include "../inc/pit.h"
#include "../inc/calculate.h"
#include "../inc/rtc.h"
#include "../inc/buttons.h"
#include "../inc/dma.h"
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
  if (PORTB->ISFR & (1 << BUT1))
  {
    LCD1602_PrintXY("BUT1\0", 0, 0);
    while ((FPTB->PDIR & (1 << BUT1)) == 0)
      ;                                    //wait for button release
    PORTB->PCR[BUT1] |= PORT_PCR_ISF_MASK; //clear interrupt
  }

  if (PORTB->ISFR & (1 << BUT2))
  {
    LCD1602_PrintXY("BUT2\0", 0, 0);
    while ((FPTB->PDIR & (1 << BUT2)) == 0)
      ;                                    //wait for button release
    PORTB->PCR[BUT2] |= PORT_PCR_ISF_MASK; //clear interrupt
  }

  if (PORTB->ISFR & (1 << BUT3))
  {
    LCD1602_PrintXY("BUT3\0", 0, 0);
    while ((FPTB->PDIR & (1 << BUT3)) == 0)
      ;                                    //wait for button release
    PORTB->PCR[BUT3] |= PORT_PCR_ISF_MASK; //clear interrupt
  }
}

void DMA0_IRQHandler(void)
{
  DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE_MASK; // clear interrupt
  DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_BCR(2);    // 2 bytes (16 bits) per transfer
}

int main(void)
{
  LCD1602_Init(); // initialize LCD
  LCD1602_Backlight(TRUE);
  KB_init();       // initialize Keyboard
  PIT_Init();      // initialize PIT
  RTC_init();      // initialize RTC
  BUTTOONS_init(); // initialize buttons

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
