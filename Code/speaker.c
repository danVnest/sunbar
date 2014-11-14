#include "speaker.h"

void initSpeaker(void) {
	DDRD |= (1<<PD7);
	TCCR4C = (1<<COM4D1) | (1<<PWM4D); // enable PWM
	OCR4C = T4_TOP;
	OCR4D = T4_TOP / 2; // best volume for tone
	//TIMSK4 = (1<<TOIE4); // enable overflow interrupt
}

void offSpeaker(void) {
	TCCR4B &= ~(1<<CS40) & ~(1<<CS41) & ~(1<<CS42) & ~(1<<CS43); // stop timer
}

void soundAlarm(void) {
	TCCR4B |= (1<<CS40) | (1<<CS41) | (1<<CS42); // start timer (1000Hz sound)
	//TCCR4C |= (1<<COM4D1); // enable pin
}

/*
ISR(TIMER4_OVF_vect) {
	static uint16_t overflows = 0; 
	static uint16_t pitchControlTime = T4_OVERFLOWS_PER_SEC / 1000; // 1000Hz sound
	// TODO: volume and pitch control
	if (++overflows >= pitchControlTime) {
		if ((TCCR4C >> COM4D1) & 1) TCCR4C &= ~(1<<COM4D1); // disable pin
		else TCCR4C |= (1<<COM4D1);
		overflows = 0;
		// TODO: add timer for pause in sound and end
	}
}
*/
