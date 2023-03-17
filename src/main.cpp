#include <Arduino.h>
#include <ArduinoJson.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <HardwareSerial.h>
#include "SD.h"

#include "main.h"
#include "motor.h"

static WebServer webserver(80);
static WebSocketsServer websocket = WebSocketsServer(81);
static HardwareSerial scale(1);
static Motor motor;
static hw_timer_t *timer = NULL;

static Data_t data;
static char weight[64];
static unsigned int steps;
static volatile int step = 0;
static volatile bool update = true;
static volatile bool done = false;

// Spec schedule here
const Step_t programme[] = {
    {5, 1.0},
    {2, 10.0},
    {5, 50.0},
    {2, 100.0}};

int getLED();
void setupLED();
void toggleLED();
void setupWiFi(bool useLocal);
void updateData();
void showSchedule();
void setupWeb(WebServer &, WebSocketsServer &, WebSocketsServerCore::WebSocketServerEvent);
void sendData(WebSocketsServer &, const Data_t *);
void onWebSocketEvent(byte, WStype_t, uint8_t *, size_t);
void setupScale(HardwareSerial &);
char *getWeight(HardwareSerial &scale, char (&)[64]);
void setupSDCard();
void logSD(float, char *);

void IRAM_ATTR onAlarm()
{
    ++step;
    update = true;
}

void setup()
{
    Serial.begin(115200);
    setupWiFi(true);
    setupWeb(webserver, websocket, onWebSocketEvent);
    setupLED();
    setupScale(scale);
    setupSDCard();
    motor.diagnose();

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onAlarm, true);
    timerAlarmWrite(timer, programme[step].duration * 1000000, true);
    step = 0;
    timerAlarmEnable(timer);

    steps = sizeof(programme) / sizeof(Step_t);
}

void loop()
{
    motor.setStealthChop();

    const int moment = 500; // ms
    static unsigned long earlier = 0;

    webserver.handleClient();
    websocket.loop();

    updateData();

    auto w = getWeight(scale, weight);
    logSD(data.rpm, w);

    sendData(websocket, &data);

    if (done)
        return;
    if (step < steps)
    {
        if (update)
        {
            showSchedule();
            timerAlarmWrite(timer, programme[step].duration * 1000000, true);
            motor.setRPM(programme[step].rpm);
        }
    }
    else
    {
        motor.setSpeed(0);
        timerAlarmDisable(timer);
        done = true;
        Serial.println("Schedule done.");
    }
}

void showSchedule()
{
    Serial.printf("Stage %d/%d: running at %5.1f RPM for %d seconds...\n",
                  step + 1, steps, programme[step].rpm, programme[step].duration);
    update = false;
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
