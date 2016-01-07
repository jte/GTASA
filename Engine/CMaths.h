#pragma once

// min resolution of sine lookup table
#define SINE_STEP (M_PI / 128.0f)

class CMaths
{
public:
    static void InitMathsTables();
    static float GetSin(size_t step);
private:
    static float ms_SinTable[256];
};