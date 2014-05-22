#ifndef _display_h
#define _display_h

#include <avr/io.h>

#define DISPLAY_LEDS		10
#define DISPLAY_FLASH_RES	4
#define DISPLAY_FLASH_TIME	0.25
#define DISPLAY_TIME_PER_LED	(60L * 60L)
#define DISPLAY_TIME_RES	(DISPLAY_TIME_PER_LED / DISPLAY_FLASH_RES)
#define DISPLAY_TIME_MAX	(DISPLAY_LEDS * DISPLAY_TIME_PER_LED)

#define LEDL PORTB
#define LEDLT PINF
#define LEDLD DDRB
#define LED0 PB0
#define LED1 PB1
#define LED2 PB2
#define LED3 PB3
#define LED4 PB7

#define LEDH PORTF
#define LEDHT PINF
#define LEDHD DDRF
#define LED5 PF0
#define LED6 PF1
#define LED7 PF4
#define LED8 PF5
#define LED9 PF6

void initDisplay(void);
void offDisplay(void);
uint16_t display(float interval);
void updateDisplay(void);
void checkDisplay(float time);

#endif
