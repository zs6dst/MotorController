#ifndef __MAIN__
#define __MAIN__

typedef struct
{
    bool led;
    float rpm;
} Data;

void setMotorRPM(float rpm);

#endif