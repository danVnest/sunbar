#include "led.h"
#include "display.h"
#include "clock.h"

#define RISE_DURATION		(30*60)
#define ALARM_DELAY		(15*60)
#define WAKE_FADE		30
#define CONTROL_DURATION	10
#define CONTROL_FADE		1
#define CONTROL_INTENSITY	0x9FFF
#define RISE_INTENSITY		0xFFFF
#define ALARM_INTENSITY		0xFFFF
#define TEST_INTENSITY		0xFFFF
#define HOLD_INTERVAL		0.25
#define ONE_DAY			(24L*60L*60L)
#define DEFAULT_ALARM_TIME 	(7.5*60L*60L)
#define SNOOZE_DURATION		(15*60)
#define SNOOZE_TIMEOUT		(2*60*60)

#define TOGGLE_U_LED		(PIND |= (1<<PD6))

enum STATES { WAITING, RISING, CONTROLLING };

uint8_t checkButtons(void);
void testLEDs(void);
