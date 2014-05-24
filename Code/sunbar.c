#include "sunbar.h"

int main(void) {
	CLKPR = (1<<CLKPCE); CLKPR = 0x00; // set to F_CPU
	float leftAlarmTime = DEFAULT_ALARM_TIME;
	float rightAlarmTime = leftAlarmTime;
	float leftRiseTime = leftAlarmTime - RISE_DURATION - ALARM_DELAY;
	float rightRiseTime = leftRiseTime;
	float displayUpdateTime = 0;
	float controlTimeout = 0;
	uint8_t leftState = WAITING;
	uint8_t rightState = WAITING;
	uint8_t leftAlarmToggle = 1;
	uint8_t rightAlarmToggle = 1;
	initLEDs();
	initDisplay();
	initClock();
	DDRD |= (1<<PD6); 
	PORTD |= (1<<INT0) | (1<<INT1);
	sei();
	testLEDs();
	while (1) {
		float currentTime = time();
		// Control Timeout - Update Settings 
		if ((leftState == CONTROLLING) || (rightState == CONTROLLING)) {
			if (currentTime >= controlTimeout) {
				if (leftState == CONTROLLING) {
					leftRiseTime = leftAlarmTime - RISE_DURATION - ALARM_DELAY;
					if (currentTime < leftRiseTime) leftState = WAITING;
					else {
						leftRiseTime += ONE_DAY;
						leftState = RISING;
					}
					if (rightState == CONTROLLING) rightAlarmTime = leftAlarmTime;
					fadeLEDs(0, CONTROL_FADE, LEFT);
				}
				if (rightState == CONTROLLING) {
					rightRiseTime = rightAlarmTime - RISE_DURATION - ALARM_DELAY;
					if (currentTime < rightRiseTime) rightState = WAITING;
					else {
						rightRiseTime += ONE_DAY;
						rightState = RISING;
					}
					fadeLEDs(0, CONTROL_FADE, RIGHT);
				}
				display(0);
				displayUpdateTime = currentTime;
			}
			else checkDisplay(currentTime);
		}
		// Button Control 
		uint8_t buttons = checkButtons();
		if (buttons != NONE) {
			if (buttons == BOTH) {
				if (rightState == CONTROLLING) leftAlarmTime = display(rightAlarmTime - currentTime) + currentTime;
				else leftAlarmTime = display(leftAlarmTime - currentTime) + currentTime;
				fadeLEDs(CONTROL_INTENSITY, CONTROL_FADE, BOTH);
				leftState = CONTROLLING;
				rightState = CONTROLLING;
				controlTimeout = currentTime + CONTROL_DURATION;
			}
			else if (leftState == CONTROLLING) {
				if (buttons == LEFT) { if (leftAlarmTime - 1.5*DISPLAY_TIME_RES > currentTime) leftAlarmTime -= DISPLAY_TIME_RES; }
				else if (leftAlarmTime + DISPLAY_TIME_RES <= currentTime + DISPLAY_TIME_MAX) leftAlarmTime += DISPLAY_TIME_RES;
				leftAlarmTime = display(leftAlarmTime - currentTime) + currentTime;
				controlTimeout = currentTime + CONTROL_DURATION;
			}
			else if (rightState == CONTROLLING) {
				if (buttons == LEFT) { if (rightAlarmTime - 1.5*DISPLAY_TIME_RES > currentTime) rightAlarmTime -= DISPLAY_TIME_RES; }
				else if (rightAlarmTime + DISPLAY_TIME_RES <= currentTime + DISPLAY_TIME_MAX) rightAlarmTime += DISPLAY_TIME_RES;
				rightAlarmTime = display(rightAlarmTime - currentTime) + currentTime;
				controlTimeout = currentTime + CONTROL_DURATION;
			}
			else if (buttons == LEFT) {
				if (leftState == RISING) {
					fadeLEDs(0, WAKE_FADE, LEFT);
					leftRiseTime += ONE_DAY;
					leftAlarmTime += ONE_DAY;
					leftState = WAITING;
					displayUpdateTime = currentTime;
				}
				else { // leftState == WAITING
					// TODO: detect ambient light
					if (leftAlarmTime > currentTime + DISPLAY_TIME_MAX) leftAlarmTime = currentTime + DEFAULT_ALARM_TIME;
					display(leftAlarmTime - currentTime);
					fadeLEDs(CONTROL_INTENSITY, CONTROL_FADE, LEFT);
					leftState = CONTROLLING;
					controlTimeout = currentTime + CONTROL_DURATION;
				}
			}
			else { // buttons == RIGHT
				if (rightState == RISING) {
					fadeLEDs(0, WAKE_FADE, RIGHT);
					rightRiseTime += ONE_DAY;
					rightAlarmTime += ONE_DAY;
					rightState = WAITING; // TODO: or snooze?
					displayUpdateTime = currentTime;
				}
				else { // rightState == WAITING
					if (rightAlarmTime > currentTime + DISPLAY_TIME_MAX) rightAlarmTime = currentTime + DEFAULT_ALARM_TIME;
					display(rightAlarmTime - currentTime);
					fadeLEDs(CONTROL_INTENSITY, CONTROL_FADE, RIGHT);
					rightState = CONTROLLING;
					controlTimeout = currentTime + CONTROL_DURATION;
				}
			}
		}
		// Rise and Alarm Triggers
		if ((leftState == WAITING) && (currentTime >= leftRiseTime)) {
			fadeLEDs(RISE_INTENSITY, RISE_DURATION, LEFT);
			leftRiseTime += ONE_DAY;
			leftState = RISING;
		}
		else if (leftState == RISING) {
			if (currentTime >= leftAlarmTime) {
				if ((rightAlarmToggle ^= 1)) setLEDintensity(ALARM_INTENSITY, LEFT);
				else offLEDs(LEFT);
				leftAlarmTime += 1;
				// TODO: maybe random toggling would be more effective?
			}
			else if ((currentTime >= displayUpdateTime) && (leftAlarmTime <= rightAlarmTime)) {
				display((leftAlarmTime - currentTime) * DISPLAY_RISE_ZOOM + DISPLAY_TIME_RES);
				displayUpdateTime = currentTime + DISPLAY_TIME_PER_LED / DISPLAY_RISE_ZOOM;
			}
		}
		if ((rightState == WAITING) && (currentTime >= rightRiseTime)) {
			fadeLEDs(RISE_INTENSITY, RISE_DURATION, RIGHT);
			rightRiseTime += ONE_DAY;
			rightState = RISING;
		}
		else if (rightState == RISING) {
			if (currentTime >= rightAlarmTime) {
				if ((leftAlarmToggle ^= 1)) setLEDintensity(ALARM_INTENSITY, RIGHT);
				else offLEDs(RIGHT);
				rightAlarmTime += 1;
			}
			else if (currentTime >= displayUpdateTime) {
				display((rightAlarmTime - currentTime) * DISPLAY_RISE_ZOOM + DISPLAY_TIME_RES);
				displayUpdateTime = currentTime + DISPLAY_TIME_PER_LED / DISPLAY_RISE_ZOOM;
			}
		}
	}
}

