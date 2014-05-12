#include "sunbar.h"

int main(void) {
	CLKPR = (1<<CLKPCE); CLKPR = 0x00; // set to F_CPU
	initLEDs();
	initClock();
	sei();
	while (1) {
		testLEDs();
		float riseTime = time() + 7*60*60;
		uint16_t riseDuration = 30*60;
		while (time() < riseTime);
		fadeLEDs(0xFFFF, riseDuration, RIGHT);
		while (time() < riseTime + riseDuration + 30*60);
		while (1) testLEDs();
		while (time() < riseTime + 24*60*60 - riseDuration - 30*60);
	}
}

void testLEDs(void) {
	float start = time();
	fadeLEDs(0xFFFF, 2, LEFT);
	fadeLEDs(0xFFFF, 0.5, RIGHT);
	while (time() < start + 1);
	fadeLEDs(0, 0.5, RIGHT);
	while (time() < start + 2);
	fadeLEDs(0xFFFF, 0.5, RIGHT);
	while (time() < start + 3);
	fadeLEDs(0, 2, LEFT);
	fadeLEDs(0, 0.5, RIGHT);
	while (time() < start + 4);
	fadeLEDs(0xFFFF, 0.5, RIGHT);
	while (time() < start + 5);
	fadeLEDs(0, 0.5, RIGHT);
	while (time() < start + 6);
}
