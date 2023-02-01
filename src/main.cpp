#include <Arduino.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

#include "main.h"
#include "network.h"
#include "webservers.h"
#include "led.h"
#include "motor.h"

#define now millis()

static Motor motor;
static Data data;

extern WebServer server;
extern WebSocketsServer websocket;

void updateData();

void setup()
{
    Serial.begin(115200);

    setupWiFi();
    setupServers();
    setupLED();

    motor.diagnose();
    motor.setSpeed(100);
}

void loop()
{
    motor.setStealthChop();

    const int moment = 500; // ms
    static unsigned long earlier = 0;

    server.handleClient();
    websocket.loop();

    if (now > earlier + moment)
    {
        updateData();
        broadcastData(&data);
        earlier = now;
    }
}

void setMotorRPM(float value)
{
    motor.setRPM(value);
}

void setMotoruSteps(uint value)
{
    motor.setMicroSteps((MICROSTEPS)value);
}

void setMotorAcceleration(uint value)
{
    motor.setAcceleration(value);
}

void sendData()
{
    broadcastData(&data);
}

void updateData()
{
    data.led = getLED();
    data.stealthChop = motor.getStealthChop();
    data.microSteps = motor.getMicroSteps();
    data.rpm = motor.getRPM();
    data.speed = motor.getSpeed();
    data.acceleration = motor.getAcceleration();
}
