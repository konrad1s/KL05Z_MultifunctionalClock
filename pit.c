#include "MKL05Z4.h"
#include "pit.h"

void PIT_Init(void)
{
	uint32_t tsv;
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;	// enable the clock to the PIT module
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;	// turn on the PIT
	tsv = BUS_CLOCK / 5;	// interrupt evry 200ms
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(tsv);	// default start value
	PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; // enable timer interrupt
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_EnableIRQ(PIT_IRQn); // initialize the NVIC to enable the PIT interrupt
}