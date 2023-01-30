#include <TMC2209.h>
#include "motor.h"

Motor::Motor()
{
    baseSteps = 200;
    microSteps = 8;
    acceleration = 1000000;

    driver.setup(Serial2);
    driver.setMicrostepsPerStep(microSteps);
    driver.setRunCurrent(100);
    driver.enable();
}

void Motor::diagnose()
{
    bool result = driver.isSetupAndCommunicating();
    Serial.printf("Stepper driver %ssetup and communicating!\n", result ? "" : "not");
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
    driver.moveAtVelocity(speed);
}

float Motor::getRPM()
{
    return (float)(this->speed * 60.0 / baseSteps / microSteps);
}

void Motor::setRPM(float rpm)
{
    this->speed = (uint)(rpm * baseSteps * microSteps / 60);
    driver.moveAtVelocity(this->speed);
}

void Motor::setAcceleration(uint value)
{
    acceleration = value;
}

uint Motor::getAcceleration()
{
    return acceleration;
}
