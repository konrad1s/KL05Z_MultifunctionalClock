#include "MKL05Z4.h"
#include "calculate.h"
#include "keyboard.h"
#include "frdm_bsp.h"
#include <string.h>
#include <math.h>

char expression[20] = {"\0"};
uint8_t expression_end = 0;
char sign = 0x00;
char separator[2] = "\0";
uint8_t separator_counter = 0;

char ReadButton(char option)
{
    static uint8_t counter = 0;
    char button;

    if (option == 0)
        button = KB_read();
    else
        button = option;

    char button_print[2] = {"\0"};
    if (button != 0)
    {
        if (expression_end == 1)
        {
            counter = 0;
            memset(expression, 0, 20 * sizeof(char));
            LCD1602_ClearAll();
            expression_end = 0;
        }

        if (expression_end == 2)
        {
            counter = strlen(expression);
            expression_end = 0;
        }

        if (button != 'C')
        {
            expression[counter] = button;
            counter++;
            button_print[0] = button;
            LCD1602_Print(button_print);
            return button;
        }
        else
        {
            if (counter > 0)
            {
                expression[--counter] = NULL;
                LCD1602_ClearAll();
                LCD1602_Print(expression);
            }
        }
    }
    return 0x00;
}

void Calc(char *str, char *separator, bool sign_equal)
{
    expression_end = 1;
    char buffor[20] = {"\0"};

    float result;

   // char *ptr = strtok(str, separator);
   // float a = atof(ptr);
		char* end;
		float a = strtod( str, & end );
   // ptr = strtok(NULL, '=');
  //  float b = atof(ptr);
		end++;
float b = strtod( end, NULL );
    if (separator[0] == '+')
        result = a + b;
    else if (separator[0] == '-')
        result = a - b;
    else if (separator[0] == '*')
        result = a * b;
    else if (separator[0] == '/')
        result = a / b;

  //  if (separator[0] == '/')
		if(result-floor(result)!=0)
        snprintf(buffor, 20, "%.2f", result);
    else
        snprintf(buffor, 20, "%.0f", result);

    if (!sign_equal)
    {
			if(result-floor(result)!=0)
        snprintf(expression, 20, "%.2f%c\0", result, separator[1]);
			else
				snprintf(expression, 20, "%.0f%c\0", result, separator[1]);
			
        LCD1602_ClearAll();
        LCD1602_Print(expression);

        expression_end = 2;
    }
    else
        LCD1602_Print(buffor);
}


void loop(void)
{
    sign = ReadButton(0);

    if (sign == '+' || sign == '-' || sign == '*' || sign == '/')
    {
        separator_counter++;
        if (separator_counter == 2)
        {
            separator[1] = sign;
            separator_counter = 1;
            Calc(expression, separator, 0);
        }
        separator[0] = sign;
    }
    else if (sign == '=')
    {
        separator_counter = 0;
        Calc(expression, separator, 1);
    }
}
