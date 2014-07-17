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

void CPostEffects::DrawQuad(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a, RwRaster* raster)
{
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void*)raster);
    ms_imf[0].x = x;
    ms_imf[0].y = y;
    ms_imf[1].x = v10;
    ms_imf[1].y = y;
    ms_imf[2].y = y + height;
    ms_imf[2].x = x;
    ms_imf[3].x = x + width;
    ms_imf[3].y = y + height;
    ms_imf[0].emissiveColor = D3DCOLOR_RGBA(r,g,b,a);
    ms_imf[1].emissiveColor = D3DCOLOR_RGBA(r,g,b,a);
    ms_imf[2].emissiveColor = D3DCOLOR_RGBA(r,g,b,a);
    ms_imf[3].emissiveColor = D3DCOLOR_RGBA(r,g,b,a);
    ms_imf[0].z = flt_C40150;
    ms_imf[1].z = flt_C40150;
    ms_imf[2].z = flt_C40150;
    ms_imf[3].z = flt_C40150;
    RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, ms_imf, 4);
}

void CPostEffects::Initialise()
{
    SetupBackBufferVertex();
    m_pGrainRaster = RwRasterCreate(256, 256, 32, rwRASTERFORMAT8888 | rwRASTERTYPETEXTURE);
    RwRGBA* pixels = RwRasterLock(m_pGrainRaster, 0, 1);
    size_t curr = 256 * 256;
    while(curr)
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