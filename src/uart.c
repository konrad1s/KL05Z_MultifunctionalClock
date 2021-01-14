#include "../inc/uart0.h"

void UART0_Init(void)
{
    uint16_t baud_div;

    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;  //turn on clock
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;  //turn on clock
    SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1); // system clock source to uart

    //disable UART
    UART0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK);

    PORTB->PCR[1] = PORT_PCR_MUX(2); //PTB1=TX_D
    PORTB->PCR[2] = PORT_PCR_MUX(2); //PTB2=RX_D

    baud_div = (uint16_t)(DEFAULT_SYSTEM_CLOCK / (BAUDRATE * 16));

    UART0->BDH |= (baud_div >> 8) & UART0_BDH_SBR_MASK;
    UART0->BDL = (baud_div & UART0_BDL_SBR_MASK);

    //set osr value
    UART0->C4 &= ~UART0_C4_OSR_MASK;
    UART0->C4 |= UART0_C4_OSR(15);

    // both edge sampling
    UART0->C5 |= UART0_C5_BOTHEDGE_MASK;

    // enable interrupt
    UART0->C2 |= UART0_C2_RIE_MASK;

    //enable UART
    UART0->C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK);

    NVIC_EnableIRQ(UART0_IRQn);
    NVIC_ClearPendingIRQ(UART0_IRQn);
}

void UART0_IRQHandler()
{
}
