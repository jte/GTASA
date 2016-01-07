#include "StdInc.h"

float gfLaRiotsLightMult;

float CTimeCycle::GetAmbientRed()
{
    return m_CurrentColours.m_ambientRed * gfLaRiotsLightMult;
}

float CTimeCycle::GetAmbientGreen()
{
    return m_CurrentColours.m_ambientGreen * gfLaRiotsLightMult;
}

float CTimeCycle::GetAmbientBlue()
{
    return m_CurrentColours.m_ambientBlue * gfLaRiotsLightMult;
}

void CTimeCycle::Initialise()
{
    CFileMgr::SetDir("DATA");
    int file = CFileMgr::OpenFile("TIMECYC.DAT", "rb");
    CFileMgr::SetDir("");
    // 23 entries for weather types
    for (size_t i = 0; i < 24; i++)
    {
        // 8 time divisons (per clock)
        for (size_t j = 0; j < 8; j++)
        {
            const char* line;
            for (line = CFileLoader::LoadLine(file); line; line = CFileLoader::LoadLine(file) )
            {
                if (line[0] != '/' && line[0])
                {
                    break;
                }
            }
            uint8_t dummy;
            sscanf(line, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %f %f %f %d %d %d %f %f %f %d %d %d %d %d %d %f %f %f %f  %f %f %f %f  %f %f %f %f %f %d %d %f",
                &m_nAmbientRed[i][j], &m_nAmbientGreen[i][j], &m_nAmbientBlue[i][j],
                &m_nAmbientRed_Obj[i][j], &m_nAmbientGreen_Obj[i][j], &m_nAmbientBlue_Obj[i][j],
                &dummy, &dummy, &dummy, // unused "direct light color"
                &m_nSkyTopRed[i][j], &m_nSkyTopGreen[i][j], &m_nSkyTopBlue[i][j],
                &m_nSkyBottomRed[i][j], &m_nSkyBottomGreen[i][j], &m_nSkyBottomBlue[i][j],
                &m_nSunCoreRed[i][j], &m_nSunCoreGreen[i][j], &m_nSunCoreBlue[i][j],
                &m_nSunCoronaRed[i][j], &m_nSunCoronaGreen[i][j], &m_nSunCoronaBlue[i][j],
                &m_fSunSize[i][j], &m_fSpriteSize[i][j], &m_fSpriteBrightness[i][j],
                &m_nShadowStrength[i][j], &m_nLightShadowStrength[i][j], &m_nPoleShadowStrength[i][j],
                &m_fFarClip[i][j], &m_fFogStart[i][j],
                &m_fLightsOnGroundBrightness[i][j],
                &m_nLowCloudsRed[i][j], &m_nLowCloudsGreen[i][j], &m_nLowCloudsBlue[i][j],
                &m_nFluffyCloudsBottomRed[i][j], &m_nFluffyCloudsBottomGreen[i][j], &m_nFluffyCloudsBottomBlue[i][j],
                &m_fWaterRed[i][j], &m_fWaterGreen[i][j], &m_fWaterBlue[i][j], &m_fWaterAlpha[i][j],
                &m_fPostFx1Alpha[i][j], &m_fPostFx1Red[i][j], &m_fPostFx1Green[i][j], &m_fPostFx1Blue[i][j],
                &m_fPostFx2Alpha[i][j], &m_fPostFx2Red[i][j], &m_fPostFx2Green[i][j], &m_fPostFx2Blue[i][j],
                &m_fCloudAlpha[i][j], &m_nHighLightMinIntensity[i][j], &m_nWaterFogAlpha[i][j],
                &byte_B79FD8[i][j]);
        
                m_fSunSize[i][j] = m_fSunSize[i][j] * 10.0f + 0.5f;
                m_fSpriteSize[i][j] = m_fSpriteSize[i][j] * 10.0f + 0.5f;
                m_fSpriteBrightness[i][j] = m_fSpriteBrightness[i][j] * 10.0f + 0.5f;
                m_fLightsOnGroundBrightness[i][j] = m_fLightsOnGroundBrightness[i][j] * 10.0f + 0.5f;
                m_fPostFx1Alpha[i][j] *= 2;
                m_fPostFx2Alpha[i][j] *= 2;
                byte_B79FD8[i][j] *= 100;
         
        }
    }
    CFileMgr::CloseFile(file);
    m_vecDirnLightToSun.x = cos(-2.356194496154785) * cos(0.7853981852531433);
    m_vecDirnLightToSun.y = sin(-2.356194496154785) * cos(0.7853981852531433);
    m_vecDirnLightToSun.z = sin(0.7853981852531433);
    m_vecDirnLightToSun.Normalise();
    m_FogReduction = 0;
    m_bExtraColourOn = 0;
}

