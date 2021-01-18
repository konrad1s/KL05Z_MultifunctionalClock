/**
 * @file pit.c
 * @author Konrad Sikora
 * @date Jan 2021
 */

#include "MKL05Z4.h"
#include "../inc/pit.h"

uint8_t irqPIT = 0;
uint8_t irqPIT2 = 0;

void PIT_IRQHandler()
{
	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK)
	{
		if (irqPIT == 0)
			irqPIT = 1;

		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK; // clear the timer interrupt flag
	}

	if (PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK)
	{
		if (!irqPIT2)
		{
			irqPIT2 = 1;
			ADC0->SC1[0] = ADC_SC1_ADCH(26) | (ADC0->SC1[0] & (ADC_SC1_AIEN_MASK));
		}

		PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK; // clear the timer interrupt flag
	}
	NVIC_ClearPendingIRQ(PIT_IRQn);
}

void PIT_init(void)
{
	uint32_t tsv;
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; // enable the clock to the PIT module
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;	  // turn on the PIT

	tsv = BUS_CLOCK / 5;											 // interrupt evry 200ms
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(tsv);						 // default start value
	PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; // enable timer interrupt

	tsv = BUS_CLOCK / 2;											 // interrupt evry 500ms
	PIT->CHANNEL[1].LDVAL = PIT_LDVAL_TSV(tsv);						 // default start value
	PIT->CHANNEL[1].TCTRL = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; // enable timer interrupt

	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_EnableIRQ(PIT_IRQn); // initialize the NVIC to enable the PIT interrupt
}