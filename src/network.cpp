#include <Arduino.h>
#include <WiFi.h>

// Local wifi
const char localSSID[] = "zeniton";
const char password[] = "CT33KATPRbaXN";

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
