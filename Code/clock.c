#include "clock.h"

static volatile float seconds = 0;

void initClock(void) {
	TCCR0B = (1<<CS02) | (1<<CS00); // 1024 prescale
	TIMSK0 = (1<<TOIE0); // enable overflow interrupt
}

float time(void) {
	return seconds;
}

ISR(TIMER0_OVF_vect) {
	seconds += T0_SECS_PER_OVERFLOW;
}
