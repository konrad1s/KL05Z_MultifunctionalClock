#include "../inc/led.h"

#define RED_LED 8
#define GREEN_LED 9
#define BLUE_LED 10

void LED_init()
{
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;

    PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1); //GPIO MUX
    PORTB->PCR[GREEN_LED] |= PORT_PCR_MUX(1);
    PORTB->PCR[BLUE_LED] |= PORT_PCR_MUX(1);

    PTB->PDDR |= (1 << RED_LED); // set as output
    PTB->PSOR |= (1 << RED_LED); //turn LED off

    PTB->PDDR |= (1 << GREEN_LED);
    PTB->PSOR |= (1 << GREEN_LED);

    PTB->PDDR |= (1 << BLUE_LED);
    PTB->PSOR |= (1 << BLUE_LED);
}