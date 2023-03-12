#include "SD.h"

static char logName[128];

void setupSDCard()
{
    if (!SD.begin(5))
    {
        Serial.println("SD card mount failed");
        return;
    }

    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("SD Card Type: ");
    if (cardType == CARD_MMC)
        Serial.println("MMC");
    else if (cardType == CARD_SD)
        Serial.println("SDSC");
    else if (cardType == CARD_SDHC)
        Serial.println("SDHC");
    else
        Serial.println("UNKNOWN");

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);

    sprintf(logName, "/%lu.csv", millis());
    Serial.printf("SD card log: %s\n", logName);
    return;
}

void logSD(float rpm, char *weight)
{
    File file = SD.open(logName, FILE_APPEND);
    if (!file)
        return;

    unsigned long timestamp = millis();
    if (weight)
        file.printf("%lu,%6.1f,%s\n", timestamp, rpm, weight);
    else
        file.printf("%lu,%6.1f,\n", timestamp, rpm);

    file.close();
}
