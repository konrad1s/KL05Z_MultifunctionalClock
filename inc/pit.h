#ifndef pit_h
#define pit_h

#define BUS_CLOCK SystemCoreClock / 2

extern uint8_t irqPIT;
extern uint8_t irqPIT2;

void PIT_init(void);

#endif