#include <Arduino.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

#include "main.h"
#include "webservers.h"
#include "webpage.h"

extern Data data;

void toggleLED();
void setRPM(float value);

WebServer server(80);
WebSocketsServer websocket = WebSocketsServer(81);

void setupServers()
{
    server.on("/", onConnectHandler);
    websocket.onEvent(onWebSocketEvent);
    server.begin();
    websocket.begin();
}

void onConnectHandler()
{ // Send the webpage
    server.send(200, "text/html", String(WEBPAGE));
}

void addDataItem(JsonArray *array, const char *id, const char *value)
{
    JsonObject item = array->createNestedObject();
    item["id"] = id;
    item["value"] = value;
}

void sendData(Data data)
{
    StaticJsonDocument<1024> doc;
    JsonArray array = doc.to<JsonArray>();

    char led[8];
    sprintf(led, "%s", data.led ? "ON" : "OFF");
    addDataItem(&array, "led", led);

    char rpm[16];
    sprintf(rpm, "%.1f", data.rpm);
    addDataItem(&array, "rpm", rpm);
    
    char speed[16];
    sprintf(speed, "%.1f", data.speed);
    addDataItem(&array, "speed", speed);

    char json[1024];
    serializeJson(array, json);
    websocket.broadcastTXT(json);
}

void onWebSocketEvent(byte num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.println(": Client disconnected");
        break;

    case WStype_CONNECTED:
        Serial.println(": Client connected");
        sendData(data);
        break;

    case WStype_TEXT:
        Serial.printf("Request received: %s\n", payload);

        StaticJsonDocument<JSON_OBJECT_SIZE(2)> req;
        deserializeJson(req, payload);

        auto id = req["id"];
        if (id == "led")
            toggleLED();
        else if (id == "rpm")
            setRPM(atof(req["value"]));
        break;
    }
}
