#ifndef __MAIN__
#define __MAIN__

typedef struct
{
    bool led;
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