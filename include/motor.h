#ifndef __MOTOR__
#define __MOTOR__

#define DRIVER TMC2209Stepper //Set the actual stepper driver in use

void setupMotor();
void setSpeed(int);
void setAcceleration(int);
void setAmps(int);
void MotorTask(void *);

#endif