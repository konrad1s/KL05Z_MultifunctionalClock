#ifndef calculate_h
#define calculate_h

typedef enum
{
  false,
  true
} bool;

char ReadButton(void);
void Calc(char *str, char *separator, bool sign_equal);
void loop(void);

#endif