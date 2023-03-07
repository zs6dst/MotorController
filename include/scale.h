#ifndef __SCALE__
#define __SCALE__

#include <HardwareSerial.h>

void setupScale(HardwareSerial &);
int getWeight(HardwareSerial &, char *);

#endif