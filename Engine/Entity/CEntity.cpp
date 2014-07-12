#include "StdInc.h"

void CEntity::Add(const CRect& rect)
{
    CRect sector = rect;
    sector.left = ClampMin(sector.left, -3000.0f);
    sector.right = ClampMax(sector.right, 2999.0f);
    sector.bottom = ClampMin(sector.bottom, -3000.0f);
    // NOTE: R* checks for > 3000.0f bound and clamps to 2999.0f
    sector.top = ClampMax(sector.top, 2999.0f);
    if(m_isBigBuilding)
    {
#pragma message("CEntity::Add(const CRect& rect) TODO")
    }
    else
    {
        sector.left = floor(lrect.left * GRID_UNIT + GRID_OFFSET);
        sector.bottom = floor(lrect.bottom * GRID_UNIT + GRID_OFFSET);
        sector.right = floor(lrect.right * GRID_UNIT + GRID_OFFSET);
        sector.top = floor(lrect.top * GRID_UNIT + GRID_OFFSET);
        // Scan bottom to top
        for(size_t i = sector.bottom; i <= sector.top; i++)
        {
            // Scan left to right
            for(size_t j = sector.left; j <= sector.right; j++)
            {
                size_t iclamp;
                size_t jclamp;
                switch(m_type)
                {
                    case ENTITY_TYPE_BUILDING:
                    {
                        iclamp = Clamp(0.0f, sector.left, 119.0f);
                        jclamp = Clamp(0.0f, sector.bottom, 119.0f);
                        CWorld::ms_aSectors[iclamp + jclamp * GRID_MAX_UNITS].ptr = new CPtrNodeSingleLink(this);
                    }
                    break;
                    case ENTITY_TYPE_VEHICLE:
                        iclamp = i % GRID_CELL_SIZE;
                        jclamp = j % GRID_CELL_SIZE;
                        CWorld::ms_aRepeatSectors[iclamp + GRID_CELL_SIZE * jclamp].ptr = new CPtrNodeDoubleLink(this);
                    break;
                    case ENTITY_TYPE_PED:
                        iclamp = i % GRID_CELL_SIZE;
                        jclamp = j % GRID_CELL_SIZE;
                        CWorld::ms_aRepeatSectors[iclamp + GRID_CELL_SIZE * jclamp].next = new CPtrNodeDoubleLink(this);
                    break;
                    case ENTITY_TYPE_OBJECT:
                        iclamp = i % GRID_CELL_SIZE;
                        jclamp = j % GRID_CELL_SIZE;
                        CWorld::ms_aRepeatSectors[iclamp + GRID_CELL_SIZE * jclamp].prev = new CPtrNodeDoubleLink(this);
                    break;
                    case ENTITY_TYPE_DUMMY:
                        iclamp = Clamp(0.0f, sector.left, 119.0f);
                        jclamp = Clamp(0.0f, sector.bottom, 119.0f);
                        CWorld::ms_aSectors[iclamp + jclamp * GRID_MAX_UNITS].next = new CPtrNodeSingleLink(this);
                    break;
                }
            }
        }
    }
}

void CEntity::UpdateRpHAnim()
{
    RpAtomic *atomic = GetFirstAtomic(m_rwObject);
    if(atomic && RpSkinGeometryGetSkin(atomic->geometry))
    {
        if(!m_dontUpdateHierarchy)
        {
            RpHAnimHierarchy *animHierarchy = GetAnimHierarchyFromSkinClump(m_rwObject);
            RpHAnimHierarchyUpdateMatrices(animHierarchy);
        }
    }
}

CEntity::CEntity()
{
    m_status = STATUS_SIMPLE;
    bIsVisible = true;
    bBackfaceCulled = true;
    m_nScanCode = 0;
    m_modelIndex = -1;
    m_rwObject = NULL;
    m_areaCode = 0;
    m_iplIndex = 0;
    m_ucSeedColFlags = rand();
    m_pRef = NULL;
   // pLastRenderedLink = NULL;
    numLodChildren = 0;
    numLodChildrenRendered = 0;
    m_pLod = NULL;
}

void CEntity::SetModelIndex(unsigned int modelIndex)
{
    SetModelIndexNoCreate(modelIndex);
    CreateRwObject();
}

void CEntity::UpdateRwFrame()
{
    if(m_rwObject)
    {
        RwFrameUpdateObjects(static_cast<RwFrame*>(m_rwObject->object.parent));
    }
}

bool CEntity::DoesNotCollideWithFlyers()
{
    CBaseModelInfo *pModelInfo = CModelInfo::ms_modelInfoPtrs[m_nModelIndex];
    if((pModelInfo->flags & 0x7800) != 0x800 && (pModelInfo->flags & 0x7800) != 0x1000)
    {
        return pModelInfo->flags & 0x400;
    }
    return true;
}

void CEntity::Add()
{
    Add(GetBoundRect());
}

