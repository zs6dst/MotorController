#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "webpage.h"

#define LED 2

// Local wifi
const char localSSID[] = "zeniton";
const char password[] = "CT33KATPRbaXN";

WebServer server(80);
bool led;

void connectToWiFi();
void sendPage(WiFiClient *);
void onConnectHandler();
void onLedHandler();

typedef struct
{
    bool led;
} Result;

void setup()
{
    pinMode(LED, OUTPUT);

    Serial.begin(115200);

    connectToWiFi();

    server.on("/", onConnectHandler);
    server.on("/led", onLedHandler);
    server.begin();
}

void loop()
{
    server.handleClient();
}

void connectToWiFi()
{
    Serial.print("Connecting to local network...");
    WiFi.begin(localSSID, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }

    Serial.print("\nConnected. IP: ");
    Serial.println(WiFi.localIP());
}

void onConnectHandler()
{
    server.send(200, "text/html", String(WEBPAGE));
}

void onLedHandler()
{
    led = !led;
    digitalWrite(LED, led ? HIGH : LOW);

    StaticJsonDocument<JSON_OBJECT_SIZE(1)> result;
    result["led"] = led;
    
    char json[32];
    serializeJson(result, json);

    server.send(200, "application/json", String(json));
}