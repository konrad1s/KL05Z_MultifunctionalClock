#include "../inc/adc.h"

void ADC_init()
{
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK; // enableADC0 clock

    // bus/2 clock source, divede 4, long sample time, 12bit conversion
    ADC0->CFG1 = ADC_CFG1_ADICLK(1) | ADC_CFG1_ADIV(0x02) | ADC_CFG1_ADLSMP_MASK | ADC_CFG1_MODE(1);

    // DMA enable
    ADC0->SC2 |= ADC_SC2_DMAEN_MASK;

    // reset SC3
    ADC0->SC3 = 0;

    // disable module
    ADC0->SC1[0] |= ADC_SC1_ADCH(31);
	
}

uint8_t ADC_calibration()
{
    uint16_t kalib_temp;
    while (ADC0->SC3 & ADC_SC3_CAL_MASK)
        ; // Czekaj na koniec kalibracji

    if (ADC0->SC3 & ADC_SC3_CALF_MASK)
    {
        ADC0->SC3 |= ADC_SC3_CALF_MASK;
        return 1; // Wróć, jeśli błąd kalibracji
    }

    kalib_temp = 0x00;
    kalib_temp += ADC0->CLP0;
    kalib_temp += ADC0->CLP1;
    kalib_temp += ADC0->CLP2;
    kalib_temp += ADC0->CLP3;
    kalib_temp += ADC0->CLP4;
    kalib_temp += ADC0->CLPS;
    kalib_temp += ADC0->CLPD;
    kalib_temp /= 2;
    kalib_temp |= 0x8000;             // Ustaw najbardziej znaczący bit na 1
    ADC0->PG = ADC_PG_PG(kalib_temp); // Zapisz w  "plus-side gain calibration register"
    ADC0->OFS = 1;                    // Klaibracja przesunięcia zera (z pomiaru swojego punktu odniesienia - masy)
    return 0;
}

uint16_t ADC_read(uint8_t channel)
{
    ADC0->SC1[0] = (channel & ADC_SC1_ADCH_MASK) | (ADC0->SC1[0] & (ADC_SC1_AIEN_MASK)); // Write to SC1A to start conversion
    while (ADC0->SC2 & ADC_SC2_ADACT_MASK)
        ; // Conversion in progress
    while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK))
        ; // wait for end
    return ADC0->R[0];
}