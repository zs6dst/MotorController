#include <Arduino.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

#include "webpage.h"
#include "main.h"

void toggleLED();

void setupWeb(WebServer &server, WebSocketsServer &websocket, WebSocketsServerCore::WebSocketServerEvent event)
{
    auto sendMainPage = [&server]()
    {
        server.send(200, "text/html", String(WEBPAGE));
    };

    server.on("/", sendMainPage);
    websocket.onEvent(event);
    server.begin();
    websocket.begin();
}

void sendData(WebSocketsServer &websocket, const Data_t *data)
{
    auto addDataItem = [](JsonArray *array, const char *id, const char *value) -> void
    {
        JsonObject item = array->createNestedObject();
        item["id"] = id;
        item["value"] = value;
    };

    StaticJsonDocument<1024> doc;
    JsonArray array = doc.to<JsonArray>();

    char led[8];
    sprintf(led, "%s", data->led ? "ON" : "OFF");
    addDataItem(&array, "led", led);

    char schop[8];
    sprintf(schop, "%s", data->stealthChop ? "ON" : "OFF");
    addDataItem(&array, "schop", schop);

    char usteps[16];
    sprintf(usteps, "%d", data->microSteps);
    addDataItem(&array, "uSteps", usteps);

    char rpm[16];
    sprintf(rpm, "%.1f", data->rpm);
    addDataItem(&array, "rpm", rpm);

    char speed[16];
    sprintf(speed, "%d", data->speed);
    addDataItem(&array, "speed", speed);

    char json[1024];
    serializeJson(array, json);
    websocket.broadcastTXT(json);
}