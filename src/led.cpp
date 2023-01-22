#include <Arduino.h>
#include "led.h"

#define LED_PIN 2

bool led;

void setupLED()
{
    pinMode(LED_PIN, OUTPUT);
}

void toggleLED()
{
    digitalWrite(LED_PIN, led ? LOW : HIGH);
}

bool getLED()
{
    led = digitalRead(LED_PIN) == HIGH;
    return led;
}