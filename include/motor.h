#ifndef __MOTOR__
#define __MOTOR__

#define BASESTEPS 200
#define MICROSTEPS 8

const long STEPS = BASESTEPS * MICROSTEPS;

void setupMotor(uint32_t speed, uint32_t acceleration);
void setRPM(float value);
void setAcceleration(int32_t value);
float toRPM();
void MotorTask(void *);

#endif