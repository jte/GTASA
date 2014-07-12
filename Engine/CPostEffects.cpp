#include "StdInc.h"

void CPostEffects::NightVisionSetLights()
{
    if(m_bNightVision)
    {
        if(!m_bInCutscene)
        {
            SetLightsForNightVision();
        }
    }
}

bool CPostEffects::IsVisionFXActive()
{
    return !m_bInCutscene && (m_bNightVision || m_bInfraredVision);
}

void CPostEffects::InfraredVisionSetLightsForDefaultObjects()
{
    if(m_bInfraredVision)
    {
        if(!m_bInCutscene)
        {
            SetLightsForInfraredVisionDefaultObjects();
        }
    }
}