#include <Arduino.h>
#include <ArduinoJson.h>
#include <HardwareSerial.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "sdcard.h"

#include "main.h"
#include "motor.h"

// Devices
static AsyncWebServer webserver(80);
static HardwareSerial scale(1);
static hw_timer_t *timer = NULL;
static Motor motor;
static SDCard sdcard;

// Variables
static Data_t data;
static char weight[64];
static unsigned int steps;
static volatile int step = 0;
static volatile bool update = true;
static volatile bool done = false;

// Spec programme here
const Step_t programme[] = {
    {5, 1.0},
    {2, 10.0},
    {5, 50.0},
    {2, 100.0}};

// Forward declarations
int getLED();
void setupLED();
void toggleLED();
void setupWiFi(bool useLocal);
void updateData();
void showSchedule();
void setupWeb();
// void sendData(WebSocketsServer &, const Data_t *);
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

    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
    Serial.println("SPIFFS mounted successfully");

    setupWiFi(true);
    setupWeb();
    setupLED();
    setupScale(scale);
    motor.diagnose();
    sdcard.diagnose();

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

    updateData();

    auto w = getWeight(scale, weight);
    sdcard.log(data.rpm, w);

    // sendData(websocket, &data);

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

void updateData()
{
    data.led = getLED() == HIGH;
    data.stealthChop = motor.getStealthChop();
    data.microSteps = motor.getMicroSteps();
    data.rpm = motor.getRPM();
    data.speed = motor.getSpeed();
}

void setupWeb()
{
    webserver.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->send(SPIFFS, "/index.html", "text/html"); });

    webserver.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->send(SPIFFS, "/style.css", "text/css"); });

    webserver.begin(); 
}
