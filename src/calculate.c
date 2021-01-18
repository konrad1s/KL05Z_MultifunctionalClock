/**
 * @file calculate.c
 * @author Konrad Sikora
 * @date Jan 2021
 */

#include "MKL05Z4.h"
#include "../inc/calculate.h"
#include "../inc/keyboard.h"
#include "../inc/frdm_bsp.h"
#include <string.h>
#include <math.h>

// tab of char where the entire expression is stored
char expression[30] = {"\0"};

///////////////////////////////////////////////////////////
// expression_check == 0 => do nothing
// expression_check == 1 => there is = , clear expression tab, clear LCD
// expression_check == 2 => there is more than one sign(+,-,*,/), print result on LCD, save result into expression tab
///////////////////////////////////////////////////////////
uint8_t expression_check = 0;

///////////////////////////////////////////////////////////
// Function add numbers to expression[] and print on LCD pressed button
// return pressed button
///////////////////////////////////////////////////////////
uint8_t counter = 0;
uint8_t separator_counter = 0;

char ReadButton(void)
{

    char button;

    button = KB_read();

    char button_print[2] = {"\0"};
    if (button != 0)
    {
        if (expression_check == 1) //if there was "=", clear LCD and expression[]
        {
            counter = 0;
            memset(expression, 0, 20 * sizeof(char));
            LCD1602_ClearRow(0);
            expression_check = 0;
        }

        if (expression_check == 2)
        {
            counter = strlen(expression); //set new value of counter if there was more than one sign
            expression_check = 0;
        }

        if (button != 'C')
        {
            expression[counter] = button;
            button_print[0] = button;
            LCD1602_PrintXY(button_print, counter, 0); //print one number or sign
            counter++;
            return button;
        }
        else // button == "C"
        {
            if (counter > 0)
            {
                expression[--counter] = NULL; // clear number in expression tab
                LCD1602_ClearRow(0);
                LCD1602_PrintXY(expression, 0, 0); // print new expression
            }
        }
    }
    return 0x00;
}

///////////////////////////////////////////////////////////
// Function that calculates the result
///////////////////////////////////////////////////////////
void Calc(char *str, char *separator, bool sign_equal)
{
    expression_check = 1;
    char buffor[20] = {"\0"};
    char endl[] = {"\r\n"};

    float result;

    char *end;
    float a = strtod(str, &end); //convert first number to float
    end++;                       //sign between two numbers
    float b = strtod(end, NULL); //convert second number to float

    //calculate te result
    if (separator[0] == '+')
        result = a + b;
    else if (separator[0] == '-')
        result = a - b;
    else if (separator[0] == '*')
        result = a * b;
    else if (separator[0] == '/')
        result = a / b;

    int lcd_position = strlen(expression);

    //display the result depending on the type of number
    if (result - floor(result) != 0)
        snprintf(buffor, 20, "%.2f", result);
    else
        snprintf(buffor, 20, "%.0f", result);

    if (!sign_equal)
    {
        if (result - floor(result) != 0)
            snprintf(expression, 20, "%.2f%c\0", result, separator[1]);
        else
            snprintf(expression, 20, "%.0f%c\0", result, separator[1]);

        LCD1602_ClearRow(0);
        LCD1602_PrintXY(expression, 0, 0);

        expression_check = 2;
    }
    else
    {
        LCD1602_PrintXY(buffor, lcd_position, 0);
        // if there was '=' send data to uart
        uart_send((uint8_t *)expression);
        uart_send((uint8_t *)buffor);
        uart_send((uint8_t *)endl);
    }
}

///////////////////////////////////////////////////////////
// loop function
// checks if there was "=" sign or more than one sign(+,-,*,/)
///////////////////////////////////////////////////////////
void CalculatorLoop(void)
{
    static char separator[2] = "\0";

    char sign = ReadButton();

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

void CalculatorReset(void)
{
    memset(expression, 0, 20 * sizeof(char));
    counter = 0;
    separator_counter = 0;
}
