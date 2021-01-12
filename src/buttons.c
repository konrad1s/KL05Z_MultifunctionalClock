#include "MKL05Z4.h"
#include "../inc/buttons.h"
#include "../inc/rtc.h"

void PORTB_IRQHandler(void)
{
  if (PORTB->ISFR & (1 << BUT1))
  {
    rtc_seconds_counter += 3600;
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
