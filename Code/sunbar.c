#include "sunbar.h"

int main(void) {
	CLKPR = (1<<CLKPCE); CLKPR = 0x00; // set to F_CPU
	float leftAlarmTime = 7.5*60*60;
	float rightAlarmTime = leftAlarmTime;
	float leftRiseTime = leftAlarmTime - RISE_DURATION - ALARM_DELAY;
	float rightRiseTime = leftRiseTime;
	float controlTimeout = 0;
	uint8_t leftState = WAITING;
	uint8_t rightState = WAITING;
	uint8_t alarmToggle = 1;
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
				display(0);
				if (leftState == CONTROLLING) {
					leftRiseTime = leftAlarmTime - RISE_DURATION - ALARM_DELAY;
					if (currentTime < leftRiseTime) leftState = WAITING;
					else {
						leftState = RISING;
						leftRiseTime += ONE_DAY;
					}
					fadeLEDs(0, CONTROL_FADE, LEFT);
				}
				if (rightState == CONTROLLING) {
					if (leftState == CONTROLLING) rightRiseTime = leftRiseTime;
					else rightRiseTime = rightAlarmTime - RISE_DURATION - ALARM_DELAY;
					if (currentTime < rightRiseTime) rightState = WAITING;
					else {
						rightState = RISING;
						rightRiseTime += ONE_DAY;
					}
					fadeLEDs(0, CONTROL_FADE, RIGHT);
				}
			}
			else checkDisplay(currentTime);
		}
		// Button Control 
		// TODO: change all chars to uint8_t
		// TODO: make a bool_t type
		uint8_t buttons = checkButtons();
		if (buttons != NONE) {
			if (buttons == BOTH) {
				leftAlarmTime = display(leftAlarmTime - currentTime);
				rightAlarmTime = leftAlarmTime;
				fadeLEDs(CONTROL_INTENSITY, CONTROL_FADE, BOTH);
				controlTimeout = currentTime + CONTROL_DURATION;
				leftState = CONTROLLING;
				rightState = CONTROLLING;
			}
			else if (leftState == CONTROLLING || rightState == CONTROLLING) {
				if (leftState == CONTROLLING) {
					if (buttons == LEFT) {
						if (leftAlarmTime - DISPLAY_TIME_RES > currentTime) leftAlarmTime -= DISPLAY_TIME_RES;
					}
					else {
						if (leftAlarmTime + DISPLAY_TIME_RES <= currentTime + DISPLAY_TIME_MAX) leftAlarmTime += DISPLAY_TIME_RES;
					}
					leftAlarmTime = display(leftAlarmTime - currentTime);
				}
				else {
					if (buttons == LEFT) {
						if (rightAlarmTime - DISPLAY_TIME_RES > currentTime) rightAlarmTime -= DISPLAY_TIME_RES;
					}
					else {
						if (rightAlarmTime + DISPLAY_TIME_RES <= currentTime + DISPLAY_TIME_MAX) rightAlarmTime += DISPLAY_TIME_RES;
					}
					rightAlarmTime = display(rightAlarmTime - currentTime);
				}
				controlTimeout = currentTime + CONTROL_DURATION;
			}
			else if (buttons == LEFT) {
				if (leftState == RISING) {
					fadeLEDs(0, WAKE_FADE, LEFT);
					leftAlarmTime = leftRiseTime + RISE_DURATION + ALARM_DELAY;
					leftState = WAITING;
				}
				else {
					// TODO: detect ambient light
					display(leftAlarmTime - currentTime);
					fadeLEDs(CONTROL_INTENSITY, CONTROL_FADE, LEFT);
					controlTimeout = currentTime + CONTROL_DURATION;
					leftState = CONTROLLING;
				}
			}
			else {
				if (rightState == RISING) {
					fadeLEDs(0, WAKE_FADE, RIGHT);
					rightAlarmTime = rightRiseTime + RISE_DURATION + ALARM_DELAY;
					rightState = WAITING; // TODO: or snooze?
				}
				else {
					display(rightAlarmTime - currentTime);
					fadeLEDs(CONTROL_INTENSITY, CONTROL_FADE, RIGHT);
					controlTimeout = currentTime + CONTROL_DURATION;
					rightState = CONTROLLING;
				}
			}
		}

		// Rise and Alarm Triggers
		if (currentTime >= leftRiseTime) {
			fadeLEDs(RISE_INTENSITY, RISE_DURATION, LEFT);
			leftRiseTime += ONE_DAY;
			leftState = RISING;
		}
		else if (currentTime >= leftAlarmTime) {
			if ((alarmToggle ^= 1)) setLEDintensity(ALARM_INTENSITY, LEFT);
			else offLEDs(LEFT);
			leftAlarmTime += 1;
			// TODO: maybe random toggling would be more effective?
		}
		if (currentTime >= rightRiseTime) {
			fadeLEDs(RISE_INTENSITY, RISE_DURATION, RIGHT);
			rightRiseTime += ONE_DAY;
			rightState = RISING;
		}
		else if (currentTime >= rightAlarmTime) {
			if (!(alarmToggle ^= 1)) setLEDintensity(ALARM_INTENSITY, RIGHT);
			else offLEDs(RIGHT);
			rightAlarmTime += 1;
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
