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
    motor.setRPM(60);
}

void loop()
{
    const int moment = 1000; // ms
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

void sendData()
{
    broadcastData(&data);
}

void updateData()
{
    data.led = getLED();
    data.microSteps = motor.getMicroSteps();
    data.rpm = motor.getRPM();
    data.speed = motor.getSpeed();
    // data.acceleration = motor.getAcceleration();
}
