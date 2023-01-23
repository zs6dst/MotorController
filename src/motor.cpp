#include <AccelStepper.h>
#include "motor.h"

#define STEPS_PER_ROTATION 200 // Set for motor in use
#define STEP_PIN 0             // Step
#define DIR_PIN 4              // Direction

AccelStepper motor = AccelStepper(1, STEP_PIN, DIR_PIN);

void setupMotor(float speed, float maxSpeed)
{
    motor.setMaxSpeed(maxSpeed);
    motor.setSpeed(speed);
}

void setSpeed(int value)
{
}

void MotorTask(void *)
{
    while (true)
    {
        motor.runSpeed();
        // vTaskDelay(2000);
    }
}