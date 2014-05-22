#include "display.h"

static uint8_t ledsOn = 0;
static uint8_t flashState = 0;
static uint8_t nextToggle = 0;

void initDisplay(void) {
	LEDLD |= (1<<LED0) | (1<<LED1) | (1<<LED2) | (1<<LED3) | (1<<LED4);
	LEDHD |= (1<<LED5) | (1<<LED6) | (1<<LED7) | (1<<LED8) | (1<<LED9);
}

void offDisplay(void) {
	LEDL &= ~(1<<LED0) & ~(1<<LED1) & ~(1<<LED2) & ~(1<<LED3) & ~(1<<LED4);
	LEDH &= ~(1<<LED5) & ~(1<<LED6) & ~(1<<LED7) & ~(1<<LED8) & ~(1<<LED9);
}

uint16_t display(float interval) {
	uint8_t displayUnits = (interval / DISPLAY_TIME_RES) + 0.5;
	ledsOn = displayUnits / DISPLAY_FLASH_RES; 
	nextToggle = displayUnits % DISPLAY_FLASH_RES;
	if (nextToggle == 0) flashState = 0;
	if (flashState != 0) nextToggle = DISPLAY_FLASH_RES - nextToggle;
	updateDisplay();
	return displayUnits * DISPLAY_TIME_RES;
}

void updateDisplay(void) {
	offDisplay();
	switch (ledsOn + flashState) {
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
}

void checkDisplay(float time) {
	static uint8_t delta = 0;
	static float nextTime = 0;
	if ((nextToggle != 0) && (time >= nextTime)) {
		if (++delta > nextToggle) {
			updateDisplay();
			flashState ^= 1;
			delta = 0;
			nextToggle = DISPLAY_FLASH_RES - nextToggle;
		}
		nextTime = time + DISPLAY_FLASH_TIME;
	}
}
