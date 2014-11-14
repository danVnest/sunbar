#ifndef _speaker_h
#define _speaker_h

#include <avr/io.h>
#include <avr/interrupt.h>

#define T4_TOP 0xFF // TODO: 10 bits are available (11 with ENHC)
#define PITCH_1K (
#define T4_PRESCALE 1
#define T4_TICS_PER_OVERFLOW 0x100 
#define T4_TICS_PER_SEC (F_CPU / T4_PRESCALE)
#define T4_SECS_PER_OVERFLOW (T4_TICS_PER_OVERFLOW / T4_TICS_PER_SEC)
#define T4_OVERFLOWS_PER_SEC (T4_TICS_PER_SEC / T4_TICS_PER_OVERFLOW)

void initSpeaker(void);
void offSpeaker(void);
void soundAlarm(void);

#endif
