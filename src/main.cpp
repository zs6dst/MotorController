#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include "webpage.h"

#define LED 2

// Local wifi
const char localSSID[] = "zeniton";
const char password[] = "CT33KATPRbaXN";

WebServer server(80);
WebSocketsServer websocket = WebSocketsServer(81);

int interval = 1000;              // Virtual delay
unsigned long previousMillis = 0; // Tracks the time since last event fired

bool led;

void connectToWiFi();
void sendPage(WiFiClient *);
void onConnectHandler();
void onWebSocketEvent(byte num, WStype_t type, uint8_t *payload, size_t length);
void sendData();
void updateData();
void toggleLed();
void setSpeed(int);
void setAcceleration(int);
void setAmps(int);

// LIBRARIES THAT YOU NEED
#include <TMCStepper.h>       // https://www.arduino.cc/reference/en/libraries/tmcstepper/
#include "FastAccelStepper.h" // https://www.arduino.cc/reference/en/libraries/fastaccelstepper/

// DEFINE YOUR STARTING VALUES!
int AMPS = 1500;              // SET STARTING CURRENT MAX 2000
int micro = 0;                // SET MICROSTEPS
int Maccell = 1000;           // SET STARTING ACCELERATION
int Mspeed = 1000;            // SET STARTING STEPS/S
int MMperRev = 2;             // SET MM PER REVOLUTION
int moveMM = 57;              // SET MOVEMENT IN MM
float StepsPerRotation = 200; // PHYSICAL STEPS OF MOTOR, NO MICROSTEPS INCLUDED

// SET THESE 3 PER YOUR CONNECTIONS, AND YOU ARE GOOD TO GO!:
#define DIR_PIN 4           // Direction
#define STEP_PIN 0          // Step
#define SERIAL_PORT Serial2 // HardwareSerial port

#define STALL_VALUE 255     // [0..255]
#define DRIVER_ADDRESS 0b00 // TMC2209 Driver address according to MS1 and MS2
#define R_SENSE 0.11f       // 0.11 for MKS TMC2209

TMC2209Stepper driver(&SERIAL_PORT, R_SENSE, DRIVER_ADDRESS);
using namespace TMC2209_n;

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;

unsigned long howLong = 0;

TaskHandle_t Motor;
void MotorTask(void *);

void setup()
{
    pinMode(LED, OUTPUT);

    Serial.begin(115200);

    connectToWiFi();

    server.on("/", onConnectHandler);
    websocket.onEvent(onWebSocketEvent);
    server.begin();
    websocket.begin();

    // INTITIALIZE SERIAL0 ITERFACE
    Serial.println(F("UART0 serial output interface intitialized @ 115200"));

    // INITIALIZE SERIAL2 UART FOR TMC2209
    SERIAL_PORT.begin(115200); //,SERIAL_8N1, 16, 17
    Serial.println(F("UART2 interface for TMC2209 intitialized   @ 115200"));

    // TMCSTEPPER SETTINGS
    driver.begin();

    driver.toff(2);        // [1..15] enable driver in software
    driver.blank_time(24); // [16, 24, 36, 54]

    driver.hysteresis_start(1); // [1..8]
    driver.hysteresis_end(12);  // [-3..12]

    driver.rms_current(AMPS, 0.01); // motor RMS current "rms_current will by default set ihold to 50% of irun but you can set your own ratio with additional second argument; rms_current(1000, 0.3)."

    driver.seimin(1); // minimum current for smart current control 0: 1/2 of IRUN 1: 1/4 of IRUN

    driver.semin(15); // [0... 15] If the StallGuard4 result falls below SEMIN*32, the motor current becomes increased to reduce motor load angle.
    driver.semax(15); // [0... 15]  If the StallGuard4 result is equal to or above (SEMIN+SEMAX+1)*32, the motor current becomes decreased to save energy.

    driver.sedn(4); // current down step speed 0-11%
    driver.seup(2); // Current increment steps per measured StallGuard2 value 5 seup0 %00 … %11: 1, 2, 4, 8

    driver.iholddelay(3); // 0 - 15 smooth current drop

    driver.TPWMTHRS(0); // 0: Disabled, 0xFFFFF = 1048575 MAX TSTEP.
                        // StealthChop PWM mode is enabled, if configured. When the velocity exceeds
                        // the limit set by TPWMTHRS, the driver switches to SpreadCycle.

    driver.TCOOLTHRS(0);          // 0-7 TSTEP
                                  // 0: TPWM_THRS= 0
                                  // 1: TPWM_THRS= 200
                                  // 2: TPWM_THRS= 300
                                  // 3: TPWM_THRS= 400
                                  // 4: TPWM_THRS= 500
                                  // 5: TPWM_THRS= 800
                                  // 6: TPWM_THRS= 1200
                                  // 7: TPWM_THRS= 4000
    driver.pwm_autoscale(true);   // Needed for stealthChop
    driver.en_spreadCycle(false); // false = StealthChop / true = SpreadCycle

    driver.microsteps(micro); // microsteps
    driver.shaft(false);      // direction
    driver.intpol(true);      // interpolate to 256 microsteps
    // driver.ihold(2);               // hold current  0=1/32 … 31=32/32
    // driver.irun(31);
    driver.SGTHRS(STALL_VALUE);
    // driver.I_scale_analog(0);   // if 5v vdd

    // ACCELL STEPPER SPEED & ACCELERATION
    engine.init();
    stepper = engine.stepperConnectToPin(STEP_PIN);
    stepper->setDirectionPin(DIR_PIN);
    stepper->setSpeedInHz(Mspeed); // STEPS PER SECOND
    stepper->setAcceleration(Maccell);

    if (micro)
        StepsPerRotation = StepsPerRotation * micro;

    // xTaskCreatePinnedToCore(PrintTask, "Print", 2000, NULL, tskIDLE_PRIORITY, &Print, 1);
    xTaskCreatePinnedToCore(MotorTask, "Motor", 5000, NULL, 5, &Motor, 0);
}

