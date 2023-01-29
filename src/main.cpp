#include <Arduino.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <TMC2209.h>

#include "main.h"
#include "network.h"
#include "webservers.h"
#include "led.h"
#include "motor.h"

#define now millis() 

void updateData();

Data data;
long t;

extern WebServer server;
extern WebSocketsServer websocket;

Motor motor;

void setup()
{
    Serial.begin(115200);

    setupWiFi();
    setupServers();
    setupLED();
}

void loop()
{
    const int moment = 1000; //ms
    static unsigned long earlier = 0;

    server.handleClient();
    websocket.loop();

    if (now > earlier + moment)
    {
        updateData();
        sendData(data);
        earlier = now;
    }
}

void updateData()
{
    data.led = getLED();
    data.rpm = motor.getRPM();
    data.speed = motor.getSpeed();
    // data.acceleration = motor.getAcceleration();
}
