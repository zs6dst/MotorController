#include <Arduino.h>
#include <ArduinoJson.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <HardwareSerial.h>

#include "main.h"
#include "network.h"
#include "webservers.h"
#include "led.h"
#include "motor.h"
#include "scale.h"

static WebServer webserver(80);
static WebSocketsServer websocket = WebSocketsServer(81);
static HardwareSerial scale(1);
static Motor motor;
static Data data;

void updateData();
void onWebSocketEvent(byte, WStype_t, uint8_t *, size_t);

void setup()
{
    Serial.begin(115200);
    setupWiFi(true);
    setupWeb(webserver, websocket, onWebSocketEvent);
    setupLED();
    setupScale(scale);

    motor.diagnose();
}

void loop()
{
    motor.setStealthChop();

    const int moment = 500; // ms
    static unsigned long earlier = 0;

    webserver.handleClient();
    websocket.loop();

    static char weight[64];
    int newWeight = getWeight(scale, weight);
    if (newWeight)
    {
        Serial.printf("Data: %s\n", weight);
        newWeight = false;
    }

    delay(1000);

    if (millis() > earlier + moment)
    {
        updateData();
        sendData(websocket, &data);
        earlier = millis();
    }
}

void onWebSocketEvent(byte num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.println("Client disconnected");
        break;

    case WStype_CONNECTED:
        Serial.println("Client connected");
        sendData(websocket, &data);
        break;

    case WStype_TEXT:
        Serial.printf("Request received: %s\n", payload);

        StaticJsonDocument<32> req;
        deserializeJson(req, payload);

        auto id = req["id"];
        if (id == "restart")
            ESP.restart();
        else if (id == "led")
            toggleLED();
        else if (id == "rpm")
            motor.setRPM(atof(req["value"]));
        else if (id == "usteps")
            motor.setMicroSteps((MICROSTEPS)atoi(req["value"]));
        break;
    }
}

void updateData()
{
    data.led = getLED() == HIGH;
    data.stealthChop = motor.getStealthChop();
    data.microSteps = motor.getMicroSteps();
    data.rpm = motor.getRPM();
    data.speed = motor.getSpeed();
}
