#include "StdInc.h"

CObject::CObject()
{
    m_pRelatedDummy = NULL;
    Init();
    field_13C = 0;
}

CObject::CObject(int iModelIndex, bool bCreate)
{
    m_pRelatedDummy = NULL;
    if(bCreate)
    {
        SetModelIndex(iModelIndex);
    }
    else
    {
        SetModelIndexNoCreate(iModelIndex);
    }
    Init();
}

CObject::CObject(CDummyObject *object)
{
    SetModelIndexNoCreate(object->model_index);
    if(object->m_pRwObject)
    {
        AttachToRwObject(object->m_pRwObject, 1);
    }
    else
    {
        if(!object->m_xyz)
        {
            object->AllocateMatrix();
            object->placement.RotateZ(object->m_xyz->matrix);
        }
        SetMatrix(object->m_xyz->matrix);
    }
    DetachFromRwObject(object);
    Init();
    m_iplIndex = object->m_iplIndex;
    m_areaCode = object->m_areaCode;
    bRenderDamaged = object->bRenderDamaged;
    RpClump *rwobj = m_pRwObject;
    if(rwobj)
    {
        if(rwobj->object.type != TYPE_CLUMP)
        {
            rwobj = (RpAtomic*)getFirstAtomicInClump(rwobj);
        }
        if(!isAtomicHaveNightColor((RpAtomic*)rwobj))
        {
            bLightObject = true;
        }
    }
}

bool CObject::CreateRwObject()
{
    return CEntity::CreateRwObject();
}

void CObject::SetRelatedDummy(CDummyObject *pDummy)
{
    m_pRelatedDummy = pDummy;
    pDummy->RegisterReference(&m_pRelatedDummy);
}

bool CObject::TryToExplode()
{
    if(m_pObjectInfo->ucCausesExplosion && !bExploded)
    {
        bExploded = true;
        ForceExplode();
        return true;
    }
    return false;
}

void CObject::Explode()
{
    CVector pos = Placeable.GetPos();
    pos.z += 0.5;
    CExplosion::AddExplosion(this, getPlayerPed(-1), 9, pos, 100, 1, -1.0, 0);
    if(m_colDamageEffect == 0xCA || m_colDamageEffect == 0xc8)
    {
        pos.z -= 1.0;
        Break(10000.0, pos, 0, getPlayerPed(-1), 51);
    }
    else if(!bDisableFriction)
    {
        m_vecLinearVelocity.z += 0.5;
        m_vecLinearVelocity.x += (rand() - 128) * 0.000199999994947575;
        m_vecLinearVelocity.y += (rand() - 128) * 0.000199999994947575;
        if(bIsStatic || bIsStaticWaitingForCollision)
        {
            SetIsStatic(false);
            AddToMovingList();
        }
    }
    if(m_pObjectInfo->fxType == 2)
    {
        CMatrix pos = CMatrix::FromXYZ(m_xyz);
        // to object space
        CVector fx_obj_pos = pos * m_pObjectInfo->fxOffset;
        fx_obj_pos += GetPos(); // to world space
        FxSystem_c *sys = FxManager.InitialiseFxSystem(m_pObjectInfo->pFxSystemBP, fx_obj_pos, 0, 0);
        if(sys)
        {
            sys->Start();
        }
    }
}

