#include <FastAccelStepper.h>
#include "motor.h"

#define DIR_PIN 4  // Direction
#define STEP_PIN 0 // Step

float StepsPerRotation = 200; // PHYSICAL STEPS OF MOTOR, NO MICROSTEPS INCLUDED

void setupMotor()
{
    FastAccelStepperEngine engine = FastAccelStepperEngine();
    engine.init();
    auto stepper = engine.stepperConnectToPin(STEP_PIN);
    stepper->setDirectionPin(DIR_PIN);
    // stepper->setSpeedInHz(Mspeed);
    // stepper->setAcceleration(Maccell);
}

void setSpeed(int value)
{
    Serial.printf("setSpeed: %d\n", value);
}

void MotorTask(void *)
{
    // while (true)
    // {
    //     bool blocking = true;

    //     unsigned long timeIs = millis();
    //     stepper->moveTo(moveMM / (MMperRev / StepsPerRotation), blocking); // TRUE makes this a blocking function. Remove it to use it as non blocking.
    //     howLong = millis() - timeIs;

    //     vTaskDelay(2000);

    //     stepper->moveTo(0, blocking); //

    //     vTaskDelay(2000);
    // }
}