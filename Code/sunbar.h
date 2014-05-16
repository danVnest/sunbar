#include "led.h"
#include "display.h"
#include "clock.h"
#include <math.h>

#define RISE_DURATION		(30*60)
#define ALARM_DELAY		(30*60)
#define WAKE_FADE		30
#define CONTROL_DURATION	5
#define CONTROL_FADE		1
#define CONTROL_INTENSITY	0xFFFF
#define RISE_INTENSITY		0xFFFF
#define ALARM_INTENSITY		0xFFFF
#define TEST_INTENSITY		0xFFFF
#define ONE_DAY			(24L*60L*60L)

enum STATES { WAITING, RISING, CONTROLLING };

void testLEDs(void);
