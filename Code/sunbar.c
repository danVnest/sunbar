#include "sunbar.h"

int main(void) {
	initLEDs();
	setLEDintensity(0x09ff, LEFT); // max of 0xFFFF
	setLEDintensity(0x00ff, RIGHT); // max of 0xFFFF
	while (1) {
		//
	}
}

/* Test
Roll through each phase for 5 seconds
- fade in
- stay on (2s)
- fade out
- off (2s)
- blink 5 times (fast in-on-out-off)
*/
void testLEDs() {
}
