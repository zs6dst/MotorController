#include <Arduino.h>

#define LED_PIN 2

void setupLED()
{
    pinMode(LED_PIN, OUTPUT);
}

int getLED()
{
    return digitalRead(LED_PIN);
}

void toggleLED()
{
    digitalWrite(LED_PIN, getLED() ? LOW : HIGH);
}