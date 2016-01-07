#pragma once

#include "FxInfo_c.h"
#include "FxInterpInfoFloat_c.h"

class FxInfoEmRate_c : protected FxInfo_c
{
public:
    virtual ~FxInfoEmRate_c();
    virtual void Load(int file,int);
    virtual void GetValue(float,float,float,float,uint8_t,void *);
private:
    FxInterpInfoFloat_c m_interpInfo;
};