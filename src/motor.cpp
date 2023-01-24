#include <AccelStepper.h>
#include "motor.h"

#define STEP_PIN 0 // Step
#define DIR_PIN 4  // Direction

AccelStepper motor = AccelStepper(1, STEP_PIN, DIR_PIN);

void setupMotor(float speed, float maxSpeed)
{
    motor.setMaxSpeed(maxSpeed);
    motor.setSpeed(speed);
}

float toRPM()
{
    float rpm = motor.speed() * 60 / STEPS;
    return rpm;
}

void setRPM(float rpm)
{
    float speed = rpm * STEPS / 60;
    motor.setSpeed(speed);
}

void MotorTask(void *)
{
    while (true)
    {
        motor.runSpeed();
        // vTaskDelay(2000);
    }
}