#include "StdInc.h"

void CRenderer::RenderRoads()
{
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
    DeActivateDirectional();
    SetAmbientColours();
    for(size_t i = 0; i < ms_nNoOfVisibleEntities; i++)
    {
        CEntity *pEntity = ms_aVisibleEntityPtrs[i];
        if(pEntity->GetType() == ENTITY_TYPE_BUILDING)
        {
            if(CModelInfo::GetModelInfo(pEntity->GetModelIndex())->GetIsRoad())
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
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
    DeActivateDirectional();
    SetAmbientColours();
    CVisibilityPlugins::RenderFadingEntities();
}

void CRenderer::RenderOneNonRoad(CEntity* entity)
{
    CPed* ped = (CPed*)entity;
    if(entity->GetType() != ENTITY_TYPE_PED || (entity->GetType() == ENTITY_TYPE_PED && ped->GetState() != PED_DRIVING))
    {
        entity->SetupLighting();
        if(entity->GetType() == ENTITY_TYPE_VEHICLE)
        {
            CVehicle* vehicle = (CVehicle*)entity;
            CVisibilityPlugins::SetupVehicleVariables((RpClump*)entity->GetRwObject());
            CVisibilityPlugins::InitAlphaAtomicList();
            vehicle->RenderDriverAndPassengers();
            vehicle->SetupRender();
        }
        else
        {
            if(!entity->GetIsBackfaceCulled())
            {
                RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
            }
        }
        if(CPostEffects::IsVisionFXActive())
        {
            if(CPostEffects::m_bNightVision)
            {
                CPostEffects::NightVisionSetLights();
            }
            if(CPostEffects::m_bInfraredVision )
            {
                CPostEffects::InfraredVisionSetLightsForDefaultObjects();
            }
            CPostEffects::FilterFX_StoreAndSetDayNightBalance();
            entity->Render();
            CPostEffects::FilterFX_RestoreDayNightBalance();
        }
        else
        {
            entity->Render();
        }
        if(entity->GetType() == ENTITY_TYPE_VEHICLE)
        {
            CVehicle* vehicle = (CVehicle*)entity;
            entity->SetIsBeingRendered(true);
            CVisibilityPlugins::RenderAlphaAtomics();
            entity->SetIsBeingRendered(false);
            vehicle->ResetAfterRender();
        }
        else
        {
            if(!entity->GetIsBackfaceCulled())
            {
                RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
            }
        }
        entity->RemoveLighting();
    }
}