#include <Arduino.h>
#include <WiFi.h>
#include "network.h"

#define USELOCALWIFI false
#define AP_IP 10.1.1.1

// Local wifi
static const char localSSID[] = "zeniton";
static const char password[] = "CT33KATPRbaXN";

//AP
static const char SSID[] = "AutoReloader";
static const IPAddress IP = IPAddress(10, 1, 1, 1);

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
    WiFi.mode(WIFI_AP);
    WiFi.softAP(SSID);
    WiFi.softAPConfig(IP, IP, IPAddress(255, 255, 255, 0));
    Serial.print("Soft AP created at ");
    Serial.println(IP);
}
 