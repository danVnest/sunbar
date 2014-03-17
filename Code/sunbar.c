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
	char start = seconds(); // TODO: implement overflow control
	fadeLEDs(0x01FF, 2, LEFT);
	fadeLEDs(0x01FF, 0.5, RIGHT);
	while (seconds() < start + 1);
	fadeLEDs(0, 0.5, RIGHT);
	while (seconds() < start + 2);
	fadeLEDs(0x01FF, 0.5, RIGHT);
	while (seconds() < start + 3);
	fadeLEDs(0, 2, LEFT);
	fadeLEDs(0, 0.5, RIGHT);
	while (seconds() < start + 4);
	fadeLEDs(0x01FF, 0.5, RIGHT);
	while (seconds() < start + 5);
	fadeLEDs(0, 0.5, RIGHT);
	while (seconds() < start + 6);
}
