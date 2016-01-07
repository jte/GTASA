#pragma once

#include "FxInfo_c.h"

class FxInterpInfo_c
{
public:
    // virtual methods
    virtual ~FxInterpInfo_c();
    virtual void Load(int file) = 0;
    //
    FxInterpInfo_c();
protected:
    bool m_looped;
    uint8_t m_numKeys;
    uint8_t m_numValues;
    uint8_t m_pad1;
    FixedPoint* m_timeArray;//_C=256
};