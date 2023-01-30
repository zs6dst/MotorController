#include <Arduino.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

#include "main.h"
#include "webservers.h"
#include "webpage.h"
#include "motor.h"

void toggleLED();

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

void broadcastData(const Data *data)
{
    StaticJsonDocument<1024> doc;
    JsonArray array = doc.to<JsonArray>();

    char led[8];
    sprintf(led, "%s", data->led ? "ON" : "OFF");
    addDataItem(&array, "led", led);

    char rpm[16];
    sprintf(rpm, "%.1f", data->rpm);
    addDataItem(&array, "rpm", rpm);
    
    char speed[16];
    sprintf(speed, "%d", data->speed);
    addDataItem(&array, "speed", speed);
    
    // char accel[16];
    // sprintf(accel, "%d", data.acceleration);
    // addDataItem(&array, "accel", accel);

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
        sendData();
        break;

    case WStype_TEXT:
        Serial.printf("Request received: %s\n", payload);

        StaticJsonDocument<JSON_OBJECT_SIZE(2)> req;
        deserializeJson(req, payload);

        auto id = req["id"];
        if (id == "restart")
            ESP.restart();
        else if (id == "led")
            toggleLED();
        else if (id == "rpm")
            setMotorRPM(atof(req["value"]));
        // else if (id == "accel")
        //     motor.setAcceleration(atoi(req["value"]));
        break;
    }
}