void CEntity::Render()
{
    if(m_rwObject)
    {
        if(m_rwObject->object.type = RW_TYPE_ATOMIC && CTagManager::IsTag(this))
        {
            CTagManager::RenderTagForPC(m_pRwObject);
        }
        else
        {  
            int alphaFunc;
            // Remove alpha function for translucent objects
            if(m_modelIndex == MI_JELLYFISH || m_modelIndex == MI_JELLYFISH01)
            {
                RwRenderStateGet(rwRENDERSTATEALPHATESTFUNCTIONREF, &alphaFunc);
                RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, NULL);
            }
            m_imBeingRendered = true;
            if(m_rwObject->object.type == RW_TYPE_ATOMIC)
            {
                RpAtomic* atomic = reinterpret_cast<RpAtomic*>(m_rwObject);
                atomic->renderCallBack(atomic);
            }
            else
            {
                RpClumpRender(m_rwObject);
            }
            CStreaming::RenderEntity(m_pLastRenderedLink);
            RenderEffects();
            // Restore alpha function for translucent objects
            if(m_modelIndex == MI_JELLYFISH || m_modelIndex == MI_JELLYFISH01)
            {
                RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, &alphaFunc);
            }
            m_imBeingRendered = false;
        }
    }
}

bool CEntity::LivesInThisNonOverlapSector(int sectorCenterX, int sectorCenterY)
{
    CRect boundRect = GetBoundRect();
    // convert to sector grid
    boundRect.left = floor(boundRect.left * GRID_UNIT + GRID_OFFSET);
    boundRect.bottom = floor(boundRect.bottom * GRID_UNIT + GRID_OFFSET);
    boundRect.right = floor(boundRect.right * GRID_UNIT + GRID_OFFSET);
    boundRect.top = floor(boundRect.top * GRID_UNIT + GRID_OFFSET);
    // calculate the center point of rectangle
    int centerX = floor((boundRect.right + boundRect.left) * GRID_UNIT * 0.5 + GRID_OFFSET);
    int centerY = floor((boundRect.top + boundRect.bottom) * GRID_UNIT * 0.5 + GRID_OFFSET);
    return centerX == sectorCenterX && centerY == sectorCenterY;
}

void CEntity::SetupBigBuilding()
{
    m_usesCollision = false;
    m_isBigBuilding = true;
    m_dontCastShadowsOn = true;
    CModelInfo::ms_modelInfoPtrs[m_modelIndex]->flags |= 0x20;
}

void CEntity::ModifyMatrixForCrane()
{
    if(!CTimer::m_UserPause && !CTimer::m_CodePause)
    {
        if(m_rwObject)
        {
            RwMatrix *pModelling = &reinterpret_cast<RpClump*>(m_rwObject->object.parent)->modelling;
            if(m_rwObject->parent != -16)
            {
                CMatrix mat(m_pRwObject->parent->modelling);
                mat.SetRotateZOnly((CTimer::GetCurrentTimeInCycles() % 1024) * 0.006132812704890966);
                mat.UpdateRW();
                UpdateRwFrame();
            }
        }
    }
}

void CEntity::RemoveLighting(bool bAdjustGlobal)
{
    if(bAdjustGlobal)
    {
        SetAmbientColours();
        DeActivateDirectional();
        CPointLights::RemoveLightsAffectingObject();
    }
}

bool CEntity::SetupLighting()
{
    if(bLightObject)
    {
        ActivateDirectional();
        CVector vecPos = Placeable.GetPos();
        SetLightColoursForPedsCarsAndObjects(CPointLights::GenerateLightsAffectingObject(vecPos, 0, this) * 0.5);
    }
}

void CEntity::CleanUpOldReference(CEntity **ppRef)
{
    if(m_pReferences)
    {
        for(SReference *pRef = m_pReferences; pRef; pRef = pRef->m_pNext)
        {
            if(pRef->m_pEntity == ppRef)
            {
                m_pReferences->m_pNext = pRef->m_pNext;
                pRef->m_pNext = &CReferences::pEmptyList;
                CReferences::pEmptyList = pRef;
                pRef->m_pEntity = NULL;
            }
        }
    }
}

void CEntity::AttachToRwObject(RwObject *pObject, bool bUpdateMatrix)
{
}

void CEntity::BuildWindSockMatrix()
{
}

void CEntity::CleanUpOldReference(CEntity **ppRef)
{
    CReference *pRef = m_pRef;
    while(pRef)
    {
        if(m_pRef->pEntity == *ppRef)
        {
            m_pRef->pNext = CReferences::pEmptyList;
            m_pRef->pEntity = NULL;
            CReferences::pEmptyList = m_pRef;
        }
        pRef = pRef->pNext;
    }
}

void CEntity::SetIsStatic(bool bIsStatic)
{
    bIsStatic = true;
}

void CEntity::PreRender()
{
    CBaseModelInfo *pModelInfo = CModelInfo::ms_modelInfoPtrs[m_nModelIndex];
    if(pModelInfo->m_uc2DFxCount)
    {
        ProcessLightsForEntity();
    }
    if(!(pModelInfo->flags & 1))
    {
        pModelInfo->flags |= 1;
        CAtomicModelInfo *pAtomicInfo = pModelInfo->AsAtomicModelInfoPtr();
        if(pAtomicInfo)
        {
            // #TODO
        }
}
