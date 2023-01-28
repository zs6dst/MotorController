#include <TMC2209.h>
#include "motor.h"

#define STEP_PIN 0 // Step
#define DIR_PIN 4  // Direction

TMC2209 driver;
HardwareSerial &uart = Serial2;

void setupMotor(int speed, int acceleration)
{
    driver.setup(uart);

    // stepper->setSpeedInHz(speed);
    // stepper->setAcceleration(acceleration);
}

void setRPM(float rpm)
{
}

float getRPM()
{
    // float rpm = stepper->getCurrentSpeedInMilliHz() / 1000 * 60 / STEPS;
    return 0;
}

void setAcceleration(int value)
{
}

int getAcceleration()
{
    return 0;
}

void setSpeed(int value)
{
}

int getSpeed()
{
    return 0;
}