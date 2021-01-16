#include "../inc/uart.h"
#include "../inc/lcd1602.h"

circular_buff tx_buffor, rx_buffor;

volatile uint8_t data;

void UART0_Init(void)
{
    uint16_t baud_div;

    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;  //turn on clock
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;  //turn on clock
    SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1); // system clock source to uart

    tx_buffor.buffor = CB_init(&tx_buffor);
    tx_buffor.head = tx_buffor.data;
    tx_buffor.tail = tx_buffor.data;
    tx_buffor.buffor = tx_buffor.data;
    tx_buffor.new_len = 0;

    rx_buffor.buffor = CB_init(&rx_buffor);
    rx_buffor.head = rx_buffor.data;
    rx_buffor.tail = rx_buffor.data;
    rx_buffor.buffor = rx_buffor.data;
    rx_buffor.new_len = 0;

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
    CB_state buffor_state = CB_buff_empty(&tx_buffor);

    // if TRDE flag is set and buffor not empty
    if ((UART0->S1 & UART0_S1_TDRE_MASK) && buffor_state == buffor_not_empty)
    {
        UART0->D = CB_read_data(&tx_buffor);
    }
    // else if buffor is empty
    else if (buffor_state == buffor_empty)
    {
        UART0->C2 &= ~UART0_C2_TIE_MASK;
    }

    buffor_state = CB_buff_full(&rx_buffor);

    // if RDRF flag is set and buffor not full
    if ((UART0->S1 & UART0_S1_RDRF_MASK) && buffor_state == buffor_not_full)
    {
        data = UART0->D;
        CB_add_data(&rx_buffor, data);
    }

    // else if buffor is empty
    else if (buffor_state == buffor_empty)
    {
        // disable rx interrupt
        UART0->C2 &= ~UART0_C2_RIE_MASK;
    }
}

void uart_log(uint8_t *c)
{
    uint32_t i = 0;
    while (*(c + i) != '\0')
    {
        CB_add_data(&tx_buffor, *(c + i)); // Reads the string till \0 and adds that data to the transmit buffer
        i++;
    }
    UART0->C2 |= UART0_C2_TIE_MASK; //Enable the tx interrupt when there is data in tx buffer
}
