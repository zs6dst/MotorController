#include "sdcard.h"
#include "SD.h"

void setFilename(char *);

SDCard::SDCard()
{
    ok = false;
}

void SDCard::diagnose()
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

    setFilename(fileName);
    Serial.printf("SD card log name: %s\n", fileName);

    ok = true;
    return;
}

void SDCard::log(float rpm, char *weight)
{
    if (!ok)
        return;

    File file = SD.open(fileName, FILE_APPEND);
    if (!file)
        return;

    unsigned long timestamp = millis();
    if (weight)
        file.printf("%lu,%6.1f,%s\n", timestamp, rpm, weight);
    else
        file.printf("%lu,%6.1f,\n", timestamp, rpm);

    file.close();
}

void setFilename(char *fileName)
{
    sprintf(fileName, "/%lu.csv", millis());
}
