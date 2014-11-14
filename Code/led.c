#include "led.h"

static volatile float leftStep = 0, rightStep = 0, leftIntensity = 0, rightIntensity = 0, leftIntensityFinal = 0, rightIntensityFinal = 0;
static volatile uint8_t leftStepIsPositive, rightStepIsPositive;

void initLEDs(void) {
	// PWM SETUP
	// uses timer 1 for pwm output on pins B5 and B6 (OCR1A and OCR1B)
	// COM1x1 set for non-inverting PWM mode
	// WGM11-3 set for fast PWM mode (max resolution and frequency) with TOP set in ICR1
	// fast PWM mode cannot have 0% duty, disconnect OCR1x and drive Bx low instead 
	// CS10 set for no prescale
	DDRB |= (1<<PB5) | (1<<PB6);
	TCCR1A = (1<<WGM11); 
	TCCR1B = (1<<WGM12) | (1<<WGM13) | (1<<CS10);
	ICR1 = T1_TOP; 
	TIMSK1 = (1<<TOIE1);
}

void offLEDs(uint8_t strip) {
	switch (strip) {
		case LEFT: {
				   TCCR1A &= ~(1<<COM1B1);
				   leftIntensity = 0;
				   break;
			   }
		case RIGHT: {
				    TCCR1A &= ~(1<<COM1A1);
				    rightIntensity = 0;
				    break;
			    }
		case BOTH: {
				   TCCR1A &= ~((1<<COM1A1) | (1<<COM1B1));
				   leftIntensity = 0;
				   rightIntensity = 0;
				   break;
			   }
		// default: break; // TODO: error
	}
}

void setLEDintensity(float intensity, uint8_t strip) {
	if (intensity == 0) { offLEDs(strip); return; }
#define B 0xFFFF
	// TODO: use lookup table (at least partially and use linear interpolation), or reduce computation expense another way
	float lIntensity = 0xFFFF / (B - 1) * (powf(B, intensity/0xFFFF) - 1);
	switch (strip) {
		case BOTH: {
				   TCCR1A |= (1<<COM1A1);
				   OCR1A = lIntensity;
				   rightIntensity = intensity;
			   }
		case LEFT: {
				   TCCR1A |= (1<<COM1B1);
				   OCR1B = lIntensity;
				   leftIntensity = intensity;
				   break;
			   }
		case RIGHT: {
				    TCCR1A |= (1<<COM1A1);
				    OCR1A = lIntensity;
				    rightIntensity = intensity;
				    break;
			    }
		// default: break; // TODO: error
	}
}

void fadeLEDs(float intensity, float duration, uint8_t strip) {
	// if (duration > MAX_FADE_DURATION) return; // TODO: error;
	float overflows = duration * T1_OVERFLOW_PER_SEC;
	if (overflows < 1) { setLEDintensity(intensity, strip); return; }
	switch (strip) {
		case BOTH: {
				   rightStep = (intensity - rightIntensity) / overflows;
				   if (rightStep > 0) rightStepIsPositive = 1;
				   else rightStepIsPositive = 0;
				   rightIntensityFinal = intensity;
			   }
		case LEFT: {
				   leftStep = (intensity - leftIntensity) / overflows;
				   if (leftStep > 0) leftStepIsPositive = 1;
				   else leftStepIsPositive = 0;
				   leftIntensityFinal = intensity;
				   break;
			   }
		case RIGHT: {
				    rightStep = (intensity - rightIntensity) / overflows;
				    if (rightStep > 0) rightStepIsPositive = 1;
				    else rightStepIsPositive = 0;
				    rightIntensityFinal = intensity;
				    break;
			   }
		// default: break; // TODO: error
	}
}	

ISR(TIMER1_OVF_vect) {
	// TODO: reduce complexity in interrupt
	if (leftStep != 0) {
		float leftIntensityNew = leftIntensity + leftStep;
		uint8_t leftIntensityIsHigh = (leftIntensityNew >= leftIntensityFinal);
		if (!(leftStepIsPositive ^ leftIntensityIsHigh)) {
			leftStep = 0;
			leftIntensityNew = leftIntensityFinal;
		}
		setLEDintensity(leftIntensityNew, LEFT);
	}
	if (rightStep != 0) {
		float rightIntensityNew = rightIntensity + rightStep;
		uint8_t rightIntensityIsHigh = (rightIntensityNew >= rightIntensityFinal);
		if (!(rightStepIsPositive ^ rightIntensityIsHigh)) {
			rightStep = 0;
			rightIntensityNew = rightIntensityFinal;
		}
		setLEDintensity(rightIntensityNew, RIGHT);
	}
}
