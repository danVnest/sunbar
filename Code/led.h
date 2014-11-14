#ifndef _led_h
#define _led_h

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#define T1_TOP 0xFFFF // maximum resolution for >200Hz
#define T1_PRESCALE 1
#define T1_TICS_PER_OVERFLOW 0x10000
#define T1_TICS_PER_SEC ((float)F_CPU / T1_PRESCALE)
#define T1_OVERFLOW_PER_SEC ((float)T1_TICS_PER_SEC / T1_TICS_PER_OVERFLOW)

enum SIDES { LEFT, RIGHT, BOTH, NONE };

void initLEDs(void);
void offLEDs(uint8_t strip);
void setLEDintensity(float intensity, uint8_t strip);
void fadeLEDs(float intensity, float duration, uint8_t strip);

#endif
