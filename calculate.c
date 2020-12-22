#include "MKL05Z4.h"
#include "calculate.h"
#include "keyboard.h"
#include "frdm_bsp.h"
#include <string.h>

char expression[20] = {"\0"};
bool expression_end = 0;
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
        if (expression_end)
        {
            counter = 0;
            memset(expression, 0, 20 * sizeof(char));
            LCD1602_ClearAll();
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
            expression[--counter] = NULL;
            LCD1602_ClearAll();
            LCD1602_Print(expression);
        }
    }
    return 0x00;
}

void Calc(char *str, char *separator)
{
    expression_end = 1;
    char sm[20] = {"\0"};
    float result;

    char *ptr = strtok(str, separator);
    float a = atoi(ptr);
    ptr = strtok(NULL, '=');
    float b = atoi(ptr);

    if (separator[0] == '+')
        result = a + b;
    else if (separator[0] == '-')
        result = a - b;
    else if (separator[0] == '*')
        result = a * b;
    else if (separator[0] == '/')
        result = a / b;

    if (separator[0] == '/')
        snprintf(sm, 20, "%.2f", result);
    else
        snprintf(sm, 20, "%.0f", result);

    LCD1602_Print(sm);
}
void loop(void)
{

    sign = ReadButton(0);

    if (sign == '+' || sign == '-' || sign == '*' || sign == '/')
    {
        separator_counter++;
        if (separator_counter == 2)
        {
            ReadButton('C');
            ReadButton('=');
            separator_counter = 0;
            Calc(expression, separator);
        }
        *separator = sign;
    }
    else if (sign == '=')
    {
        separator_counter = 0;
        Calc(expression, separator);
    }
}
