#include <Arduino.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <TMC2209.h>

#include "main.h"
#include "network.h"
#include "webservers.h"
#include "led.h"
#include "motor.h"

void updateData();

Data data;
long t;

extern WebServer server;
extern WebSocketsServer websocket;

void setup()
{
    Serial.begin(115200);

    setupWiFi();
    setupServers();
    setupLED();
    setupMotor(5000, 5000);

    // stepper->move(1);
    // stepper->keepRunning();
}

void loop()
{
    const int delta = 1000;
    static unsigned long t = 0;

    server.handleClient();
    websocket.loop();

    if ((millis() - t) >= delta)
    {
        updateData();
        sendData(data);
        t = millis();
    }
}

void updateData()
{
    data.led = getLED();
    data.rpm = getRPM();
    data.speed = getSpeed();
    data.acceleration = getAcceleration();
}
