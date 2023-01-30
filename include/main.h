#ifndef __MAIN__
#define __MAIN__

typedef struct
{
    bool led;
    uint microSteps;
    float rpm;
    uint speed;
} Data;

void setMotorRPM(float value);
void setMotoruSteps(uint value);
void sendData();

#endif