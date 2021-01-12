#include "MKL05Z4.h"

#ifndef adc_h
#define adc_h

void ADC_init();
uint8_t ADC_calibration();
uint16_t ADC_read(uint8_t channel);

#endif