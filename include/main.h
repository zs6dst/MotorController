#ifndef __MAIN__
#define __MAIN__

typedef unsigned int uint;

typedef struct
{
    bool led;
    bool stealthChop;
    uint microSteps;
    float rpm;
    uint speed;
    uint acceleration;
} Data;

void setMotorRPM(float value);
void setMotoruSteps(uint value);
void setMotorAcceleration(uint value);
void sendData();

#endif