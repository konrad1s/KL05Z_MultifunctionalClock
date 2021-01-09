#include "MKL05Z4.h"
#include "rtc.h"

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
}