#ifndef _led_h
#define _led_h

#include <avr/io.h>
#include <avr/interrupt.h>

#define TOP 0xFFFF // maximum resolution for >200Hz
#define T1_PRESCALE 1
#define T1_TICS_PER_OVERFLOW 0x10000
#define T1_TICS_PER_SEC (F_CPU / T1_PRESCALE)
#define T1_OVERFLOW_PER_SEC (T1_TICS_PER_SEC / T1_TICS_PER_OVERFLOW)

enum STRIPS { LEFT, RIGHT, BOTH };

void initLEDs(void);
void offLEDs(char strip);
void setLEDintensity(float intensity, char strip);
void fadeLEDs(float intensity, float duration, char strip);

#endif
