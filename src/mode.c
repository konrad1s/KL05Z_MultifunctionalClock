#include "MKL05Z4.h"
#include "../inc/mode.h"
#include "../inc/pit.h"
#include "../inc/dma.h"
#include "../inc/lcd1602.h"

uint8_t mode = 0;
uint8_t prev_mode = 0;

void chooseMode()
{
  switch (mode)
  {
  case 0:
    calculatorMode();
    break;
  case 1:
    temperatureMode();
    break;
  }
  if (prev_mode != mode)
  {
    LCD1602_ClearRow(0);
		CalculatorReset();
    prev_mode = mode;
  }
}

void calculatorMode()
{
  if (irqPIT)
  {
    CalculatorLoop();
    irqPIT = 0;
  }
}
void temperatureMode()
{
  char buff[20] = "\0";
  float adc_volt_coeff = ((float)(((float)2.91) / 4095));
  float result = 0;
  float Ut25 = 0.716;
  float m = 0.00162;

  if (irqPIT2)
  {
    result = 25.0 - (((DMAvalue[0] & 0xFFFF) * adc_volt_coeff) - Ut25) / m;
    snprintf(buff, 20, "T=%0.1f%cC   ", result, 0xDF);
    LCD1602_PrintXY(buff, 0, 0);
    irqPIT2 = 0;
  }
}