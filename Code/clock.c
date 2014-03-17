#include "clock.h"

volatile unsigned int overflows = 0;
volatile unsigned char _seconds = 0, _minutes = 0, _hours = 0, _days = 0;

void initClock(void) {
	// TODO: init with a time
	TCCR0B = (1<<CS02) | (1<<CS00); // 1024 prescale
	TIMSK0 = (1<<TOIE0); // enable overflow interrupt
}

unsigned char seconds(void) {
	return _seconds;
}

ISR(TIMER0_OVF_vect) {
	if (++overflows > T0_OVERFLOWS_PER_SEC) {
		overflows = 0;
		if (++_seconds >= 60) {
			_seconds = 0;
			if (++_minutes >= 60) {
				_minutes = 0;
				if (++_hours >= 24) {
					_hours = 0;
					if (++_days >= 7) _days = 0;
				}
			}
		}
	}
}
