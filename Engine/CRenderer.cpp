#include "StdInc.h"

void CRenderer::RenderRoads()
{
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)1);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)1);
    RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)2);
    DeActivateDirectional();
    SetAmbientColours();
    for(size_t i = 0; i < CRenderer::ms_nNoOfVisibleEntities; i++)
    {
        CEntity *pEntity = CRenderer::ms_aVisibleEntityPtrs[i];
        if(pEntity->m_nType == ENTITY_TYPE_BUILDING)
        {
            if(CModelInfo::ms_modelInfoPtrs[pEntity->m_nModelIndex]->bIsRoad)
            {
                if(CPostEffects::IsVisionFxActive())
                {
                    CPostEffects::FilterFX_StoreAndSetDayNightBalance();
                    pEntity->Render();
                    CPostEffects::FilterFX_RestoreDayNightBalance();
                }
                else
                {
                    pEntity->Render();
                }
            }
        }
    }
}

void CRenderer::Init()
{
}

void CRenderer::Shutdown()
{
}

void CRenderer::RenderFadingInEntities()
{
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)1);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)1);
    RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)2);
    DeActivateDirectional();
    SetAmbientColours();
    CVisibilityPlugins::RenderFadingEntities();
}