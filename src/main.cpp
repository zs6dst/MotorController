#include <Arduino.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <TMCStepper.h>
#include <FastAccelStepper.h>
#include "main.h"
#include "webservers.h"
#include "network.h"
#include "led.h"
#include "motor.h"

void updateData();

Data data;
TaskHandle_t Motor;

extern WebServer server;
extern WebSocketsServer websocket;
extern DRIVER driver;


void setup()
{
    Serial.begin(115200);

    connectToWiFi();
    setupServers();
    setupLED();
    setupMotor();

    xTaskCreatePinnedToCore(MotorTask, "Motor", 5000, NULL, 5, &Motor, 0);
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
}
