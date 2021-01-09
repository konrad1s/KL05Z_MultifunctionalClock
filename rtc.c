#include "MKL05Z4.h"

void RTC_init(void)
{
    unsigned int prev_val;
    // enable the clock to SRTC module
    SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;

    //select RTC clock
    SIM->SOPT1 &= ~SIM_SOPT1_OSC32KSEL_MASK;
    SIM->SOPT1 |= SIM_SOPT1_OSC32KSEL(0);

    //enable time seconds interrupt
    RTC->IER = RTC_IER_TSIE_MASK;

    //reset RTC if time is invalid (Time Invalid Flag)
    if (RTC->SR & RTC_SR_TIF_MASK)
    {
        RTC->CR |= RTC_CR_SWR_MASK;
        RTC->CR &= ~RTC_CR_SWR_MASK;
    }

    //read and set prev value
    prev_val = RTC->TSR;
    RTC->TSR = prev_val;

    // enable rtc
    RTC->SR |= RTC_SR_TCE_MASK;

    //enable RTC seconds interrupt
    NVIC_ClearPendingIRQ(RTC_Seconds_IRQn);
    NVIC_SetPriority(RTC_Seconds_IRQn, 10);
    NVIC_EnableIRQ(RTC_Seconds_IRQn);
}