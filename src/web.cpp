#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "main.h"

void setValue(String, String);

void setupWeb(AsyncWebServer &webserver, AsyncEventSource &events)
{
    webserver.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->send(SPIFFS, "/index.html", "text/html"); });

    webserver.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->send(SPIFFS, "/styles.css", "text/css"); });

    webserver.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->send(SPIFFS, "/main.js", "application/javascript"); });

    webserver.on("/setvalue", HTTP_GET, [](AsyncWebServerRequest *request)
                 { 
                    auto p = request->getParam(0);
                    setValue(p->name(), p->value());
                    request->send(200, "text/plain", ""); 
                });

    webserver.addHandler(&events);
    webserver.begin();
}

void sendData(AsyncEventSource &events, Data_t &data)
{
    events.send(String(data.led ? "ON" : "OFF").c_str(), "led", millis());
    events.send(String(data.stealthChop ? "ON" : "OFF").c_str(), "stealthchop", millis());
    events.send(String(data.rpm).c_str(), "rpm", millis());
    events.send(String(data.microSteps).c_str(), "microsteps", millis());
    events.send(String(data.speed).c_str(), "speed", millis());
}