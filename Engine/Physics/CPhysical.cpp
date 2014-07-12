#include "StdInc.h"

CPhysical::CPhysical()
{
	AllocateStaticMatrix();
    m_xyz->matrix->SetIdentityMatrix();
	m_fMass = 1.0f;
	m_fTurnMass = 1.0f;
	m_pad2 = 1.0f;
	fAirResistance = 0.1f;
	bUsesCollision = true;
	m_pad1 = 100.0f;
	bApplyGravity = true;
	m_pad9 = 1.0f;
}

void CPhysical::ApplyMoveForce(CVector force)
{
    if(!m_infinite_mass && !m_disable_z_component)
    {
        if(m_0x80)
        {
            force.z = 0.0f;
        }
        // a = F / m
        m_linear_velocity += force / m_mass;
    }
}

void CPhysical::ApplyTurnForce(CVector force, CVector point)
{
    if(!m_disable_turn_force)
    {
        CVector intertialTensor;
        if(!m_infinite_mass)
        {
            intertialTensor = m_xyz->matrix * m_center_of_mass;
        }
        if(m_disable_z_component)
        {
            point.z = 0.0;
            force.z = 0.0;
        }
        dx = point - intertialTensor;
        pThis->vecAngularVelocity += dx.CrossProduct(force) / m_mass;
    }
}

void CPhysical::ApplyMoveSpeed()
{
    if(bDisableZComponent || b0x2000)
    {
        m_linear_velocity = CVector(0.0, 0.0, 0.0);
    }
    else
    {
        // Euler integration
        m_xyz->matrix.matrix.pos += CTimer::GetTimeStep() * m_vecLinearVelocity;
    }
}

void CPhysical::ApplyTurnSpeed()
{
    if(b0x2000)
    {
        m_vecAngularVelocity = 0;
    }
    else
    {
        CVector dt_avel = m_angular_velocity * dt;
        m_xyz->matrix.matrix.right += dt_avel.CrossProduct(m_xyz->matrix.matrix.right);
        m_xyz->matrix.matrix.top += dt_avel.CrossProduct(m_xyz->matrix.matrix.top);
        m_xyz->matrix.matrix.at += dt_avel.CrossProduct(m_xyz->matrix.matrix.at);
        if(!bInfiniteMass && !bDisableZComponent)
        {
            m_xyz->matrix.matrix.pos += dt_avel.CrossProduct(m_xyz * -m_center_of_mass);
        }
    }
}

void CPhysical::ApplyGravity()
{
    if(bApplyGravity)
    {
        if(!bDisableZComponent)
        {
            if(bInfiniteMass)
            {
                CVector gravity(0.0f, dt * m_mass * -0.008f, 0.0f);
                CVector inertiaTensor = m_xyz->matrix * m_center_of_mass;
                ApplyForce(gravity, inertiaTensor, true);
            }
            else
            {
                if(bUsesCollision)
                {
                    m_linear_velocity.z -= dt * 0.00800000037997961;
                }
            }
        }
    }
}

void CPhysical::ApplyForce(CVector force, CVector point, bool apply_rotation)
{
    if(b0x80)
        {} /* @! revision needed */
    if(!bInfiniteMass && !bDisableZComponent)
    {
        // a = F / m
        m_linear_velocity += vecForce / m_mass;
    }
    if(!bDisableTurnForce && apply_rotation)
    {
        CVector inertia_tensor = CVector(0.0, 0.0, 0.0);
        float turn_mass = m_turn_mass;
        if(bInfiniteMass)
        {
            turn_mass += m_center_of_mass.z * m_center_of_mass.z * m_mass * 0.5;
        }
        else
        {
            inertia_tensor = m_xyz->matrix * m_center_of_mass;
        }
        if(bDisableZComponent)
        {
            force.z = 0.0;
            point.z = 0.0;
        }
        point = point - inertia_tensor;
        m_angular_velocity += point.CrossProduct(force) / m_mass;
    }
}

