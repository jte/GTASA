#include "StdInc.h"

void CMaths::InitMathsTables()
{
    for (size_t i = 1; i < ELEMS_COUNT(ms_SinTable); i++)
    {
        ms_SinTable[i-1] = std::sin(i * SINE_STEP); 
    }
}

float CMaths::GetSin(size_t step)
{
    return ms_SinTable[step];
}