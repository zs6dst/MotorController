#ifndef __MOTOR__
#define __MOTOR__

#define BASESTEPS 200
#define MICROSTEPS 8

const long STEPS = BASESTEPS * MICROSTEPS;

void setupMotor(float speed, float maxSpeed);
void setRPM(float value);
float rpm();
void MotorTask(void *);

#endif