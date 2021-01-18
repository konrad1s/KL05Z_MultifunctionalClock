/**
 * @file calculate.h
 * @author Konrad Sikora
 * @date Jan 2021
 */

#ifndef calculate_h
#define calculate_h

typedef enum
{
  false,
  true
} bool;

char ReadButton(void);
void Calc(char *str, char *separator, bool sign_equal);
void CalculatorLoop(void);
void CalculatorReset(void);

#endif