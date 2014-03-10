#include <avr/io.h>

#define TOP 0xFFFF // maximum resolution for >200Hz

enum STRIPS { LEFT, RIGHT, BOTH };

void initLEDs(void);
void onLEDs(char strip);
void offLEDs(char strip);
void setLEDintensity(unsigned int intensity, char strip);
void fadeLEDs(unsigned int intensity, unsigned int duration, char strip);
