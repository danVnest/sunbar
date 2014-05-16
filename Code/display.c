#include "display.h"

static volatile uint8_t flasher = 0;
static volatile uint8_t flashState = 0;
static volatile uint8_t flashDuty = 0;
static volatile float nextTime = 0;

void initDisplay(void) {
	LEDLD |= (1<<LED0) | (1<<LED1) | (1<<LED2) | (1<<LED3) | (1<<LED4);
	LEDHD |= (1<<LED5) | (1<<LED6) | (1<<LED7) | (1<<LED8) | (1<<LED9);
	//testDisplay();
}

void offDisplay(void) {
	LEDL &= ~(1<<LED0) & ~(1<<LED1) & ~(1<<LED2) & ~(1<<LED3) & ~(1<<LED4);
	LEDH &= ~(1<<LED5) & ~(1<<LED6) & ~(1<<LED7) & ~(1<<LED8) & ~(1<<LED9);
	flashState = 0;
	flashDuty = 0;
}

uint16_t display(float interval) {
	uint8_t displayUnits = (interval / DISPLAY_TIME_RES) + 0.5;
	offDisplay();
	flasher = displayUnits / DISPLAY_FLASH_RES; 
	flashDuty = displayUnits % DISPLAY_FLASH_RES;
	switch (flasher) {
		case 10: LEDH |= (1<<LED9);
		case 9: LEDH |= (1<<LED8);
		case 8: LEDH |= (1<<LED7);
		case 7: LEDH |= (1<<LED6);
		case 6: LEDH |= (1<<LED5);
		case 5: LEDL |= (1<<LED4);
		case 4: LEDL |= (1<<LED3);
		case 3: LEDL |= (1<<LED2);
		case 2: LEDL |= (1<<LED1);
		case 1: LEDL |= (1<<LED0);
	} 
	return displayUnits * DISPLAY_TIME_RES;
}

void checkDisplay(float time) {
	if ((flashDuty != 0) && (time >= nextTime)) {
		switch (flasher) {
			case 0: { LEDLT = (1<<LED0); break; }
			case 1: { LEDLT = (1<<LED1); break; }
			case 2: { LEDLT = (1<<LED2); break; }
			case 3: { LEDLT = (1<<LED3); break; }
			case 4: { LEDLT = (1<<LED4); break; }
			case 5: { LEDHT = (1<<LED5); break; }
			case 6: { LEDHT = (1<<LED6); break; }
			case 7: { LEDHT = (1<<LED7); break; }
			case 8: { LEDHT = (1<<LED8); break; }
			case 9: { LEDHT = (1<<LED9); break; }
		}
		if (flashState ^= 1) nextTime = time + flashDuty * DISPLAY_FLASH_PERIOD;
		else nextTime = time + (DISPLAY_FLASH_RES - flashDuty) * DISPLAY_FLASH_PERIOD; 
	}
}

void testDisplay(void) {
	display(10);
	for (uint16_t i = 0; i < 0xFFFF; i++);
	offDisplay();
}
