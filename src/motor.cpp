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

void setSpeed(float value)
{
    motor.setSpeed(value);
}

float rpm()
{
    return motor.speed() * 60 / STEPS;
}

void setRPM(float rpm)
{
    float speed = rpm * STEPS / 60;
    motor.setSpeed(speed);
    Serial.printf("RPM: %4.1f\tSpeed: %6.1f\n", rpm, speed);
}

void MotorTask(void *)
{
    while (true)
    {
        motor.runSpeed();
        // vTaskDelay(2000);
    }
}