#pragma once

// http://www.superkits.net/whitepapers/Fixed%20Point%20Representation%20&%20Fractional%20Math.pdf

typedef uint16_t FixedPoint;
typedef uint8_t FixedPoint8;

template<int _C/* = 128*/>
const float FixedPointToFloat(const FixedPoint fp)
{
    return fp / _C;
}

template<int _C/* = 128*/>
const FixedPoint FloatToFixedPoint(const float f)
{
    return f * _C;
}