void loop()
{
    server.handleClient();
    websocket.loop();

    auto currentMillis = millis();
    if ((currentMillis - previousMillis) >= interval)
    {
        updateData();
        sendData();
        previousMillis = currentMillis;
    }
}

void updateData()
{
    led = digitalRead(LED) == HIGH;
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
{ // Send the webpage
    server.send(200, "text/html", String(WEBPAGE));
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
        if (id == "led")
            toggleLed();
        if (id == "speed")
            setSpeed(atoi(req["value"]));
        if (id == "accel")
            setAcceleration(atoi(req["value"]));
        if (id == "amps")
            setAmps(atoi(req["value"]));
        break;
    }
}

void toggleLed()
{
    digitalWrite(LED, led ? LOW : HIGH);
}

void setSpeed(int value)
{
    Serial.printf("setSpeed: %d\n", value);
    Mspeed = value;
    stepper->setSpeedInHz(value);
}

void setAcceleration(int value)
{
    Serial.printf("setAcceleration: %d\n", value);
    Maccell = value;
    stepper->setAcceleration(value);
}

void setAmps(int value)
{
    Serial.printf("setAmps: %d\n", value);
    AMPS = value;
    driver.rms_current(value, 0.01);
}

void addDataItem(JsonArray *array, const char *id, const char *value)
{
    JsonObject item = array->createNestedObject();
    item["id"] = id;
    item["value"] = value;
}

void sendData()
{
    int SSpeed;
    int steppes;
    float ActualSpeed;
    float MaxSpeed;

    steppes = driver.TSTEP();
    SSpeed = 12000000. / (steppes * 256.);
    ActualSpeed = (SSpeed / StepsPerRotation) * MMperRev;

    if (ActualSpeed > MaxSpeed)
        MaxSpeed = ActualSpeed;
    if (ActualSpeed == 0)
        MaxSpeed = 0;

    char buf[32];
    StaticJsonDocument<1024> data;
    JsonArray array = data.to<JsonArray>();
    addDataItem(&array, "led", led ? "ON" : "OFF");
    addDataItem(&array, "actAmp", itoa(driver.cs2rms(driver.cs_actual()), buf, 10));
    addDataItem(&array, "maxAmp", itoa(AMPS, buf, 10));

    char json[1024];
    serializeJson(array, json);
    websocket.broadcastTXT(json);
}

void MotorTask(void *)
{
    while (true)
    {
        bool blocking = true;

        unsigned long timeIs = millis();
        stepper->moveTo(moveMM / (MMperRev / StepsPerRotation), blocking); // TRUE makes this a blocking function. Remove it to use it as non blocking.
        howLong = millis() - timeIs;

        vTaskDelay(2000);

        stepper->moveTo(0, blocking); //

        vTaskDelay(2000);
    }
}