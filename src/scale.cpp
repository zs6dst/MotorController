#include <Arduino.h>
#include <HardwareSerial.h>

#define SCALE_BAUD 9600
#define SCALE_RX 15
#define SCALE_TX 27

void setupScale(HardwareSerial &scale)
{
    scale.begin(SCALE_BAUD, SERIAL_8N1, SCALE_RX, SCALE_TX);
}

void removeCharacter(char *str, const char c)
{
    int j, n = strlen(str);
    for (int i = j = 0; i < n; i++)
        if (str[i] != c)
            str[j++] = str[i];
    str[j] = '\0';
}

char *getWeight(HardwareSerial &scale, char (&weight)[64])
{
    if (!scale.available()) 
        return NULL;

    uint8_t n = scale.readBytesUntil('\n', weight, 64);
    removeCharacter(weight, '+');
    removeCharacter(weight, ' ');
    return weight;
}