uint8_t checkButtons(void) {
	static uint16_t leftFilter = 0, rightFilter = 0;
	static uint8_t buttons[2] = {0};
	static float holdTime = 0;
	uint8_t edges = NONE;
	leftFilter = (leftFilter << 1) | ((PIND >> INT0) & 1);
	rightFilter = (rightFilter << 1) | ((PIND >> INT1) & 1);
	if ((leftFilter | 0xE000) == 0xF000) {
		edges = LEFT;
		buttons[LEFT] = 1;
	}
	else if ((leftFilter & 0x1FFF) == 0x0FFF) {
		TOGGLE_U_LED;
		buttons[LEFT] = 0;
	}
	if ((rightFilter | 0xE000) == 0xF000) {
		edges = RIGHT;
		buttons[RIGHT] = 1;
	}
	else if ((rightFilter & 0x1FFF) == 0x0FFF) {
		TOGGLE_U_LED;
		buttons[RIGHT] = 0;
	}
	if (edges != NONE) {
		TOGGLE_U_LED;
		holdTime = time() + HOLD_INTERVAL;
		if ((buttons[LEFT] & buttons[RIGHT]) != 0) return BOTH;
	}
	else if ((buttons[LEFT] | buttons[RIGHT]) != 0) {
		if (time() >= holdTime) {
			if (buttons[LEFT] != 0) edges = LEFT;
			else edges = RIGHT;
			holdTime = time() + HOLD_INTERVAL;
		}
	}
	return edges;
}

void testLEDs(void) {
	float start = time();
	display(1L*60*60);
	fadeLEDs(TEST_INTENSITY, 2, LEFT);
	fadeLEDs(TEST_INTENSITY, 0.5, RIGHT);
	while (time() < start + 1);
	display(2L*60*60);
	fadeLEDs(0, 0.5, RIGHT);
	while (time() < start + 2);
	display(3L*60*60);
	fadeLEDs(TEST_INTENSITY, 0.5, RIGHT);
	while (time() < start + 3);
	display(5L*60*60);
	fadeLEDs(0, 2, LEFT);
	fadeLEDs(0, 0.5, RIGHT);
	while (time() < start + 4);
	display(7L*60*60);
	fadeLEDs(TEST_INTENSITY, 0.5, RIGHT);
	while (time() < start + 5);
	display(10L*60*60);
	fadeLEDs(0, 0.5, RIGHT);
	while (time() < start + 6);
	display(0);
}
