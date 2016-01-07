#pragma once

#include "FixedPoint.h"
#include "FxInterpInfo_c.h"

class FxInterpInfoFloat_c : protected FxInterpInfo_c
{
public:
    virtual ~FxInterpInfoFloat_c();
    virtual void Load(int fp);
    //
    void GetVal(float* values, float time);
	float GetVal(int startValue, float, float);
    FxInterpInfoFloat_c();
private:
    float** m_valueArray;
};