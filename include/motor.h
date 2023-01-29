#ifndef __MOTOR__
#define __MOTOR__

#include <TMC2209.h>

typedef unsigned int uint;

enum MICROSTEPS
{
    _1 = 1,
    _2 = 2,
    _4 = 4, 
    _8 = 8, 
    _16 = 16,
    _32 = 32, 
    _64 = 64, 
    _128 = 128, 
    _256 = 256
};

class Motor
{
public:
    Motor();
    uint getSteps();
    uint getMicroSteps();
    void setMicroSteps(MICROSTEPS value);
    float getRPM();
    void setRPM(float value);
    // uint getAcceleration();
    // void setAcceleration(uint value);
    uint getSpeed();
    void setSpeed(uint value);

private:
    TMC2209 driver;
    uint baseSteps;
    uint microSteps;
    uint speed;
    uint acceleration;
};

#endif