bool CPhysical::GetHasCollidedWith(CEntity *entity)
{
    if(bEnableCollision)
    {
        if(m_collisions_count >= 0 && m_collisions_count <= sizeof(m_last_contacted_entities))
        {
            for(size_t i = 0; i < sizeof(m_last_contacted_entity); i++)
            {
                if(m_last_contacted_entity[i] == entity)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool CPhysical::GetHasCollidedWithAnyObject()
{
    if(bEnableCollision)
    {
        if(m_collisions_count >= 0 && m_collisions_count <= sizeof(m_last_contacted_entities))
        {
            for(size_t i = 0; i < sizeof(m_last_contacted_entity); i++)
            {
                if(m_last_contacted_entity[i]->entityInfo & 0x07 == 4)
                {
                    return true;
                }
            }
        }
    }
    return false;  
}

void CPhysical::ApplySpringCollision(float k, CVector &force, CVector &point, float damp, float bias, float& dx)
{
    damp = 1.0 - damp;
    if(damp > 0.0)
    {
        float dt_local = dt;
        if(dt >= 3.0)
        {
            dt_local = 3.0;
        }
        // F = -k*x
        dx = damp * m_mass * k * 0.01600000075995922 * bias * dt_local;
        force *= -dx;
        ApplyForce(force, point, true);
    }
}

void CPhysical::AddToMovingList()
{
    if(!m_moving_list && !bIsStaticWaitingForCollision)
    {
        CPtrNodeDouble *m = new CPtrNodeDouble;
        m->ptr = this;
        m->next = NULL;
        m->prev = g_MovingList;
        m_moving_list = m;
        g_MovingList->next = m;
        g_MovingList = m;
    }
}

void CPhysical::RemoveFromMovingList()
{
    if(m_moving_list)
    {
        if(g_MovingList == m_moving_list)
        {
            g_MovingList = m_moving_list->prev;
        }
        if(m_moving_list->next)
        {
            m_moving_list->next->prev = m_moving_list->prev;
        }
        if(m_moving_list->prev)
        {
            m_moving_list->prev->next = m_moving_list->next;
        }
        delete m_moving_list;
        m_moving_list = NULL;
    }
}

void CPhysical::RemoveAndAdd()
{
    if(bIsBIGBuilding)
    {
        CEntity::Remove();
        CEntity::Add();
    }
    else
    {
        
    }
}

void CPhysical::Remove()
{
    if(bIsBIGBuilding)
    {
        CEntity::Remove();
    }
    else
    {
        CEntryInfoNode *node = m_collision_list;
        while(node)
        {
            CPtrNodeDouble *link = node.link.prev;
            //if(node->link.ptr->ptr == link->prev)
            //    link->ptr->ptr = link->prev->prev;
            // bypass and remove previous node
            if(link->next)
            {
                link->next->prev = link->prev;
            }
            if(link->prev)
            {
                link->prev->next = link->next;
            }
            link->Release();
            link = 
        }
    }
}

void CPhysical::RemoveRefsToEntity(CEntity *entity)
{
    for(size_t i = 0; i < m_collisions_count; i++)
    {
        if(m_last_contacted_entity[i] == entity)
        {
            for(size_t j = i; j < m_collisions_count - 1; j++)
            {
                m_last_contacted_entity[i] = m_last_contacted_entity[i + 1];
            }
            m_collisions_count--;
        }
    }
}

bool CPhysical::CanPhysicalBeDamaged(eWeaponType weapon, unsigned char *explosion_prone)
{
    if(explosion_prone) 
    {
        *explosion_prone = false;
    }
    switch(weapon)
    {
        case WEAPON_TYPE_FIST:
        case WEAPON_TYPE_BRASS_KNUCKLES:
        case WEAPON_TYPE_GOLF_CLUB:
        case WEAPON_TYPE_NIGHTSTICK:
        case WEAPON_TYPE_KNIFE:
        case WEAPON_TYPE_BASEBALL_BAT:
        case WEAPON_TYPE_SHOVEL:
        case WEAPON_TYPE_POOL_CUE:
        case WEAPON_TYPE_KATANA:
        case WEAPON_TYPE_CHAINSAW:
        case WEAPON_TYPE_DILDO1:
        case WEAPON_TYPE_DILDO2:
        case WEAPON_TYPE_VIBRATOR1:
        case WEAPON_TYPE_VIBRATOR2:
        case WEAPON_TYPE_FLOWERS:
        case WEAPON_TYPE_CANE:
            return !bMeeleProof;
        /* !# finish it*/
        case WEAPON_TYPE_9MM:
        case WEAPON_TYPE_SILENCED9MM:
        case WEAPON_TYPE_EAGLE:
        case WEAPON_TYPE_SHOTGUN:
        case WEAPON_TYPE_SAWNOFF_SHOTGUN:
        case WEAPON_TYPE_COMBAT_SHOTGUN:
        case WEAPON_TYPE_UZI:
        case WEAPON_TYPE_SMG:
        case WEAPON_TYPE_MP5:
        case WEAPON_TYPE_AK47:
        case WEAPON_TYPE_M4:
        case WEAPON_TYPE_TEC9:
        case WEAPON_TYPE_RIFLE:
        case WEAPON_TYPE_SNIPER:
        case WEAPON_TYPE_MINIGUN:
        case 52:
            return !bBulletProof;
        case WEAPON_TYPE_FLAMETHROWER:
            return !bFireProof;
        case WEAPON_TYPE_GRANADE:
        case 19:
        case 20:
        case 21:
        case WEAPON_TYPE_RPG:
        case WEAPON_TYPE_HEATSEEKING_ROCKET:
        case WEAPON_TYPE_SATCHEL_CHARGE:
        case WEAPON_TYPE_DETONATOR:
        case WEAPON_TYPE_EXPLOSION:
            if(explosion_prone)
            {
                *explosion_prone = !bExplosionProof;
            }
            return !bExplosionProof;
        case WEAPON_TYPE_MOLOTOV:
            if(explosion_prone)
            {
                *explosion_prone = !bExplosionProof || !bFireProof;
            }
            return !bExplosionProof || !bFireProof;
        case WEAPON_TYPE_VEHICLE:
        case WEAPON_TYPE_HELI_BLADES:
        case WEAPON_TYPE_SPLAT:
            return !bCollisionProof;
    }
    return true;
}

CRect *CPhysical::GetBoundRect() const
{
    CVector center;
    GetBoundCenter(center);
    float radius = modelPtrs[m_model_index]->pColModel->sphere.radius;
    CRect rect;
    rect.left = center.x - radius;
    rect.top = center.y + radius;
    rect.right = center.x + radius;
    rect.bottom = center.y - radius;
}
  
static void CPhysical::PlacePhysicalRelativeToOtherPhysical(CPhysical *first, CPhysical *second, CVector offset)
{
    CVector offset_obj_space = TransformPoint(m_xyz->matrix, offset);
    offset_obj_space += dt * 0.8999999761581421 * m_linear_velocity;
    CWorld::Remove(second);
    second->m_xyz->matrix = first->m_xyz->matrix;
    if(second->m_xyz)
    {
        second->m_xyz->matrix.matrix.pos = offset_obj_space;
    }
    else
    {
        second->placement.pos = offset_obj_space;
    }
    second->m_linear_velocity = first->m_linear_velocity;
    if(second->m_rw_object)
    {
        if(second->m_xyz)
        {
            second->m_xyz->matrix->ToRwMatrix(second->m_rw_object->modelling);
        }
        else
        {
            second->placement.ToRwMatrix(second->m_rw_object->modelling);
        }
    }
    second->UpdateRwFrame();
    CWorld::Add(second);
}

void CPhysical::AttachEntityToEntity(CEntity *entity, CVector *offset, RtQuat *rotation)
{
    if(entity)
    {
        m_attached_entity = entity;
        entity->RegisterReference(&m_attached_entity);
        if(!entity->m_xyz)
        {
            entity->AllocateMatrix();
            entity->m_placement.pos.RotateZ(entity->m_Xyz->matrix);
        }
        CMatrix mat;
        mat.FromMatrixLink(entity->m_xyz);
        if(entityInfo & 7 == 4)
        {
            if(entity->m_model_index = 530/*forklift*/)
            {
                if((CAutomobile*)entity->pSpecialParts[0])
                {
                    RwMatrix *rwmat = 0x7F0990(entity->pSpecialParts[0]);
                    mat.FromRwMatrix(rwmat);
                }
            }
        }
        if(offset)
        {
            m_attached_entity_offset = *offset;
        }
        else
        {
         /* fill in */
    }
}

void CPhysical::AttachEntityToEntity(CEntity *entity, CVector offset, CVector position)
{
    if(entity)
    {
        m_attached_entity = entity;
        entity->registerReference(&m_attached_entity);
        m_attached_entity_offset = offset;
        if(bInfiniteMass)
        {
            if(m_xyz)
            {
                m_attached_entity_pos = m_xyz->matrix.matrix.pos;
            }
            else
            {
                m_attached_entity_pos = m_placement.pos;
            }
        }
        else
        {
            m_attached_entity_pos = position;
        }
        m_attached_entity_quat = RtQuat(0.0, 0.0, 0.0, 0.0);
        m_ppAttachedEntity = m_attached_entity; /* @! decipher meaning of m_ppAttachedEntity*/
        if(bDisableFriction)
        {
            bCollidable = false;
            ProcessAttachedEntity();
        }
        else
        {
            if((entity->entityInfo & 7) == 4 && entity->bDisableFriction && !bInfiniteMass)
            {
                bDisableFriction = true;
                m_mass = 99999.9;
                m_turn_mass = 99999.9;
            }
            ProcessAttachedEntity();
        }
    }
}

void CPhysical::ApplyFrictionMoveForce(CVector force)
{
    if(!bInfiniteMass && !bDisableZComponent)
    {
        if(b0x80)
        {
            force.z = 0.0;
        }
        m_friction_move_force += force / m_mass;
    }
}

void CPhysical::ApplyFrictionForce(CVector force, CVector point)
{
    if(b0x80)
    {
        force.z = 0.0;
    }
    if(!bInfiniteMass && !bDisableZComponent)
    {
        m_friction_move_force += force / m_mass;
    }
    if(!bDisableTurnForce)
    {
        CVector inertialTensor(0.0, 0.0, 0.0);
        if(bInfiniteMass)
        {
            m_turn_mass += m_center_of_mass.z * m_center_of_mass.z * m_mass * 0.5;
        }
        else
        {
            inertialTensor = m_xyz->matrix * m_center_of_mass;
        }
        if(bDisableZComponent)
        {
            force.z = 0.0;
            point.z = 0.0;
        }
        point -= inertialTensor;
        m_friction_turn_force += point.CrossProduct(force) / m_turn_mass;
    }
}

void CPhysical::GetSpeed(CVector &speed, CVector point)
{
    CVector inertiaTensor;
    if(!bInfiniteMass)
    {
       inertialTensor = m_xyz->matrix * m_center_of_mass 
    }
    point -= inertialTensor;
    CVector total_momentum = m_angular_velocity + m_friction_turn_force;
    speed = m_friction_move_force + m_linear_velocity +  total_momentum.CrossProduct(point);
}

bool CPhysical::ApplyFriction(float friction, CColPoint &point)
{
    if(bDisableFriction)
    {
        return false;
    }
    if(bDisableTurnForce)
    {
        CVector friction_force = m_linearVelocity - point.Normal * m_linear_velocity.DotProduct(point.Normal);
        if(friction_force.Length() > 0.0)
        {
            float Fn = -friction_force.Length();
            float Fg = -dt / m_mass * friction;
            if(Fn < Fg)
            {
                Fn = Fg;
            }
            m_friction_move_force.x += Fn * (friction_force.x / friction_force.Length());
            m_friction_move_force.y += Fn * (friction_force.y / friction_force.Length());
            return true;
        }
        return false;
    }
    CVector speed;
    CVector posdiff = point->Position - m_xyz->matrix.matrix.pos;
    GetSpeed(&speed, posdiff);
    CVector friction_force = speed - point->Normal * speed.DotProduct(Normal);
    if(friction_force.Length() <= 0.0)
    {
        return false;
    }
    CVector relative_pos = posdiff - m_xyz->matrix * m_center_of_mass;
    CVector vec = relative_pos.CrossProduct(friction_force.Unit());
    float Fg = -(1.0 / (vec.LengthSquared() / m_turn_mass + 1.0 / m_mass) * friction_force.Length());
    float Fn = -friction; 
    if(Fg < Fn)
    {
        Fg = Fn;
    }
    ApplyFrictionForce(friction_force.Unit() * Fg, posdiff);
    if(friction_force.Length() > 0.1)
    {
        if(g_surfaceInfos->GetFrictionEffect(point->bSurfaceTypeB))
        {
            if(g_surfaceInfos->GetFrictionEffect(point->bSurfaceTypeA) == 1 || bIsTypeVehicle)
            {
                if(!bIsTypeVehicle || /*now we are vehicle class*/m_field_594 != 10 || !m_pDriver || fabs(point->Normal.DotProduct(m_xyz->matrix.matrix.right)) >= 0.86669999)
                {
                    for(size_t i = 0; i < 8; i++)
                    {
                        float fRandSubLoc = rand() * 0.00003 * 0.4 - 0.2;
                        CVector vecPosition = point->Normal.CrossProduct(m_vecLinearVelocity).Normalise() * fRandSubLoc + point->Position;
                        CVector vecDirection = friction_force.Unit() + point->Normal * 0.1;
                        CVector vecAcrossLine = friction_force.Unit() * friction_force.Length() * 0.25;
                        Fx_c::AddSparks(vecPosition, vecDirection, friction_force.Length() * 12.5, 1, vecAcrossLine, 0.0, 0.1, 1.0);
                    }
                }
            }
        }
    }
}

void CPhysical::AddCollisionRecord(CEntity *pEntity)
{
    m_nImmunities |= 0x200;
    m_lastTimeCollisionCheck = CTimer::m_snTimeInMiliseconds;
    if(bIsTypeVehicle && pEntity->bIsTypeVehicle)
    {
        // trigger vehicle alarm ( this was inlined )
        CVehicle *pMeVehicle = (CVehicle*)this;
        if(pMeVehicle->CarAlarmState = 0xFFFF)
        {
            pMeVehicle->CarAlarmState = 15000;
        }
        CVehicle *pOtherVehicle = (CVehicle*)pEntity;
        if(pOtherVehicle->CarAlarmState = 0xFFFF)
        {
            pOtherVehicle->CarAlarmState = 15000;
        }
    }
    if(m_nImmunities & 0x10000000)
    {
        if(m_ucCollisionsNum == 0)
        {
#tODO#
        }
    }
}

float CPhysical::GetLightingTotal() const
{
    bool bIsInInterior = false;
    if(m_areaCode && bIsTypePed)
    {
        CPed *pThisPed = (CPed*)this;
        CPlayerData *pPlayerData = pThisPed->pPlayerData;
        bIsInInterior = pPlayerData && (CGame::currArea || pPlayerData->bForceInteriorLighting);
    }
    return m_fBrightness2 + GetLightingFromCol(bIsInInterior);
}

void CPhysical::ApplyAirResistance()
{
    if(m_fAirResistance <= 0.1 || bIsTypeVehicle)
    {
        float fFactor = m_vecLinearVelocity.Length() * m_fAirResistance;
        if(CCullZones::DoExtraAirResistanceForPlayer() && bIsTypeVehicle)
        {
            if(!m_field_594 || m_field_594 == 9)
            {
                fFactor *= CVehicle::m_fAirResistanceMult;
            }
        }
        m_vecLinearVelocity *= pow(CTimer::ms_fTimeStep, 1.0 - fFactor);
        m_vecAngularVelocity *= pow(CTimer::ms_fTimeStep, 1.0 - fFactor);
    }
    m_vecLinearVelocity *= pow(CTimer::ms_fTimeStep, m_fAirResistance);
    m_vecAngularVelocity *= pow(CTimer::ms_fTimeStep, m_fAirResistance);
}

void CPhysical::ApplyCollisionAlt(CEntity *pEntity, CColPoint &pColPoint, float& fCounterForce, CVector&, CVector&)
{
    if(m_pAttachedEntity)
    {   
        if(nEntityType > 1 && nEntityType < 6 && nEntityType != 3)
        {
            int dummy;
            ApplySoftCollision(m_pAttachedEntity, pEntity, pColPoint, &dummy);
        }
    }
    if(m_bDisableTurnForce)
    {
        float fDot = m_vecLinearVelocity.DotProduct(pColPoint);
        if(fDot < 0.0)
        {
            fCounterForce = -fDot * m_fMass;
            CVector vecCounterForce = fCounterForce * pColPoint->Normal;
            ApplyMoveForce(vecCounterForce);
            AudioEngine.ReportCollision(this, pEntity, pColPoint->bSurfaceTypeA, pColPoint->bSurfaceTypeB, pColPoint, vecCounterForce, fCounterForce / m_fMass, 1.0, 0, 0);
            return 1;
        }
        return 0;
    }
    CVector vecPoint = pColPoint->Position - m_xyz->matrix.matrix.right;
    CVector vecPointSpeed = GetSpeed(vecPoint);
    if(m_nImmunities & 0x4000000 && m_bIsTypeVehicle && pColPoint->bSurfaceTypeA == 65)
    {
        CVehicle *pMeAsVehicle = (CVehicle*)this;
        vecPointSpeed += pMeAsVehicle->AddMovingCollisionSpeed(v61, &vecPointSpeed);
    }
    float fDot = vecPointSpeed.DotProduct(pColPoint->Normal);
    if(fDot >= 0.0)
    {
        return 0;
    }
    CVector vecInertia = Multiply3x3(m_xyz->matrix, m_vecCenterOfMass);
    CVector vecActualSpeed;
    if(m_nImmunities & bInfiniteMass)
    {
        vecInertia = CVector(0.0,0.0,0.0);
        vecActualSpeed = CVector(0.0,0.0,0.0);
    }
    vecActualSpeed = vecPointSpeed - vecInertia;
    CVector vecResultant = CrossProduct(vecActualSpeed, pColPoint->Normal);
    float fCounterSpeed = 1.0 / (vecResultant.LengthSquared() / m_fTurnMass + 1.0/m_fMass);
    if(m_nEntityType == 4)
    {
    }
    if(m_nEntityType == ENTITY_TYPE_VEHICLE)
    {
        if(!(m_nImmunities & 0x100))
        {
            if(m_uiCarType != 9 || (entityInfo & 0x8) != 4 && entityInfo != 5)
            {
                if(m_uiCarType == 5)
                {
                }
                else
                {
                }
                // TODO
}

CPhysical::~CPhysical()
{
    if(m_pRealTimeShadow)
    {
       g_realTimeShadowMan.ReturnRealTimeShadow(m_pRealTimeShadow);
    }
    m_pCollisionList->Flush();
}


void CPhysical::ProcessEntityCollision(CEntity *pOther, CColPoint *pColPoint)
{
    pOther->EnsureHasValidMatrix();
    CColModel *pMyColModel = GetColModel();
    CColModel *pOtherColModel = pOther->GetColModel();
    if(CCollision::ProcessColModels(GetMatrix(), pMyColModel, pOther->GetMatrix(), pOtherColModel, pColPoint, NULL, 0.0, false))
    {
        AddCollisionRecord(&pOther);
        if(pOther->nType == ENTITY_TYPE_BUILDING)
        {
            if(pOther->bIsStatic || pOther->bIsStaticWaitingForCollision)
            {
                m_bHasHitWall = true;
            }
        }
        else
        {
            pOther->AddCollisionRecord(&this);
        }
    }
}