void CTimeCycle::StartExtraColour(int id, bool noInterpol)
{
    m_ExtraColourWeatherType = id / 8 + 21;
    m_ExtraColour = id % 8;
    m_bExtraColourOn = true;
    m_ExtraColourInter = noInterpol ? 0 : 1.0f;
}

void CTimeCycle::StopExtraColour(bool noInterpol)
{
    m_bExtraColourOn = false;
    if (!noInterpol)
    {
        m_ExtraColourInter = 0;
    }
}

void CTimeCycle::AddOne(CBox& a1, short a2, int a3, float a4, float a5, float a6)
{
    m_aBoxes[m_NumBoxes].box = a1;
    m_aBoxes[m_NumBoxes].field_18 = a2;
    m_aBoxes[m_NumBoxes].field_1C = a3;
    m_aBoxes[m_NumBoxes].field_20 = a4 * 0.0099999998;
    m_aBoxes[m_NumBoxes].field_24 = a5;
    m_aBoxes[m_NumBoxes].field_1A = ClampMax(a6, 4.0f) * 32.0;
    m_NumBoxes++;
}

void CTimeCycle::InitForRestart()
{
    m_bExtraColourOn = false;
    m_ExtraColourInter = 0.0f;
}

void CTimeCycle::SetConstantParametersForPostFX()
{
    if (!CPostEffects::IsVisionFXActive())
    {
        return;
    }
    if (CPostEffects::m_bNightVision)
    {
        m_CurrentColours.m_shadowStrength = 0;
        m_CurrentColours.m_lightShadowStrength = 0;
        m_CurrentColours.m_poleShadowStrength = 0;
        m_CurrentColours.m_ambientRed = 0;
        m_CurrentColours.m_ambientGreen = 0.4f;
        m_CurrentColours.m_ambientBlue = 0;
        m_CurrentColours.m_ambientRedObj = 0;
        m_CurrentColours.m_ambientGreenObj = 0.4f;
        m_CurrentColours.m_ambientBlueObj = 0;
        m_CurrentColours.m_skyTopRed = 0;
        m_CurrentColours.m_skyTopGreen = 128;
        m_CurrentColours.m_skyTopBlue = 0;
        m_CurrentColours.m_skyBottomRed = 0;
        m_CurrentColours.m_skyBottomGreen = 128;
        m_CurrentColours.m_skyBottomBlue = 0;
    }
    if (CPostEffects::m_bInfraredVision)
    {
        m_CurrentColours.m_shadowStrength = 0;
        m_CurrentColours.m_lightShadowStrength = 0;
        m_CurrentColours.m_poleShadowStrength = 0;
        m_CurrentColours.m_lightsOnGroundBrightness = 0;
        m_CurrentColours.m_highLightMinIntensity = 0;
        m_CurrentColours.field_A0 = 0;
        m_CurrentColours.m_ambientRed = 0;
        m_CurrentColours.m_ambientGreen = 0;
        m_CurrentColours.m_ambientBlue = 1.0f;
        m_CurrentColours.m_ambientRedObj = 0;
        m_CurrentColours.m_ambientGreenObj = 0;
        m_CurrentColours.m_ambientBlueObj = 1.0f;
        m_CurrentColours.m_skyTopRed = 0;
        m_CurrentColours.m_skyTopGreen = 0;
        m_CurrentColours.m_skyTopBlue = 128;
        m_CurrentColours.m_skyBottomRed = 0;
        m_CurrentColours.m_skyBottomGreen = 0;
        m_CurrentColours.m_skyBottomBlue = 128;
    }
}

float CTimeCycle::GetAmbientRed_Obj()
{
    return m_CurrentColours.m_ambientRedObj;
}

float CTimeCycle::GetAmbientGreen_Obj()
{
    return m_CurrentColours.m_ambientGreenObj;
}

float CTimeCycle::GetAmbientBlue_Obj()
{
    return m_CurrentColours.m_ambientBlueObj;
}

void CTimeCycle::Update()
{
    CalcColoursForPoint(TheCamera.GetPos(), &m_CurrentColours);
}

void CTimeCycle::Shutdown()
{
}

float CTimeCycle::FindFarClipForCoors(CVector coors)
{
    bool savedExtraColourOn = m_bExtraColourOn;
    float savedExtraColourInter = m_ExtraColourInter;
    m_bExtraColourOn = false;
    m_ExtraColourInter = 0;
    CColourSet v6;
    CalcColoursForPoint(coors, &v6);
    m_bExtraColourOn = savedExtraColourOn;
    m_ExtraColourInter = savedExtraColourInter;
    return v6.m_farClip;
}

CColourSet CTimeCycle::GetCurrentColours()
{
    return m_CurrentColours;
}

CVector CTimeCycle::GetCurrentVectorToSun()
{
    return m_VectorToSun[m_CurrentStoredValue];
}