void CObject::ObjectFireDamage(float damage, CEntity *damager)
{
    if(m_nModelIndex == MODEL_INDEX_GRASSPLANT || m_nModelIndex == MODEL_INDEX_GRASSHOUSE)
    {
        m_fHealth -= damage;
        if(m_fHealth < 0.0)
        {
            m_fHealth = 0.0;
        }
        if(!bInvulnerable || damager == getPlayerPed(-1) || damager == getPlayerVehicle(-1,0))
        {
            if(m_nModelIndex == MODEL_INDEX_GRASSPLANT)
            {
                m_burnTime = currentTime + 3000;
                m_burnDamage = 1.0 - m_fHealth * 0.0005000000237487257;
            }
            if(m_fHealth == 0.0)
            {
                m_burnTime = 0;
                switch(m_colDamageEffect)
                {
                    case 1:
                        if(!bCollisionProcessed)
                        {
                            bRenderDamaged = true;
                            DeleteRwObject();
                            CShadows::AddPermanentShadow(1, texShadHeli, GetPos(), 0x40400000u, 0, 0, 0xC0400000u, 200, 0, 0, 0, 0x41200000u, 30000, 0x3F800000u);
                        }
                    break;
                    case 200: 
                    case 202:
                        if(m_bBroken)
                        {
                            CAudio::ProcessBrokenObject(this);
                        }
                        BreakableInfoPool.Add(this, m_pObjectInfo->breakVelocity, m_pObjectInfo->fBreakVelocityRand, 1);
                        m_bUsesCollision = false;
                        m_bIsVisible = false;
                        if(!m_bIsStatic && !bIsStaticWaitingForCollision)
                        {
                            RemoveFromMovingList();
                        }
                        m_bExplosionProof = true;
                        m_bIsStatic = true;
                        m_vecLinearVelocity = 0;
                        m_vecAngularVelocity = 0;
                        m_bBroken = true;
                        DeleteRwObject();
                    break;
                }
            }
        }
    }
}

void CObject::ObjectDamage(float damage, CVector* vec1, CVector* vec2, CEntity* damager, eWeaponType weapon_type)
{
    if(m_bUsesCollision)
    {
        return;
    }
    if(weapon_type == 55 && damager && damager->entityType == ENTITY_TYPE_VEHICLE)
    {
        weapon_type == 50;
    }
    if(!CanPhysicalBeDamaged(weapon_type, NULL))
    {
        return;
    }
    m_fHealth = Max(m_fHealth - damage * m_pObjectInfo->fColDamageMultiplier, 0.0);
    if(m_colDamageEffect == 0 || (m_bInvulnerable && damager != getPlayerPed(-1) && damager != getPlayerVehicle(-1, 0)))
    {
        return;
    }
    if(m_nModelIndex == MODEL_INDEX_IMY_SMASH_WALL)
    {
        // vehicle must be SWAT Water Cannon Vehicle
        CVehicle *veh = damager;
        if(!veh)
        {
            return;
        }
        switch(damager->entityType)
        {
            case ENTITY_TYPE_PED:
                if(!damager->pedFlags.bInVehicle || !damager->pCurrentObjective)
                {
                    return;
                }
                veh = damager->pCurrentObjective;
            break;
            case ENTITY_TYPE_VEHICLE:
            break;
            default:
                return;
        }
        if(veh->m_nModelIndex != 601)
        {
            return;
        }
    }
    // must not be a Forklift
    if(damager && damager->m_nModelIndex == MODEL_INDEX_FORKLIFT)
    {
        return;
    }
    m_breakWeaponType = weapon_type;
    if(damage * m_pObject->fColDamageMultiplier > 150.0)
    {
        switch(m_colDamageEffect)
        {
            case 200:
            case 202:
            {
                bool smashed = false;
                if(m_pObjectInfo->fSmashMultiplier * 150.0 < damage * m_pObjectInfo->fColDamageMultiplier)
                {
                    smashed = true;
                }
                BreakableInfoPool.Add(this, m_pObjectInfo->breakVelocity, m_pObjectInfo->fBreakVelocityRand, smashed);
                m_bUsesCollision = false;
                m_bIsVisible = false;
                if(!IsStatic())
                {
                    RemoveFromMovingList();
                }
                m_bExplosionProof = true;
                m_bIsStatic = true;
                m_vecLinearVelocity = 0;
                m_vecAngularVelocity = 0;
                m_bBroken = true;
                DeleteRwObject();
                v14 = 1; // v14: advancedDamage ( needs audio + explosion) # basic is only fx
            }
            break;
            case 1:
                if(!bRenderDamaged)
                {
                    DeleteRwObject();
                    v14 = 1;
                }
            break;
            case 20:
                m_bUsesCollision = false;
                m_bIsVisible = false;
                if(!IsStatic())
                {
                    RemoveFromMovingList();
                }
                m_bIsStatic = true;
                m_bExplosionProof = true;
                m_vecLinearVelocity = 0;
                m_vecAngularVelocity = 0;
                DeleteRwObject();
                v14 = 1;
            break;
            case 21:
                if(m_bRenderDamaged)
                {
                    m_bUsesCollision = false;
                    m_bIsVisible = false;
                    if(!IsStatic())
                    {
                        RemoveFromMovingList();
                    }
                    m_bIsStatic = true;
                    m_bExplosionProof = true;
                    m_vecLinearVelocity = 0;
                    m_vecAngularVelocity = 0;
                    DeleteRwObject();
                    v14 = 1;
                }
                else
                {
                    DeleteRwObject();
                    m_bRenderDamaged = true;
                }
            break;
        }
        if(m_bUsesCollision && m_bIsVisible)
        {
            m_fHealth = 0.0;
        }
    }
    bool exploded = false;
    if(v14 == 1)
    {
        if(TryToExplode())
        {
            exploded = true;
        }
        CAudio::ProcessBrokenObject(this);
    }
    if(!exploded || !v14)
    {
        v27 = m_pObjectInfo->fxType == 3;
        if(v14 == 0)
        {
            if(m_pObjectInfo->fxType != 1)
            {
                return;
            }
            v27 = damage > 30.0;
        }
        else
        {
            v27 = m_pObjectInfo->fxType == 2;
        }
        if(v27)
        {
            if(m_pObjectInfo->fxOffset.x >= -500.0)
            {
                CMatrix pos = CMatrix::FromXYZ(m_xyz);
                // to object space
                CVector fx_obj_pos = pos * m_pObjectInfo->fxOffset;
                fx_obj_pos += GetPos(); // to world space
                FxSystem_c *sys = FxManager.InitialiseFxSystem(m_pObjectInfo->pFxSystemBP, fx_obj_pos, 0, 0);
                if(sys)
                {
                    sys->Start();
                }    
            }
            else
            {
                if(!offset)
                {
                    return;
                }
                RwMatrix matrix;
                Fx_c::CreateMatFromVec(matrix, offset, rotation);
                FxSystem_c *sys = FxManager.InitialiseFxSystem(m_pObjectInfo->pFxSystemBP, matrix, 0, 0);
                if(sys)
                {
                    sys->Start();
                }
            }
        }
    }
}

