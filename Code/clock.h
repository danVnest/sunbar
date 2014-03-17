#ifndef _clock_h
#define _clock_h

#include <avr/io.h>
#include <avr/interrupt.h>

#define T0_PRESCALE 1024
#define T0_TICS_PER_OVERFLOW 0x100 
#define T0_TICS_PER_SEC (F_CPU / T0_PRESCALE)
#define T0_OVERFLOWS_PER_SEC (T0_TICS_PER_SEC / T0_TICS_PER_OVERFLOW)

void initClock(void);
unsigned char seconds(void);

#endif
