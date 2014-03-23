#ifndef _clock_h
#define _clock_h

#include <avr/io.h>
#include <avr/interrupt.h>

#define T0_PRESCALE 1024
#define T0_TICS_PER_OVERFLOW 0x100 
#define T0_TICS_PER_SEC ((float)F_CPU / T0_PRESCALE)
#define T0_SECS_PER_OVERFLOW ((float)T0_TICS_PER_OVERFLOW / T0_TICS_PER_SEC)

void initClock(void);
float time(void);

#endif