void CObject::Render()
{
    if(!b0x2000000)
    {
        if(m_nRefModelIndex != -1)
        {
            if(m_ucCreator == 3)
            {
                if(flags < 0)//test cl,cl; jns
                {
                    CVehicleModelInfo::ms_pRemapTexture = m_pRemapTexture;
                    CModelInfo::ms_modelInfoPtrs[m_nRefModelIndex]->SetColours(m_colour1, m_colour2, m_colour3, m_colour4);
                }
            }
        }
        CEntity::Render();
    }
}

void CObject::AddToControlCodeList()
{
    CPtrNodeDouble *pNewEntry = new CPtrNodeDouble;
    if(pNewEntry)
    {
        pNewEntry->ptr = this;
        pNewEntry->prev = NULL;
        pNewEntry->next = CWorld::ms_listObjectsWithControlCode;
        m_pControlCodeListLink = pNewEntry;
        if(CWorld::ms_listObjectsWithControlCode)
        {
            CWorld::ms_listObjectsWithControlCode->prev = pNewEntry;
        }
        CWorld::ms_listObjectsWithControlCode = pNewEntry;
    }
}

void CObject::RemoveFromControlCodeList()
{
    CPtrNodeDouble *pLink = m_pControlCodeListLink;
    if(pLink)
    {
        if(CWorld::ms_listObjectsWithControlCode == pLink)
        {
            CWorld::ms_listObjectsWithControlCode = pLink->next;
        }
        if(pLink->prev)
        {
            pLink->prev->next = pLink->next;
        }
        if(pLink->next)
        {
            pLink->next->prev = pLink->prev;
        }
        delete pLink;
        m_pControlCodeListLink = NULL;
    }
}

void CObject::RefModelInfo(int iRefModelIndex)
{
    m_nRefModelIndex = iRefModelIndex;
    CModelInfo::ms_modelInfoPtrs[iRefModelIndex]->AddRef();
}

void CObject::SetIsStatic(bool bLockAngle)
{

}