#include "StdInc.h"

void CRenderer::RenderRoads()
{
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
    DeActivateDirectional();
    SetAmbientColours();
    for (size_t i = 0; i < ms_nNoOfVisibleEntities; i++)
    {
        CEntity *pEntity = ms_aVisibleEntityPtrs[i];
        if (pEntity->GetType() == ENTITY_TYPE_BUILDING && CModelInfo::GetModelInfo(pEntity->GetModelIndex())->GetIsRoad())
        {
            if (CPostEffects::IsVisionFxActive())
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
    if (entity->GetType() != ENTITY_TYPE_PED || (entity->GetType() == ENTITY_TYPE_PED && ped->GetState() != PED_DRIVING))
    {
        entity->SetupLighting();
        if (entity->GetType() == ENTITY_TYPE_VEHICLE)
        {
            CVehicle* vehicle = (CVehicle*)entity;
            CVisibilityPlugins::SetupVehicleVariables((RpClump*)entity->GetRwObject());
            CVisibilityPlugins::InitAlphaAtomicList();
            vehicle->RenderDriverAndPassengers();
            vehicle->SetupRender();
        }
        else if (!entity->GetIsBackfaceCulled())
        {
            RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
        }
        if (CPostEffects::IsVisionFXActive())
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
        if (entity->GetType() == ENTITY_TYPE_VEHICLE)
        {
            CVehicle* vehicle = (CVehicle*)entity;
            entity->SetIsBeingRendered(true);
            CVisibilityPlugins::RenderAlphaAtomics();
            entity->SetIsBeingRendered(false);
            vehicle->ResetAfterRender();
        }
        else if (!entity->GetIsBackfaceCulled())
        {
            RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
        }
        entity->RemoveLighting();
    }
}

LodRenderList_t gLodRenderListBase;

LodRenderList_t* CRenderer::GetLodRenderListBase()
{
    return &gLodRenderListBase;
}

void CRenderer::ProcessLodRenderLists()
{
    for (LodRenderList_t* i = &gLodRenderListBase; i != ms_pLodRenderList; i++)
    {
        if (i->entity && !i->entity->IsVisible())
        {
            i->entity->numLodChildrenRendered = 0;
            i->entity = 0;
        }
    }
    bool v15 = 0;
    do
    {
        v15 = 0;
        for (LodRenderList_t* i = GetLodRenderListBase(); i != ms_pLodRenderList; i++)
        {
            if (i->entity)
            {
                if ( (unsigned __int8)i->entity->numLodChildren <= 0u || i->entity->numLodChildrenRendered != i->entity->numLodChildren)
                {
                    CEntity* lod = i->entity->m_pLod;
                    if (lod)
                    {
                        if (CModelInfo::GetModelInfo(i->entity->GetModelIndex())->GetAlpha() != 255
                            && lod->numLodChildrenRendered != 0x80u && lod->bDisplayedSuperLowLOD)
                        {
                            lod->numLodChildrenRendered = 0;
                        }
                        if (!i->entity->GetRwObject())
                        {
                            if (lod->bDisplayedSuperLowLOD)
                            {
                                lod->numLodChildrenRendered = 0x80u;
                            }
                            i->entity = 0;
                            i->entity->numLodChildrenRendered = 0;
                            CStreaming::RequestModel(i->entity->GetModelIndex(), 0);
                        }
                    }
                }
                else
                {
                  v4->numLodChildrenRendered = 0;
                  v3->entity = 0;
                  v15 = 1;
                }
            }
        }
    } while ( v15 );

    for (LodRenderList_t* i = GetLodRenderListBase(); i != CRenderer::ms_pLodRenderList; i++)
    {
        if (i->entity && i->entity->numLodChildrenRendered > 0)
        {
            i->entity->bDisplayedSuperLowLOD = false;
            i->entity->numLodChildrenRendered = 0;
            i->entity = NULL;
        }
    }
    for (LodRenderList_t* i = GetLodRenderListBase(); i != CRenderer::ms_pLodRenderList; i++)
    {
        if (i->entity)
        {
            if (i->entity->numLodChildrenRendered == 0x80 || i->entity->numLodChildrenRendered == 0)
            {
                CBaseModelInfo* modelInfo = CModelInfo::GetModelInfo(i->entity->GetModelIndex());
                i->entity->bDisplayedSuperLowLOD = true;
                if (modelInfo->GetAlpha() != 255)
                {
                    i->entity->bDistanceFade = true;
                }
                CRenderer::AddEntityToRenderList(i->entity, j->distance);
            }
            i->entity->numLodChildrenRendered = 0;
        }
    }
}