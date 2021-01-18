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

char Calculator_ReadButton(void);
void Calculate(char *str, char *separator, bool sign_equal);
void Calculator_loop(void);
void Calculator_reset(void);

#endif