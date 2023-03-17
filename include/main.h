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
} Data_t;

typedef struct
{
    unsigned int duration;
    float rpm;
} Step_t;

#endif