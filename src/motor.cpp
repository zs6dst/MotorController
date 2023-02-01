#include <TMC2209.h>
#include "motor.h"

#define STEALTHCHOP_THRS 650

Motor::Motor()
{
    baseSteps = 200;
    microSteps = 2;
    acceleration = 1000000;

    driver.setup(Serial2);
    driver.setMicrostepsPerStep(microSteps);
    driver.setRunCurrent(100);
    driver.enableCoolStep();
    driver.setStealthChopDurationThreshold(STEALTHCHOP_THRS);
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
    setSpeed((uint)(rpm * baseSteps * microSteps / 60));
}

void Motor::setAcceleration(uint value)
{
    acceleration = value;
}

uint Motor::getAcceleration()
{
    return acceleration;
}

bool Motor::getStealthChop()
{
    TMC2209::Settings settings = driver.getSettings();
    return settings.stealth_chop_enabled;
}

uint Motor::getTSTEP()
{
    return driver.getInterstepDuration();
}

void Motor::setStealthChop()
{
    (getTSTEP() < STEALTHCHOP_THRS) ? driver.disableStealthChop() : driver.enableStealthChop();
}