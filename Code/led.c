#include "led.h"

void initLEDs(void) {
	// PWM SETUP
	// uses timer 1 for pwm output on pins B5 and B6 (OCR1A and OCR1B)
	// COM1x1 set for non-inverting PWM mode
	// WGM11-3 set for fast PWM mode (max resolution and frequency) with TOP set in ICR1
	// fast PWM mode cannot have 0% duty, disconnect OCR1x and drive Bx low instead 
	// CS10 set for no prescale
	TCCR1A |= _BV(WGM11); 
	TCCR1B |= _BV(WGM12) | _BV(WGM13) | _BV(CS10);
	DDRB |= _BV(PB5) | _BV(PB6);
	ICR1 = TOP; 
	TCNT1 = 0;
}
void onLEDs(char strip) {
	switch (strip) {
		case LEFT: { TCCR1A |= _BV(COM1A1); break;}
		case RIGHT: { TCCR1A |= _BV(COM1B1); break; }
		case BOTH: { TCCR1A |= _BV(COM1A1) | _BV(COM1B1); break; }
		// default: break; // TODO: error
	}
}

void offLEDs(char strip) {
	switch (strip) {
		case LEFT: { TCCR1A &= ~_BV(COM1A1); break;}
		case RIGHT: { TCCR1A &= ~_BV(COM1B1); break; }
		case BOTH: { TCCR1A &= ~(_BV(COM1A1) | _BV(COM1B1)); break; }
		// default: break; // TODO: error
	}
}

void setLEDintensity(unsigned int intensity, char strip) {
	// TODO: find out if int is 16bit
	// TODO: check intensity is appropriate value
	if (intensity-- == 0) { offLEDs(strip); return; } // -- as 0 is lowest intensity
	onLEDs(strip);
	switch (strip) {
		case BOTH: OCR1B = intensity;
		case LEFT: { OCR1A = intensity; break; }
		case RIGHT: { OCR1B = intensity; break; }
		// default: break; // TODO: error
	}
}

void fadeLEDs(unsigned int intensity, unsigned int duration, char strip) {
	// TODO: perhaps not use setLEDintensity, but implement in interrupts
}	

