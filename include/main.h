#ifndef __MAIN__
#define __MAIN__

typedef struct
{
    bool led;
    float rpm;
    uint speed;
} Data;

void setMotorRPM(float value);
void sendData();

#endif