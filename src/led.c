#include "MKL05Z4.h"
#include "../inc/led.h"

void LEDs_init()
{
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;

    PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1); //GPIO MUX
    PORTB->PCR[GREEN_LED] |= PORT_PCR_MUX(1);
    PORTB->PCR[BLUE_LED] |= PORT_PCR_MUX(1);

    PTB->PDDR |= (1 << RED_LED) | (1 << GREEN_LED) | (1 << BLUE_LED); // set as output
    PTB->PSOR |= (1 << GREEN_LED) | (1 << BLUE_LED); //turn LEDs off
}

void LEDs_off()
{
    PTB->PSOR |= (1 << RED_LED) | (1 << GREEN_LED) | (1 << BLUE_LED);
}

void LED_on(LED_Color color)
{
    PTB->PCOR |= (1 << color);
}

void LED_toggle(LED_Color color)
{
    PTB->PTOR |= (1 << color);
}