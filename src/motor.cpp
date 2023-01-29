#include <TMC2209.h>
#include "motor.h"

Motor::Motor()
{
    driver.setup(Serial2);
    baseSteps = 200;
    microSteps = 8;
}

uint Motor::getSteps()
{
    return baseSteps * microSteps;
}

uint Motor::getMicroSteps()
{
    return microSteps;
}

void Motor::setMicroSteps(MICROSTEPS value)
{
    microSteps = value;
    driver.setMicrostepsPerStep(value);
}

uint Motor::getSpeed()
{
    return speed;
}

void Motor::setSpeed(uint value)
{
    speed = value;
    driver.moveAtVelocity(value);
}

void Motor::setRPM(float rpm)
{
    uint speed = rpm * baseSteps * microSteps / 60;
}

float Motor::getRPM()
{
    return speed * 60.0 / baseSteps / microSteps;
}

// void Motor::setAcceleration(uint value)
// {
//     acceleration = value;
// }

// uint Motor::getAcceleration()
// {
//     return acceleration;
// }
