#include "MKL05Z4.h"
#include "../inc/buttons.h"
#include "../inc/rtc.h"
#include "../inc/mode.h"

uint8_t but3 = 0;

void PORTB_IRQHandler(void)
{
  if (PORTB->ISFR & (1 << BUT1))
  {
    if (mode == 1)
      mode = 0;
    else
      mode = 1;

    while ((FPTB->PDIR & (1 << BUT1)) == 0)
      ;                                    //wait for button release
    PORTB->PCR[BUT1] |= PORT_PCR_ISF_MASK; //clear interrupt
  }

  if (PORTB->ISFR & (1 << BUT2))
  {
    if (RTC_mode < 3)
      RTC_mode++;
    else
      RTC_mode = 0;
    while ((FPTB->PDIR & (1 << BUT2)) == 0)
      ;                                    //wait for button release
    PORTB->PCR[BUT2] |= PORT_PCR_ISF_MASK; //clear interrupt
  }

  if (PORTB->ISFR & (1 << BUT3))
  {
    but3 = 1;
    while ((FPTB->PDIR & (1 << BUT3)) == 0)
      ;                                    //wait for button release
    PORTB->PCR[BUT3] |= PORT_PCR_ISF_MASK; //clear interrupt
  }
}

void BUTTOONS_init()
{

  SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; // turn on clock on port B

  // set pins mux as gpio
  PORTB->PCR[BUT1] |= PORT_PCR_MUX(1);
  PORTB->PCR[BUT2] |= PORT_PCR_MUX(1);
  PORTB->PCR[BUT3] |= PORT_PCR_MUX(1);

  // set pins as onput
  PTA->PDOR &= ~(1 << BUT1);
  PTA->PDOR &= ~(1 << BUT2);
  PTA->PDOR &= ~(1 << BUT3);

  // enable pull-up resistor on columns
  //rising edge
  PORTB->PCR[BUT1] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK | PORT_PCR_IRQC(0xA);
  PORTB->PCR[BUT2] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK | PORT_PCR_IRQC(0xA);
  PORTB->PCR[BUT3] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK | PORT_PCR_IRQC(0xA);

  NVIC_ClearPendingIRQ(PORTB_IRQn); // Clear NVIC any pending interrupts on PORTB
  NVIC_EnableIRQ(PORTB_IRQn);       // Enable interrupts for PORTB
}
