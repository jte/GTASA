#include "StdInc.h"

static CVector2D gCoronasViewPos;
static RwTexture *gpCoronaTexture[9];

void CCoronas::DoSunAndMoon()
{
    float rayEnd = CDraw::GetFarClip() * 0.95;
    CVector vectorToSun = CTimeCycle::GetCurrentVectorToSun();
    CVector ray = vectorToSun * rayEnd;
    CVector rayPos = TheCamera.GetPos() + ray;
    if (!CGame::CanSeeOutSideFromCurrArea())
    {
        return;
    }
    if (ray.z > -0.1f)
    {
        RegisterCorona(1,  0,
                SLOBYTE(CTimeCycle::GetCurrentColours().m_sunCoreRed),
                SLOBYTE(CTimeCycle::GetCurrentColours().m_sunCoreGreen),
                SLOBYTE(CTimeCycle::GetCurrentColours().m_sunCoreBlue),
                255, rayPos, 
                CTimeCycle::GetCurrentColours().m_sunSize * 2.7335f,
                999999.88, gpCoronaTexture[0], 0,  0, 0,
                0, 0, 0, 0x3FC00000, 0, 0x41700000, 0, 0);
        if (ray.z > 0.0 && CGame::GetCurrentArea() == 0)
        {
            RegisterCorona(2, 0,
                    SLOBYTE(CTimeCycle::GetCurrentColours().m_sunCoronaRed),
                    SLOBYTE(CTimeCycle::GetCurrentColours().m_sunCoronaGreen),
                    SLOBYTE(CTimeCycle::GetCurrentColours().m_sunCoronaBlue),
                    255, rayPos, 
                    CTimeCycle::GetCurrentColours().m_sunSize * 6.0,
                    999999.88, gpCoronaTexture[0],
                    1, 0, 1,  0,
                    0, 0, 1069547520, 0, 1097859072, 0,  0);
        }
    }
    CVector viewPos;
    float screenX, screenY;
    if (CSprite::CalcScreenCoors(rayPos, &viewPos, &screenX, &screenY, true, true))
    {
        gCoronasViewPos.x = viewPos.x;
        gCoronasViewPos.y = viewPos.y;
    }
    else
    {
        gCoronasViewPos.x = 1000000.0f;
        gCoronasViewPos.y = 1000000.0f;
    }
}