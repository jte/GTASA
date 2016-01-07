#include "StdInc.h"

void CEntity::Add(const CRect& rect)
{
    CRect sector = rect;
    sector.left = ClampMin(sector.left, -3000.0f);
    sector.right = ClampMax(sector.right, 2999.0f);
    sector.bottom = ClampMin(sector.bottom, -3000.0f);
    // NOTE: R* checks for > 3000.0f bound and clamps to 2999.0f
    sector.top = ClampMax(sector.top, 2999.0f);
    if (m_isBigBuilding)
    {
    v6 = (unsigned __int64)(rect.left * 0.005 + 15.0);
    v32 = (unsigned __int64)(rect.right * 0.005 + 15.0);
    v7 = (unsigned __int64)(rect.top * 0.005 + 15.0);
    sector_top = (unsigned __int64)(rect.bottom * 0.005 + 15.0);
    if (sector.top <= v7 )
    {
      v8 = &CWorld::ms_aLodPtrLists[v6 + 30 * sector_top];
      v24 = v7 - sector_top + 1;
      do
      {
        if ( v6 <= v32 )
        {
          v10 = v32 - v6 + 1;
          do
          {
            CPtrNodeSingleLink__operator new(v8, v26);
            v8 = (CPtrNodeSingleLink *)((char *)v8 + 4);
            --v10;
          }
          while ( v10 );
        }
        v8 += 30;
        LODWORD(sector_top) = v24 - 1;
      }
      while ( v24-- != 1 );
    }
    }
    else
    {
        sector.left = floor(lrect.left * GRID_UNIT + GRID_OFFSET);
        sector.bottom = floor(lrect.bottom * GRID_UNIT + GRID_OFFSET);
        sector.right = floor(lrect.right * GRID_UNIT + GRID_OFFSET);
        sector.top = floor(lrect.top * GRID_UNIT + GRID_OFFSET);
        // Scan bottom to top
        for (size_t i = sector.bottom; i <= sector.top; i++)
        {
            // Scan left to right
            for (size_t j = sector.left; j <= sector.right; j++)
            {
                size_t iclamp;
                size_t jclamp;
                switch (m_type)
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
    RpClump *clump = reinterpret_cast<RpClump*>(m_rwObject);
    RpAtomic *atomic = GetFirstAtomic(clump);
    if (atomic && RpSkinGeometryGetSkin(atomic->geometry))
    {
        if (!m_dontUpdateHierarchy)
        {
            RpHAnimHierarchy *animHierarchy = GetAnimHierarchyFromSkinClump(clump);
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
    m_pLastRenderedLink = NULL;
    numLodChildren = 0;
    numLodChildrenRendered = 0;
    m_pLod = NULL;
}

void CEntity::SetModelIndex(uint32_t modelIndex)
{
    SetModelIndexNoCreate(modelIndex);
    CreateRwObject();
}

void CEntity::UpdateRwFrame()
{
    if (m_rwObject)
    {
        RwFrameUpdateObjects(RwFrameGetParent(m_rwObject));
    }
}

bool CEntity::DoesNotCollideWithFlyers()
{
    CBaseModelInfo *pModelInfo = CModelInfo::GetModelInfo(m_modelIndex);
    //if((pModelInfo->flags & 0x7800) != 0x800 && (pModelInfo->flags & 0x7800) != 0x1000)
    if (!bSwaysInWind && !bCollisionWasStreamedWithModel)
    {
        return pModelInfo->dwUnknownFlag27;
    }
    return true;
}

void CEntity::Add()
{
    Add(GetBoundRect());
}

void CEntity::Render()
{
    if (!m_rwObject)
    {
        return;
    }
    if (m_rwObject->type == RW_TYPE_ATOMIC && CTagManager::IsTag(this))
    {
        CTagManager::RenderTagForPC(m_rwObject);
    }
    else
    {  
        int alphaFunc;
        // Remove alpha function for translucent objects
        if (m_modelIndex == MI_JELLYFISH || m_modelIndex == MI_JELLYFISH01)
        {
            RwRenderStateGet(rwRENDERSTATEALPHATESTFUNCTIONREF, &alphaFunc);
            RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, NULL);
        }
        m_imBeingRendered = true;
        if (m_rwObject->type == RW_TYPE_ATOMIC)
        {
            RpAtomic* atomic = reinterpret_cast<RpAtomic*>(m_rwObject);
            atomic->renderCallBack(atomic);
        }
        else
        {
            RpClumpRender((RpClump*)m_rwObject);
        }
        CStreaming::RenderEntity(m_pLastRenderedLink);
        RenderEffects();
        // Restore alpha function for translucent objects
        if (m_modelIndex == MI_JELLYFISH || m_modelIndex == MI_JELLYFISH01)
        {
            RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, (void*)alphaFunc);
        }
        m_imBeingRendered = false;
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
    int centerX = floor((boundRect.right + boundRect.left) * GRID_UNIT / 2 + GRID_OFFSET);
    int centerY = floor((boundRect.top + boundRect.bottom) * GRID_UNIT / 2 + GRID_OFFSET);
    return centerX == sectorCenterX && centerY == sectorCenterY;
}

void CEntity::SetupBigBuilding()
{
    m_usesCollision = false;
    m_isBigBuilding = true;
    m_dontCastShadowsOn = true;
    CBaseModelInfo* modelInfo = CModelInfo::GetModelInfo(m_modelIndex);
    modelInfo->SetDrawAdditive(true);
}

void CEntity::ModifyMatrixForCrane()
{
    if (!CTimer::GetUserPause() && !CTimer::GetCodePause() && m_rwObject && m_rwObject->parent != -16)
    {
        CMatrix mat(&RwFrameGetParent(m_rwObject)->modelling);
        mat.SetRotateZOnly((CTimer::GetCurrentTimeMs() % 1024) * 0.006132812704890966);
        mat.UpdateRW();
        UpdateRwFrame();
    }
}

void CEntity::RemoveLighting(bool adjustGlobal)
{
    if (adjustGlobal)
    {
        SetAmbientColours();
        DeActivateDirectional();
        CPointLights::RemoveLightsAffectingObject();
    }
}

void CEntity::SetupLighting()
{
    if (bLightObject)
    {
        ActivateDirectional();
        SetLightColoursForPedsCarsAndObjects(CPointLights::GenerateLightsAffectingObject(&GetPos(), 0, this) / 2);
    }
}

void CEntity::CleanUpOldReference(CEntity** entityRef)
{
    for (SReference* ref = m_pReferences; ref; ref = ref->m_pNext)
    {
        if (ref->m_pEntity == entityRef)
        {
            m_pReferences->m_pNext = ref->m_pNext;
            ref->m_pNext = &CReferences::pEmptyList;
            CReferences::pEmptyList = ref;
            ref->m_pEntity = NULL;
        }
    }
}

void CEntity::AttachToRwObject(RwObject* rwObject, bool updateMatrix)
{
    if (!bIsVisible || !rwObject)
    {
        return;
    }
    m_rwObject = rwObject;
    if (updateMatrix)
    {
        GetMatrix()->UpdateMatrix(&RwFrameGetParent(m_rwObject)->modelling);
    }
    if (m_rwObject->type == RW_TYPE_CLUMP && CModelInfo::GetModelInfo(GetModelIndex())->GetHasAnimation())
    {
        if (GetType() == ENTITY_TYPE_OBJECT)
        {
            CPhysical* phys = (CPhysical*)this;
            if (phys->m_movingList)
            {
                phys->AddToMovingList();
            }
            SetIsStatic(false);
        }
        else
        {
            CPtrNodeDoubleLink* node = new CPtrNodeDoubleLink;
            node->data = this;
            CWorld::ms_listMovingEntityPtrs->AddToList(node);
        }
    }
    CModelInfo::GetModelInfo(m_modelIndex)->AddRef();
    m_pLastRenderedLink = CStreaming::AddEntity(this);
    CreateEffects();
}

void CEntity::BuildWindSockMatrix()
{
    CVector windDir = CVector(-(CWeather::WindDir.matrix.right.x + 0.01),
                            -(CWeather::WindDir.matrix.right.y + 0.01),
                            0.1f);
    windDir.Normalise();
    CVector verticalPlane = CVector(0.0f, 0.0f, 1.0f);
    CVector final = CrossProduct(windDir, verticalPlane);
    final.Normalise();
    CVector p = CrossProduct(verticalPlane, final);
    GetMatrix()->right = final;
    GetMatrix()->up = windDir;
    GetMatrix()->at = p;
    if (m_rwObject)
    {
        UpdateRwMatrix(m_rwObject);
        UpdateRwFrame();
    }
}
void CEntity::SetIsStatic(bool bIsStatic)
{
    bIsStatic = bIsStatic;
}

void CEntity::PreRender()
{
	CBaseModelInfo *pModelInfo = CModelInfo::GetModelInfo(GetModelIndex());
	if (pModelInfo->Get2dEffectCount() != 0)
    {
        ProcessLightsForEntity();
    }
	if (!pModelInfo->GetHasBeenPrerendered())
    {
		pModelInfo->SetHasBeenPrerendered(true);
        CAtomicModelInfo *pAtomicInfo = pModelInfo->AsAtomicModelInfoPtr();
        if (pAtomicInfo)
        {
            // #TODO
        }
	}
}

CMatrix* CEntity::GetMatrix()
{
    if (!m_xyz)
    {
        AllocateMatrix();
        UpdateMatrix(m_transform, m_xyz->matrix);
    }
    return &m_xyz->matrix.matrix;
}

eEntityType CEntity::GetType() const
{
    return m_type;
}

bool CEntity::GetUsesCollision() const
{
}

uint8_t CEntity::GetAreaCode() const
{
	return m_areaCode;
}