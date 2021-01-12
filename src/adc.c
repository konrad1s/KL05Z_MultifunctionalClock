#include "../inc/adc.h"

void ADC_init()
{
  SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK; // enable ADC0 clock

  ADC_calibration();

  // bus/2 clock source, divede 4, long sample time, 12bit conversion
  ADC0->CFG1 = ADC_CFG1_ADICLK(1) | ADC_CFG1_ADIV(0x02) | ADC_CFG1_ADLSMP_MASK | ADC_CFG1_MODE(1);

  ADC0->CFG2 = ADC_CFG2_ADHSC_MASK;

  // DMA enable
  ADC0->SC2 |= ADC_SC2_DMAEN_MASK;

  // reset SC3
  ADC0->SC3 = 0;
  // avranging 32 samples
  ADC0->SC3 = ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3);

  // disable module
  ADC0->SC1[0] |= ADC_SC1_ADCH(31);
}

uint8_t ADC_calibration()
{
  uint16_t cal_var;

  SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK; // enable ADC clock

  ADC0->SC2 &= ~ADC_SC2_ADTRG_MASK;                       // software conv trigger
  ADC0->SC3 &= (~ADC_SC3_ADCO_MASK & ~ADC_SC3_AVGS_MASK); // single conv
  ADC0->SC3 |= (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3));     // averaging = 32

  ADC0->SC3 |= ADC_SC3_CAL_MASK; // start calibration

  while (ADC0->SC3 & ADC_SC3_CAL_MASK)
    ; // wait for calibartion end

  if (ADC0->SC3 & ADC_SC3_CALF_MASK)
  {
    ADC0->SC3 |= ADC_SC3_CALF_MASK;
    return 1; // check for calibarion error
  }

  cal_var = 0x00; // calulate plus side calibration

  cal_var += ADC0->CLP0;
  cal_var += ADC0->CLP1;
  cal_var += ADC0->CLP2;
  cal_var += ADC0->CLP3;
  cal_var += ADC0->CLP4;
  cal_var += ADC0->CLPS;
  cal_var += ADC0->CLPD;
  cal_var /= 2;
  cal_var |= 0x8000; //set MSB
  ADC0->PG = ADC_PG_PG(cal_var);

  ADC0->SC3 &= ~ADC_SC3_CAL_MASK; // clear calibration bit
  return 0;
}
