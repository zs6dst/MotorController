#ifndef __SCALE__
#define __SCALE__

#include <HardwareSerial.h>

void setupScale(HardwareSerial &);
char *getWeight(HardwareSerial &scale, char (&)[64]);

#endif