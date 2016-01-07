#include "StdInc.h"

void FxPrtMult_c::SetUp(float r, float g, float b, float a, float size, float a7, float durationFactor)
{
    m_colour.red = r;
    m_colour.green = g;
    m_colour.blue = b;
    m_colour.alpha = a;
    m_size = size;
    pad1 = a7;
    m_durationFactor = durationFactor;
}

FxPrtMult_c::FxPrtMult_c()
{
    SetUp(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

FxPrtMult_c::FxPrtMult_c(float r, float g, float b, float a, float size, float a7, float durationFactor)
{
    SetUp(r, g, b, a, size, a7, durationFactor);
}