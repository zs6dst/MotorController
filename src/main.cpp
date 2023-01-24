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

Data data;
long t;

extern WebServer server;
extern WebSocketsServer websocket;
extern AccelStepper motor;

void setup()
{
    Serial.begin(115200);

    setupWiFi();
    setupServers();
    setupLED();

    setupMotor(STEPS, 1000000);
    Serial.printf("Setup done.Running at %4.1f RPM\n", toRPM());
}

void loop()
{
    motor.runSpeed();

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
    data.rpm = toRPM();
    data.speed = motor.speed();
}

