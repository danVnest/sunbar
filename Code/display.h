#ifndef _display_h
#define _display_h

#include <avr/io.h>

#define DISPLAY_LEDS		10
#define DISPLAY_FLASH_RES	4
#define DISPLAY_FLASH_TIME	0.125
#define DISPLAY_TIME_PER_LED	(60L * 60L)
#define DISPLAY_TIME_RES	(DISPLAY_TIME_PER_LED / DISPLAY_FLASH_RES)
#define DISPLAY_TIME_MAX	(DISPLAY_LEDS * DISPLAY_TIME_PER_LED)

#define LEDL PORTF
#define LEDLT PINF
#define LEDLD DDRF
#define LED0 PF0
#define LED1 PF1
#define LED2 PF4
#define LED3 PF5
#define LED4 PF6

#define LEDH PORTB
#define LEDHT PINF
#define LEDHD DDRB
#define LED5 PB0
#define LED6 PB1
#define LED7 PB2
#define LED8 PB3
#define LED9 PB7


void initDisplay(void);
void offDisplay(void);
uint16_t display(float interval);
void updateDisplay(void);
void checkDisplay(float time);

#endif
