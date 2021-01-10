#include "MKL05Z4.h"
#include "keyboard.h"

char KB_map[4][4] = {
    {'1', '2', '3', '+'},
    {'4', '5', '6', '-'},
    {'7', '8', '9', '*'},
    {'C', '0', '=', '/'}};

char KB_rows[4] = {KB_ROW1, KB_ROW2, KB_ROW3, KB_ROW4};
char KB_columnes[4] = {KB_COLUMNE1, KB_COLUMNE2, KB_COLUMNE3, KB_COLUMNE4};

///////////////////////////////////////////////////////////
// Initialize keyboard
///////////////////////////////////////////////////////////
void KB_init(void)
{
    if (KB_PORT == PORTA)
        SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; // turn on clock on port A
    else if (KB_PORT == PORTB)
        SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; // turn on clock on port B

    // set pins mux as gpio
    KB_PORT->PCR[KB_COLUMNE4] |= PORT_PCR_MUX(1);
    KB_PORT->PCR[KB_COLUMNE3] |= PORT_PCR_MUX(1);
    KB_PORT->PCR[KB_COLUMNE2] |= PORT_PCR_MUX(1);
    KB_PORT->PCR[KB_COLUMNE1] |= PORT_PCR_MUX(1);

    KB_PORT->PCR[KB_ROW4] |= PORT_PCR_MUX(1);
    KB_PORT->PCR[KB_ROW3] |= PORT_PCR_MUX(1);
    KB_PORT->PCR[KB_ROW2] |= PORT_PCR_MUX(1);
    KB_PORT->PCR[KB_ROW1] |= PORT_PCR_MUX(1);

    // set rows pins as output
    if (KB_PORT == PORTA)
    {
        PTA->PDDR = (1 << KB_ROW4);
        PTA->PDDR |= (1 << KB_ROW3);
        PTA->PDDR |= (1 << KB_ROW2);
        PTA->PDDR |= (1 << KB_ROW1);
    }
    else if (KB_PORT == PORTB)
    {
        PTB->PDDR |= (1 << KB_ROW4);
        PTB->PDDR |= (1 << KB_ROW3);
        PTB->PDDR |= (1 << KB_ROW2);
        PTB->PDDR |= (1 << KB_ROW1);
    }

    // enable pull-up resistor on columns
    KB_PORT->PCR[KB_COLUMNE4] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
    KB_PORT->PCR[KB_COLUMNE3] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
    KB_PORT->PCR[KB_COLUMNE2] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
    KB_PORT->PCR[KB_COLUMNE1] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
}

///////////////////////////////////////////////////////////
// Function read pressed button
// returns char from KB_map[][], if button is not pressed return 0
///////////////////////////////////////////////////////////
char KB_read(void)
{
    char result = 0x00;

    for (uint8_t i = 0; i < 4; i++)
    {
        if (KB_PORT == PORTA)
            PTA->PDOR &= ~(1 << KB_rows[i]); // set input row as pull-down
        else if (KB_PORT == PORTB)
            PTB->PDOR &= ~(1 << KB_rows[i]); // set input row as pull-down

        for (uint8_t j = 0; j < 4; j++)
        {
            if (KB_PORT == PORTA)
            {
                if ((PTA->PDIR & (1 << KB_columnes[j])) == 0) // check if button is pressed
                    result = KB_map[i][j];
            }
            else if (KB_PORT == PORTB)
            {
                if ((PTB->PDIR & (1 << KB_columnes[j])) == 0) // check if button is pressed
                    result = KB_map[i][j];
            }
        }
        if (KB_PORT == PORTA)
            PTA->PDOR |= (1 << KB_rows[i]); // set row as pull-up
        else if (KB_PORT == PORTB)
            PTB->PDOR |= (1 << KB_rows[i]); // set row as pull-up
    }
    return result;
}
