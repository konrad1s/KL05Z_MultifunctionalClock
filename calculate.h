#ifndef calculate_h
#define calculate_h

typedef enum
{
    false,
    true
} bool;

char ReadButton(char option);
void Calc(char *str, char *separator);
void loop(void);

#endif