#ifndef led_h
#define led_h

#define BUZZER 13

typedef enum
{
    RED_LED = 8,
    GREEN_LED = 9,
    BLUE_LED = 10
} LED_Color;

void LEDs_init();
void LEDs_off();
void LED_on(LED_Color color);
void LED_toggle(LED_Color color);

void buzzer_on(void);
void buzzer_off(void);

#endif