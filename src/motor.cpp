#include <TMC2209.h>
#include "motor.h"

#define UART Serial2
#define STEALTHCHOP_THRS 115 //170rpm @ 256uSteps

Motor::Motor()
{
    driver.setup(UART);
    driver.setMicrostepsPerStep(microSteps);
    driver.setRunCurrent(100);
    driver.enableCoolStep();
    driver.setStealthChopDurationThreshold(STEALTHCHOP_THRS);
    driver.enable();
}

void Motor::diagnose()
{
    bool result = driver.isSetupAndCommunicating();
    Serial.printf("Stepper driver %s configured and communicating!\n", result ? "is" : "not");
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
    //Validate value (so that it cannot be set to eg. 3) 
    switch (value)
    {
    default:
        return;
    case _1:
    case _2:
    case _4:
    case _8:
    case _16:
    case _32:
    case _64:
    case _128:
    case _256:
        break;
    }
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

bool Motor::getStealthChop()
{
    TMC2209::Settings settings = driver.getSettings();
    return settings.stealth_chop_enabled;
}

void Motor::setStealthChop()
{
    uint32_t tstep = driver.getInterstepDuration();
    Serial.println(tstep);
    if (tstep < STEALTHCHOP_THRS) 
        driver.disableStealthChop();
    else 
        driver.enableStealthChop();
}
