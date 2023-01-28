#ifndef __MOTOR__
#define __MOTOR__

#define BASESTEPS 200
#define MICROSTEPS 8

const long STEPS = BASESTEPS * MICROSTEPS;

void setupMotor(int speed, int acceleration);
void setRPM(float value);
float getRPM();
void setAcceleration(int value);
int getAcceleration();
void setSpeed(int value);
int getSpeed();

#endif