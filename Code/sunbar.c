#include "sunbar.h"

int main(void) {
	CLKPR = (1<<CLKPCE); CLKPR = 0x00; // set to F_CPU
	initLEDs();
	initClock();
	sei();
	while (1) {
		testLEDs();
	}
}

void testLEDs(void) {
	float start = time();
	fadeLEDs(0x3FFF, 2, LEFT);
	fadeLEDs(0x3FFF, 0.5, RIGHT);
	while (time() < start + 1);
	fadeLEDs(0, 0.5, RIGHT);
	while (time() < start + 2);
	fadeLEDs(0x3FFF, 0.5, RIGHT);
	while (time() < start + 3);
	fadeLEDs(0, 2, LEFT);
	fadeLEDs(0, 0.5, RIGHT);
	while (time() < start + 4);
	fadeLEDs(0x3FFF, 0.5, RIGHT);
	while (time() < start + 5);
	fadeLEDs(0, 0.5, RIGHT);
	while (time() < start + 6);
}
