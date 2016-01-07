#include "StdInc.h"

static RwBlendFunction gStoredRenderStateSrcBlend;
static RwBlendFunction gStoredRenderStateDestBlend;
static bool gStoredRenderStateFogEnable;
static RwCullMode gStoredRenderStateCullMode;
static bool gStoredRenderStateZTestEnable;
static bool gStoredRenderStateZWriteEnable;
static RwShadeMode gStoredRenderStateShadeMode;
static bool gStoredRenderStateVertexAlphaEnable;
static RwTextureAddressMode gStoredRenderStateTextureAddress;
static RwTextureFilterMode gStoredRenderStateTextureFilter;

void CPostEffects::NightVisionSetLights()
{
    if (m_bNightVision && !m_bInCutscene)
    {
            SetLightsForNightVision();
    }
}

bool CPostEffects::IsVisionFXActive()
{
    return !m_bInCutscene && (m_bNightVision || m_bInfraredVision);
}

void CPostEffects::InfraredVisionSetLightsForDefaultObjects()
{
    if (m_bInfraredVision && !m_bInCutscene)
    {
        SetLightsForInfraredVisionDefaultObjects();
    }
}

void CPostEffects::DrawQuad(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a, RwRaster* raster)
{
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void*)raster);

    ms_imf[0].x = x;
    ms_imf[0].y = y;
    ms_imf[0].z = flt_C40150;
    ms_imf[0].emissiveColor = D3DCOLOR_RGBA(r,g,b,a);

    ms_imf[1].x = x + width;
    ms_imf[1].y = y;
    ms_imf[1].z = flt_C40150;
    ms_imf[1].emissiveColor = D3DCOLOR_RGBA(r,g,b,a);

    ms_imf[2].y = y + height;
    ms_imf[2].x = x;
    ms_imf[2].z = flt_C40150;
    ms_imf[2].emissiveColor = D3DCOLOR_RGBA(r,g,b,a);

    ms_imf[3].x = x + width;
    ms_imf[3].y = y + height;
    ms_imf[3].z = flt_C40150;
    ms_imf[3].emissiveColor = D3DCOLOR_RGBA(r,g,b,a);
    
    RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, ms_imf, 4);
}

void CPostEffects::Initialise()
{
    SetupBackBufferVertex();
    m_pGrainRaster = RwRasterCreate(256, 256, 32, rwRASTERFORMAT8888 | rwRASTERTYPETEXTURE);
    RwRGBA* pixels = (RwRGBA*)RwRasterLock(m_pGrainRaster, 0, 1);
    size_t curr = 256 * 256;
    while (curr)
    {
        pixels->red = rand();
        pixels->green = pixels->red;
        pixels->blue = pixels->red;
        pixels->alpha = pixels->red;
        pixels += sizeof(*pixels);
        curr -= 4;
    }
    RwRasterUnlock(m_pGrainRaster);
}

void CPostEffects::ImmediateModeRenderStatesStore()
{
    RwRenderStateGet(rwRENDERSTATESRCBLEND, &gStoredRenderStateSrcBlend);
    RwRenderStateGet(rwRENDERSTATEDESTBLEND, &gStoredRenderStateDestBlend);
    RwRenderStateGet(rwRENDERSTATEFOGENABLE, &gStoredRenderStateFogEnable);
    RwRenderStateGet(rwRENDERSTATECULLMODE, &gStoredRenderStateCullMode);
    RwRenderStateGet(rwRENDERSTATEZTESTENABLE, &gStoredRenderStateZTestEnable);
    RwRenderStateGet(rwRENDERSTATEZWRITEENABLE, &gStoredRenderStateZWriteEnable);
    RwRenderStateGet(rwRENDERSTATESHADEMODE, &gStoredRenderStateShadeMode);
    RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, &gStoredRenderStateVertexAlphaEnable);
    RwRenderStateGet(rwRENDERSTATETEXTUREADDRESS, &gStoredRenderStateTextureAddress);
    RwRenderStateGet(rwRENDERSTATETEXTUREFILTER, &gStoredRenderStateTextureFilter);
}

void CPostEffects::ScriptResetForEffects()
{
    m_bNightVision = false;
    m_bInfraredVision = false;
    m_bHeatHazeFX = false;
    m_bDarknessFilter = false;
    m_bCCTV = false;
    CWaterLevel::m_bWaterFogScript = true;
}

void CPostEffects::ScriptNightVisionSwitch(bool enable)
{
    if (enable)
    {
        m_bNightVision = true;
        m_bInfraredVision = false;
    }
    else
    {
        CPostEffects::m_bNightVision = false;
    }
}

void CPostEffects::ScriptInfraredVisionSwitch(bool enable)
{
    if (enable)
    {
        m_bInfraredVision = true;
        m_bNightVision = false;
    }
    else
    {
        m_bInfraredVision = false;
    }
}

void CPostEffects::ScriptHeatHazeFXSwitch(bool enable)
{
    m_bHeatHazeFX = enable;
}

void CPostEffects::ScriptDarknessFilterSwitch(bool enable, int alpha)
{
    m_bDarknessFilter = enable;
    if (alpha == 255)
    {
        m_DarknessFilterAlpha = m_DarknessFilterAlphaDefault;
    }
    else
    {
        m_DarknessFilterAlpha = Clamp(0, alpha, 255);
    }
}

void CPostEffects::ScriptCCTVSwitch(bool enable)
{
    m_bCCTV = enable;
}

void CPostEffects::NightVision(RwRGBA a1)
{RwIm2dVertex
    if (m_fNightVisionSwitchOnFXCount > 0.0f)
    {
        m_fNightVisionSwitchOnFXCount -= CTimer::GetTimeStep();
        m_fNightVisionSwitchOnFXCount = ClampMin(m_fNightVisionSwitchOnFXCount, 0.0f);
        ImmediateModeRenderStatesStore();
        ImmediateModeRenderStatesSet();
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDONE);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDONE);
        for (size_t i = 0, end = m_fNightVisionSwitchOnFXCount; i < end; i++)
        {
        v5 = (double)dword_C40160;
        v6 = (double)dword_C4015C;
        CPostEffects::DrawQuad(0, 0, v6, v5, 8, 8u, 8u, -1, ms_imf);
        }
        ImmediateModeRenderStatesReStore();
    }
    ImmediateModeRenderStatesStore();
    ImmediateModeRenderStatesSet();
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDZERO);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDSRCCOLOR);
    DrawQuad(0, 0, RsGlobal.maximumWidth, RsGlobal.maximumHeight, 32, 255, 32, 255, NULL);
    ImmediateModeRenderStatesReStore();
}