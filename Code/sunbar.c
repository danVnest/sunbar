#include "sunbar.h"

static volatile char _buttons = NONE;

int main(void) {
	CLKPR = (1<<CLKPCE); CLKPR = 0x00; // set to F_CPU
	float leftRiseTime = 7*60*60;
	float rightRiseTime = leftRiseTime;
	float leftAlarmTime = leftRiseTime + RISE_DURATION + ALARM_DELAY;
	float rightAlarmTime = leftAlarmTime;
	float controlTimeout = 0;
	char leftState = WAITING;
	char rightState = WAITING;
	char alarmToggle = 1;
	initLEDs();
	initDisplay();
	initClock();
	DDRD |= (1<<PD6); 
	PORTD |= (1<<INT0) | (1<<INT1);
	EICRA |= (1<<ISC11) | (1<<ISC01);
	EIMSK |= (1<<INT0) | (1<<INT1);
	sei();
	testLEDs();
	while (1) {
		float currentTime = time();
		// Control Timeout - Update Settings 
		if ((leftState == CONTROLLING) || (rightState == CONTROLLING)) {
			if (currentTime >= controlTimeout) {
				TOGGLE_U_LED;
				offDisplay();
				fadeLEDs(0, CONTROL_FADE, BOTH);
				if (leftState == CONTROLLING) {
					leftRiseTime = leftAlarmTime - RISE_DURATION - ALARM_DELAY;
					leftState = WAITING;
				}
				if (rightState == CONTROLLING) {
					if (leftState == CONTROLLING) rightRiseTime = leftRiseTime;
					else rightRiseTime = rightAlarmTime - RISE_DURATION - ALARM_DELAY;
					rightState = WAITING;
				}
			}
			else checkDisplay(currentTime);
		}
		// Button Control 
		char buttons = _buttons;
		// TODO: change all chars to uint8_t
		if (buttons != NONE) { // TODO: detect button hold
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
					rightState = WAITING; // TODO: or snooze?
				}
				else {
					display(rightAlarmTime - currentTime);
					fadeLEDs(CONTROL_INTENSITY, CONTROL_FADE, RIGHT);
					controlTimeout = currentTime + CONTROL_DURATION;
					rightState = CONTROLLING;
				}
			}
			if (buttons == _buttons) _buttons = NONE;
		}

		// Rise and Alarm Triggers
		if (currentTime >= leftRiseTime) {
			TOGGLE_U_LED;
			fadeLEDs(RISE_INTENSITY, RISE_DURATION, LEFT);
			leftRiseTime += ONE_DAY;
			leftState = RISING;
		}
		else if (currentTime >= leftAlarmTime) {
			if ((alarmToggle ^= 1)) setLEDintensity(ALARM_INTENSITY, LEFT);
			else offLEDs(LEFT);
			leftAlarmTime += 1;
		}
		if (currentTime >= rightRiseTime) {
			fadeLEDs(RISE_INTENSITY, RISE_DURATION, RIGHT);
			rightRiseTime += ONE_DAY;
			rightState = RISING;
		}
		else if (currentTime >= rightAlarmTime) {
			if (!(alarmToggle ^= 1)) setLEDintensity(ALARM_INTENSITY, RIGHT);
			else offLEDs(LEFT);
			rightAlarmTime += 1;
		}
	}
}

void testLEDs(void) {
	float start = time();
	fadeLEDs(TEST_INTENSITY, 2, LEFT);
	fadeLEDs(TEST_INTENSITY, 0.5, RIGHT);
	while (time() < start + 1);
	fadeLEDs(0, 0.5, RIGHT);
	while (time() < start + 2);
	fadeLEDs(TEST_INTENSITY, 0.5, RIGHT);
	while (time() < start + 3);
	fadeLEDs(0, 2, LEFT);
	fadeLEDs(0, 0.5, RIGHT);
	while (time() < start + 4);
	fadeLEDs(TEST_INTENSITY, 0.5, RIGHT);
	while (time() < start + 5);
	fadeLEDs(0, 0.5, RIGHT);
	while (time() < start + 6);
}

ISR(INT0_vect) {
	if (PORTD & (1<<INT1)) _buttons = LEFT;
	else _buttons = BOTH;
	TOGGLE_U_LED;
}

ISR(INT1_vect) {
	if (PORTD & (1<<INT0)) _buttons = RIGHT;
	else _buttons = BOTH;
	TOGGLE_U_LED;
}
