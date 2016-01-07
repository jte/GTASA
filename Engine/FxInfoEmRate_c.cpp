#include "StdInc.h"

void FxInfoEmRate_c::Load(int file,int)
{
    m_interpInfo.Load(file);
}

//void FxInfoEmRate_c::GetValue(float,float,float,float,uint8_t,void *)
void FxInfoEmRate_c::GetValue(float fTime, float a3, float timeStep, float length, uint8_t a6, void *a7)
{
    EmissionInfo_t* emInfo = (EmissionInfo_t*)a7;
    float rate;
    float v10 = fTime - timeStep;
    m_interpInfo.GetVal(&rate, fTime);
    if (a6)
    {
        emInfo->emissionRate = rate * timeStep;
    }
    else
    {
        if (v10 >= 0.0f)
        {
            emInfo->emissionRate = m_interpInfo.GetVal(0, fTime, timeStep);
        }
        else
        {
            float negT = -v10;
            emInfo->emissionRate = m_interpInfo.GetVal(0, length, negT);
            emInfo->emissionRate += m_interpInfo.GetVal(0, fTime, v10 + timeStep);
        }
    }
}