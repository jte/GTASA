#include "StdInc.h"

void CRegisteredCorona::Update()
{
    if (!RegisteredThisFrame)
    {
        Intensity = 0;
    }
    if (bIsAttachedToEntity)
    {
        if (!pEntityAttachedTo)
        {
            Identifier = 0;
            CCoronas::NumCoronas--;
        }
    }
    if (!LOSCheck || (!CCoronas::SunBlockedByClouds || Identifier != 2) && 
       (CWorld::GetIsLineOfSightClear(Coordinates, TheCamera.GetPos(), 1, 0, 0, 0, 0, 0i64)) &&
       !((OffScreen || OnlyFromBelow && (TheCamera.GetPos().z > Coordinates.z)))
       )
    {
        if (CCoronas::bChangeBrightnessImmediately)
        {
            FadedIntensity = Intensity;
        }
        else if (Intensity > FadedIntensity)
        {
            FadedIntensity = ClampMax<uint8_t>(Intensity, FadedIntensity + CTimer::GetTimeStep() * FadeSpeed);
        }
        else if (Intensity < FadedIntensity)
        {
            FadedIntensity = ClampMin<uint8_t>(Intensity, FadedIntensity - CTimer::GetTimeStep() * FadeSpeed);
        }
        if (Identifier == 2)
        {
            CCoronas::ScreenMult = ClampMin(CCoronas::ScreenMult - CTimer::GetTimeStep() * 0.06f, 0.6f);
        }
    }
    else
    {
        FadedIntensity = ClampMin(FadedIntensity - CTimer::GetTimeStep() * FadeSpeed, 0.0f);
    }
    if (!FadedIntensity && !JustCreated)
    {
        Identifier = 0;
        CCoronas::NumCoronas--;
    }
    JustCreated = 0;
    RegisteredThisFrame = 0;
}