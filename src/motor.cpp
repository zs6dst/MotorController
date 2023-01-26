#include <FastAccelStepper.h>
#include "motor.h"

#define STEP_PIN 0 // Step
#define DIR_PIN 4  // Direction

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;

void setupMotor(uint32_t speed, uint32_t acceleration)
{
    engine.init();
    stepper = engine.stepperConnectToPin(STEP_PIN);
    stepper->setDirectionPin(DIR_PIN);

    stepper->setSpeedInHz(speed);
    stepper->setAcceleration(acceleration);
}

float toRPM()
{
    float rpm = stepper->getCurrentSpeedInMilliHz() / 1000 * 60 / STEPS;
    return rpm;
}

void setRPM(float rpm)
{
    float speed = rpm * STEPS / 60;
    stepper->setSpeedInHz((uint32_t)speed);
    stepper->applySpeedAcceleration();
    speed = stepper->getCurrentSpeedInMilliHz();
}

void setAcceleration(int32_t value)
{
    stepper->setAcceleration(value);
    stepper->applySpeedAcceleration();
}
