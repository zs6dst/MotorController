#include <TMCStepper.h>
#include <FastAccelStepper.h>
#include "motor.h"

#define STALL_VALUE 255     // [0..255]
#define DRIVER_ADDRESS 0b00 // TMC2209 Driver address according to MS1 and MS2
#define R_SENSE 0.11f       // 0.11 for MKS TMC2209

// SET THESE 3 PER YOUR CONNECTIONS, AND YOU ARE GOOD TO GO!:
#define DIR_PIN 4  // Direction
#define STEP_PIN 0 // Step

#define SERIAL_PORT Serial2

DRIVER driver(&SERIAL_PORT, R_SENSE, DRIVER_ADDRESS);
FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;

// DEFINE YOUR STARTING VALUES!
int AMPS = 1500;              // SET STARTING CURRENT MAX 2000
int micro = 0;                // SET MICROSTEPS
int Maccell = 1000;           // SET STARTING ACCELERATION
int Mspeed = 1000;            // SET STARTING STEPS/S
int MMperRev = 2;             // SET MM PER REVOLUTION
int moveMM = 57;              // SET MOVEMENT IN MM
float StepsPerRotation = 200; // PHYSICAL STEPS OF MOTOR, NO MICROSTEPS INCLUDED
unsigned long howLong = 0;

void setupMotor()
{
    SERIAL_PORT.begin(115200); // For motor comms

    driver.begin();

    driver.toff(2);        // [1..15] enable driver in software
    driver.blank_time(24); // [16, 24, 36, 54]

    driver.hysteresis_start(1); // [1..8]
    driver.hysteresis_end(12);  // [-3..12]

    driver.rms_current(AMPS, 0.01); // motor RMS current "rms_current will by default set ihold to 50% of irun but you can set your own ratio with additional second argument; rms_current(1000, 0.3)."

    driver.seimin(1); // minimum current for smart current control 0: 1/2 of IRUN 1: 1/4 of IRUN

    driver.semin(15); // [0... 15] If the StallGuard4 result falls below SEMIN*32, the motor current becomes increased to reduce motor load angle.
    driver.semax(15); // [0... 15]  If the StallGuard4 result is equal to or above (SEMIN+SEMAX+1)*32, the motor current becomes decreased to save energy.

    driver.sedn(4); // current down step speed 0-11%
    driver.seup(2); // Current increment steps per measured StallGuard2 value 5 seup0 %00 … %11: 1, 2, 4, 8

    driver.iholddelay(3); // 0 - 15 smooth current drop

    driver.TPWMTHRS(0); // 0: Disabled, 0xFFFFF = 1048575 MAX TSTEP.
                        // StealthChop PWM mode is enabled, if configured. When the velocity exceeds
                        // the limit set by TPWMTHRS, the driver switches to SpreadCycle.

    driver.TCOOLTHRS(0);          // 0-7 TSTEP
                                  // 0: TPWM_THRS= 0
                                  // 1: TPWM_THRS= 200
                                  // 2: TPWM_THRS= 300
                                  // 3: TPWM_THRS= 400
                                  // 4: TPWM_THRS= 500
                                  // 5: TPWM_THRS= 800
                                  // 6: TPWM_THRS= 1200
                                  // 7: TPWM_THRS= 4000
    driver.pwm_autoscale(true);   // Needed for stealthChop
    driver.en_spreadCycle(false); // false = StealthChop / true = SpreadCycle

    driver.microsteps(micro); // microsteps
    driver.shaft(false);      // direction
    driver.intpol(true);      // interpolate to 256 microsteps
    // driver.ihold(2);               // hold current  0=1/32 … 31=32/32
    // driver.irun(31);
    driver.SGTHRS(STALL_VALUE);
    // driver.I_scale_analog(0);   // if 5v vdd

    // ACCELL STEPPER SPEED & ACCELERATION
    engine.init();
    stepper = engine.stepperConnectToPin(STEP_PIN);
    stepper->setDirectionPin(DIR_PIN);
    stepper->setSpeedInHz(Mspeed); // STEPS PER SECOND
    stepper->setAcceleration(Maccell);

    if (micro)
        StepsPerRotation = StepsPerRotation * micro;
}

void setSpeed(int value)
{
    Serial.printf("setSpeed: %d\n", value);
    Mspeed = value;
    stepper->setSpeedInHz(value);
}

void setAcceleration(int value)
{
    Serial.printf("setAcceleration: %d\n", value);
    Maccell = value;
    stepper->setAcceleration(value);
}

void setAmps(int value)
{
    Serial.printf("setAmps: %d\n", value);
    AMPS = value;
    driver.rms_current(value, 0.01);
}

void MotorTask(void *)
{
    while (true)
    {
        bool blocking = true;

        unsigned long timeIs = millis();
        stepper->moveTo(moveMM / (MMperRev / StepsPerRotation), blocking); // TRUE makes this a blocking function. Remove it to use it as non blocking.
        howLong = millis() - timeIs;

        vTaskDelay(2000);

        stepper->moveTo(0, blocking); //

        vTaskDelay(2000);
    }
}