#include <Arduino.h>
#include <WiFi.h>
#include "network.h"

#define USELOCALWIFI true

// Local wifi
static const char localSSID[] = "zeniton";
static const char password[] = "CT33KATPRbaXN";

#define AP_IP 10.1.1.1

void connectToWiFi();
void createAP();

void setupWiFi()
{
    USELOCALWIFI ? connectToWiFi() : createAP();
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

void createAP()
{
    Serial.println("Creating soft AP...");
    //TODO
    Serial.printf("Soft AP created. IP: %s\n", "<put IP here>");
}
