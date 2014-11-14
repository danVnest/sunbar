#include <stdlib.h>
#include "led.h"
#include "display.h"
#include "speaker.h"
#include "clock.h"

#define RISE_DURATION		(30*60)
#define ALARM_DELAY		(15*60)
#define WAKE_FADE		5
#define CONTROL_DURATION	10
#define CONTROL_FADE		1
#define CONTROL_INTENSITY	0x9FFF
#define RISE_INTENSITY		0xFFFF
#define ALARM_INTENSITY		0xFFFF
#define TEST_INTENSITY		0xFFFF
#define HOLD_INTERVAL		0.25
#define ONE_DAY			(24L*60L*60L)
#define DEFAULT_ALARM_TIME 	(7.5*60L*60L)
#define SNOOZE_DURATION		(20*60)
#define SNOOZE_TIMEOUT		(3*60L*60L)
#define BUTTON_ON_THRESH 0xC000
#define BUTTON_OFF_THRESH 0x1FFF
#define IS_BUTTON_ON(filterValue) (((filterValue) | BUTTON_ON_THRESH) == ((BUTTON_ON_THRESH >> 1) + 0x8000))
#define IS_BUTTON_OFF(filterValue) (((filterValue) & BUTTON_OFF_THRESH) == (BUTTON_OFF_THRESH >> 1))
#define TOGGLE_U_LED		(PIND |= (1<<PD6))

enum STATES { WAITING, RISING, CONTROLLING };

uint8_t checkButtons(void);
void test(void);
