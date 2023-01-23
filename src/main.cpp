#include <Arduino.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <AccelStepper.h>

#include "main.h"
#include "network.h"
#include "webservers.h"
#include "led.h"
#include "motor.h"

void updateData();
float rpm();

Data data;
long t;
// TaskHandle_t Motor;

extern WebServer server;
extern WebSocketsServer websocket;
extern AccelStepper motor;

#define BASESTEPS 200
#define MICROSTEPS 8

const long STEPS = BASESTEPS * MICROSTEPS;

void setup()
{
    Serial.begin(115200);

    setupWiFi();
    setupServers();
    setupLED();

    setupMotor(STEPS, 1000000);
    Serial.printf("Setup done.Running at %4.1f RPM\n", rpm());
}

void loop()
{
    motor.runSpeed();
}

float rpm()
{
    return motor.speed() * 60 / STEPS;
}

void updateData()
{
    data.led = getLED();
}
