#include "StdInc.h"

void CPhysical::ApplyMoveForce(CVector force)
{
    if (!m_infiniteMass && !m_disableMoveForce)
    {
        if (m_disableZ)
        {
            force.z = 0.0f;
        }
        // a = F / m
        m_linearVelocity += force / m_mass;
    }
}

void CPhysical::ApplyTurnForce(CVector force, CVector point)
{
    if (!m_disableTurnForce)
    {
        CVector Ri(0.0f, 0.0f, 0.0f);
        if (!m_infiniteMass)
        {
            Ri = Multiply3x3(GetMatrix(), m_centerOfMass);
        }
        if (m_disableMoveForce)
        {
            force.z = 0.0f;
            point.z = 0.0f;
        }
        // r = Rf - Ri
        CVector displacement = point - Ri;
        // tau = r x F
        CVector tau = CrossProduct(displacement, force);
        // alpha = tau / I
        m_angularVelocity += tau / m_turnMass;
    }
}

void CPhysical::ApplyForce(CVector force, CVector point, bool applyTurnForce)
{
    if (m_disableZ)
    {
        force.z = 0.0f;
    }
    if (!m_infiniteMass && !m_disableMoveForce)
    {
        // a = F / m
        m_linearVelocity += force / m_mass;
    }
    if (applyTurnForce && !m_disableTurnForce)
    {
        float turnMass = m_turnMass;
        CVector Ri(0.0f, 0.0f, 0.0f);
        if (m_infiniteMass)
        {
            turnMass += m_centerOfMass.z * m_centerOfMass.z * m_mass * 0.5f;
        }
        else
        {
            Ri = Multiply3x3(GetMatrix(), m_centerOfMass);
        }
        if (m_disableZ)
        {
            point.z = 0.0f;
            force.z = 0.0f;
        }
        // r = Rf - Ri
        CVector displacement = point - Ri;
        // tau = r x F
        CVector tau = CrossProduct(displacement, force);
        // alpha = tau / I
        m_angularVelocity += tau / turnMass;
    }
}

void CPhysical::ApplyMoveSpeed()
{
    if (m_disableMoveForce || m_dontApplySpeed)
    {
        m_linearVelocity = CVector(0.0f, 0.0f, 0.0f);
    }
    else
    {
        CMatrix current = GetMatrix();
        // Eulear integration
        current.pos.x += CTimer::GetTimeStep() * m_linearVelocity.x;
        current.pos.y += CTimer::GetTimeStep() * m_linearVelocity.y;
        current.pos.z += CTimer::GetTimeStep() * m_linearVelocity.z;
        SetMatrix(current);
    }
}

void CPhysical::ApplyTurnSpeed()
{
    if (m_dontApplySpeed)
    {
        m_angularVelocity = CVector(0.0f, 0.0f, 0.0f);
        return;
    }
    CMatrix current = GetMatrix();
    CVector d = m_angularVelocity * CTimer::GetTimeStep();
    CVector dr = CrossProduct(d, current.right);
    current.right.x += dr.x;
    current.right.y += dr.y;
    current.right.z += dr.z;
    CVector dt = CrossProduct(d, current.up);
    current.up.x += dt.x;
    current.up.y += dt.y;
    current.up.z += dt.z;
    CVector da = CrossProduct(d, current.at);
    current.at.x += da.x;
    current.at.y += da.y;
    current.at.z += da.z;

    if (!m_infiniteMass && !m_disableMoveForce)
    {
        CVector I = Multiply3x3(current, -m_centerOfMass);
        CVector v = CrossProduct(d, I);
        current.pos.x += v.x;
        current.pos.y += v.y;
        current.pos.z += v.z;
    }
    SetMatrix(current);
}

void CPhysical::ApplyGravity()
{
    if (m_applyGravity && !m_disableMoveForce)
    {
        if (m_infiniteMass)
        {
            CVector I = Multiply3x3(GetMatrix(), m_centerOfMass);
            // (0, 0, dt * mass * -g)
            CVector gravity(0.0f, 0.0f, CTimer::GetTimeStep() * m_mass * -0.008f);
            ApplyForce(gravity, I, true);
        }
        else if (GetUsesCollision())
        {
            // a = a - dt * g
            m_linearVelocity.z -= CTimer::GetTimeStep() * 0.008f;
        }
    }
}

void CPhysical::ApplyFrictionMoveForce(CVector force)
{
    if (!m_disableMoveForce && !m_infiniteMass)
    {
        if (m_disableZ)
        {
            force.z = 0.0f;
        }
        // a = F / m
        m_frictionLinearVelocity += force / m_mass;
    }
}

void CPhysical::ApplyFrictionTurnForce(CVector force, CVector point)
{
    if (!m_disableTurnForce)
    {
        CVector Ri(0.0f, 0.0f, 0.0f);
        float totalMass = 0.0f;
        if (m_infiniteMass)
        {
            totalMass = m_centerOfMass.z * m_mass * m_centerOfMass.z * 0.5 + m_turnMass;
        }
        else
        {
            Ri = Multiply3x3(GetMatrix(), m_centerOfMass);
            totalMass = m_turnMass;
        }
        if (m_disableMoveForce)
        {
          point.z = 0.0f;
          force.z = 0.0f;
        }
        // r = Rf - Ri
        CVector displacement = point - Ri;
        // tau = r x F
        CVector tau = CrossProduct(displacement, force);
        // alpha = tau / I
        m_frictionAngularVelocity += tau / totalMass;
    }
}

CVector CPhysical::GetSpeed(CVector point)
{
    CVector Ri(0.0f, 0.0f, 0.0f);
    if (!m_infiniteMass)
    {
        Ri = Multiply3x3(GetMatrix(), m_centerOfMass);
    }
    CVector displacement = point - Ri;
    CVector netAngularVelocity = m_angularVelocity + m_frictionAngularVelocity;
    CVector netLinearVelocity = m_linearVelocity + m_frictionLinearVelocity;
    CVector angular = CrossProduct(netAngularVelocity, displacement);
    CVector speed = angular + netLinearVelocity;
    return speed;
}

void CPhysical::SkipPhysics()
{
    if (GetType() != ENTITY_TYPE_PED && GetType() != ENTITY_TYPE_VEHICLE)
    {
        SetIsBigBuilding(false);
    }
    SetHasContacted(false);
    SetIsInSafePosition(false);
    SetWasPostponed(false);
    SetHasHitWall(false);
    if (GetStatus() != STATUS_SIMPLE)
    {
        SetRenderDamaged(false);
        m_numCollisions = 0;
        m_pieceType = 0;
        m_collisionDamage = 0.0f;
        if (m_collisionedEntity)
        {
            m_collisionedEntity->CleanUpOldReference(&m_collisionedEntity);
            m_collisionedEntity = NULL;
        }
        m_frictionLinearVelocity = CVector(0.0f, 0.0f, 0.0f);
        m_frictionAngularVelocity = CVector(0.0f, 0.0f, 0.0f);
    }
}

void CPhysical::UnsetIsInSafePosition()
{
    m_linearVelocity *= -1.0f;
    m_angularVelocity *= -1.0f;
    ApplySpeed();
    m_linearVelocity *= -1.0f;
    m_angularVelocity *= -1.0f;
    SetIsInSafePosition(false);
}

void CPhysical::ProcessControl()
{
    if (GetType() != ENTITY_TYPE_PED)
    {
        SetIsBigBuilding(false);
    }
    SetHasContacted(false);
    SetIsInSafePosition(false);
    SetWasPostponed(false);
    SetHasHitWall(false);
    if (GetStatus() != STATUS_SIMPLE)
    {
        SetRenderDamaged(false);
        m_numCollisions = 0;
        m_pieceType = 0;
        m_collisionDamage = 0.0f;
        if (m_collisionedEntity)
        {
            m_collisionedEntity->CleanUpOldReference(&m_collisionedEntity);
            m_collisionedEntity = NULL;
        }
        ApplyFriction();
        if (!m_attachedEntity || m_infiniteMass)
        {
            ApplyGravity();
            ApplyAirResistance();
        }
    }
}

bool CPhysical::ProcessEntityCollision(CEntity* otherEntity, CColPoint* colPoint)
{
    if (!CCollision::ProcessColModels(GetMatrix(), *GetColModel(), otherEntity->GetMatrix(), *otherEntity->GetColModel(), colPoint, 0, 0, 0))
    {
        return false;
    }
    AddCollisionRecord(otherEntity);
    bool isBuilding = otherEntity->GetType() == ENTITY_TYPE_BUILDING;
    if (!isBuilding)
    {
        otherEntity->AddCollisionRecord(this);
    }
    else if (isBuilding || otherEntity->GetIsStatic() || otherEntity->GetIsStaticWaitingForCollision())
    {
        SetHasHitWall(true);
    }
    return true;
}

void CPhysical::RemoveRefsToEntity(CEntity* entity)
{
    for (size_t i = 0; i < m_numCollisions; i++)
    {
        if (m_contactedEntities[i] == entity)
        {
            for (size_t j = i; j < m_numCollisions - 1; j++)
            {
                m_contactedEntities[j] = m_contactedEntities[j + 1];
            }
            m_numCollisions--;
        }
    }
}

bool CPhysical::GetHasCollidedWith(CEntity* entity)
{
    if (!m_canBeCollidedWith)
    {
        return false;
    }
    for (size_t i = 0; i < m_numCollisions; i++)
    {
        if (m_contactedEntities[i] == entity)
        {
            return true;
        }
    }
    return false;
}

bool CPhysical::GetHasCollidedWithAnyObject()
{
    if (!m_canBeCollidedWith)
    {
        return false;
    }
    for (size_t i = 0; i < m_numCollisions; i++)
    {
        if (m_contactedEntities[i]->GetType() == ENTITY_TYPE_OBJECT)
        {
            return true;
        }
    }
    return false;
}

bool CPhysical::ApplyCollision(CEntity* colEntity, CColPoint& colPoint, float& colForce)
{
    if (m_disableTurnForce)
    {
        float dot = DotProduct(m_linearVelocity, colPoint.normal);
        if (dot >= 0.0)
        {
            return false;
        }
        colForce = -(dot * m_mass);
        CVector collisionVector;
        collisionVector = colPoint.normal * colForce;
        ApplyMoveForce(collisionVector);
        AudioEngine.ReportCollision(this, colEntity, colPoint.surfaceTypeA, colPoint.surfaceTypeB, colPoint,
                                    colPoint.pos, colForce / m_mass, 1.0f, 0, 0);
        return true;
    }
    CVector Rf = colPoint.pos - GetPos();
    CVector speed = GetSpeed(Rf);
    float dot = DotProduct(speed, colPoint.normal);
    if (dot >= 0.0)
    {
            return false;
    }
    CVector Ri = Multiply3x3(GetMatrix(), m_centerOfMass);
    CVector displacement = Rf - Ri;
    CVector t = CrossProduct(displacement, colPoint.normal);
    float m = 1.0 / (t.MagnitudeSquared() / m_turnMass + 1.0 / m_mass);
    colForce = -((m_elasticity + 1.0) * m * dot);
    CVector collisionMoveForce = colPoint.normal * colForce;
    if (GetType() == ENTITY_TYPE_VEHICLE && colPoint.normal.z < 0.7)
    {
        collisionMoveForce *= 0.3;
    }
    if (!m_disableCollisionForce)
    {
        bool applyTorque = !(GetType() == ENTITY_TYPE_VEHICLE && CWorld::bNoMoreCollisionTorque);
        ApplyForce(collisionMoveForce, Rf, applyTorque);
    }
    AudioEngine.ReportCollision(this, colEntity, colPoint.surfaceTypeA, colPoint.surfaceTypeB, colPoint,
                                colPoint.pos, colForce / m, 1.0f, 0, 0);
    return true;
}

void CPhysical::ApplySpringCollision(float k, CVector& force, CVector& point, float damp, float bias, float& a7)
{
    float damp_inv = 1.0 - damp;
    if (damp_inv > 0.0)
    {
        float dt = ClampMax(CTimer::GetTimeStep(), 3.0f);
        a7 = damp_inv * m_mass * k * 0.016000001 * dt * bias;
        ApplyForce(force * -a7, point, true);
    }
}

void CPhysical::ApplySpringCollisionAlt(float k, CVector& force, CVector& point, float damp, float bias, CVector& normal, float& a8)
{
    float damp_inv = 1.0f - damp;
    if (damp_inv > 0.0f)
    {
        if (DotProduct(force, normal) > 0.0f)
        {
            normal *= -1.0f;
        }
        float dt = ClampMax(CTimer::GetTimeStep(), 3.0f);
        a8 = damp_inv * dt * m_mass * k * bias * 0.016000001;
        if (m_0x01)
        {
            a8 *= 0.75f;
        }
        CPhysical::ApplyForce(normal * a8, point, true);
    }
}

static const float DAMPING_LIMIT_IN_FRAME = 0.25f;
static const float DAMPING_LIMIT_OF_SPRING_FORCE = 0.999f;

//void ApplySpringDampening(float, float, CVector&, CVector&, CVector&);
void CPhysical::ApplySpringDampening(float dampingLevel, float a3, CVector& a4, CVector& point, CVector& relVelocity)
{
	CVector speed = GetSpeed(point);
	float v30 = DotProduct(speed, a4);
	float damping = ClampMax(CTimer::GetTimeStep(), 3.0f) * dampingLevel;
	if (m_0x01)
	{
		damping = damping + damping;
	}
	damping = Clamp<float>(-DAMPING_LIMIT_IN_FRAME, damping, DAMPING_LIMIT_IN_FRAME);
	float v21 = DotProduct(relVelocity, a4);
	float bb = -(damping * v21);
	if ( bb > 0.0 && bb + v30 > 0.0 )
	{
		bb = v30 >= 0.0 ? 0.0 : -v30;
	}
	else if ( bb < 0.0 && bb + v30 < 0.0 )
	{
		bb = v30 <= 0.0 ? 0.0 : -v30;
	}
	CVector m = Multiply3x3(*GetMatrix(), m_centerOfMass);
	CVector diff = point - m;
	CVector v14 = CrossProduct(diff, a4);
	float v13 = 1.0 / (v14.MagnitudeSquared() / m_turnMass + 1.0 / m_mass) * bb;
	float bc = fabs(a3) * DAMPING_LIMIT_OF_SPRING_FORCE;
	v13 = ClampMax(v13, bc);
	CVector force = a4 * v13;
	ApplyForce(force, point, true);
}

bool CPhysical::ApplyFriction(float frictionCoeff, CColPoint& point)
{
  v3 = this->nImmunities;
	if (m_disableCollisionForce)
	{
		return false;
	}
	if (m_disableTurnForce)
	{
		float dot = DotProduct(m_linearVelocity, point.normal);
		CVector tangent = m_linearVelocity - point.normal * dot;
		float tangentMagnitude = tangent.Magnitude();
		if (tangentMagnitude <= 0.0)
		{
			return false;
		}
		tangent.Normalise();
		float friction = -(CTimer::GetTimeStep() / m_mass * frictionCoeff);
		float frictionMagnitude = ClampMin(-tangentMagnitude, friction);
		m_frictionLinearVelocity.x += tangent.x * frictionMagnitude;
		m_frictionLinearVelocity.y += tangent.y * frictionMagnitude;
		return true;
	}
  point_ = point;
  v19 = this->__parent.__parent.xyz;
  v20 = point->Position.x - v19->matrix.matrix.pos.x;
  v19 = (CMatrixLink *)((char *)v19 + 48);
  v39.x = v20;
  LODWORD(v21.x) = (_DWORD)v39.x;
  v40 = point->Position.y - v19->matrix.matrix.right.y;
  v21.y = v40;
  v41 = point->Position.z - v19->matrix.matrix.right.z;
  v21.z = v41;
  v22 = (unsigned int)CPhysical::GetSpeed(this, &b, COERCE_RWV3D(v21.x));
  v23 = b.y * point_->Normal.y + b.z * point_->Normal.z + b.x * point_->Normal.x;
  v24 = v23 * point->Normal.x;
  a.y = v23 * point->Normal.y;
  a.z = v23 * point->Normal.z;
  v18 = b.x - v24;
  m.y = b.y - a.y;
  v17 = b.z - a.z;
  v25 = sqrt(v17 * v17 + m.y * m.y + v18 * v18);
  pointb = v25;
  if ( v25 <= 0.0 )
    return 0;
  v27 = (CMatrix *)pThis->__parent.__parent.__parent.xyz;
  v28 = 1.0 / pointb;
  b.x = v28 * v18;
  b.y = m.y * v28;
  b.z = v17 * v28;
  Multiply3x3(&m, v27, &pThis->__parent.vecCenterOfMass);
  a.x = v39.x - m.x;
  a.y = v40 - m.y;
  a.z = v41 - m.z;
  v29 = CrossProduct(&out, &a, &b);
  v26 = -(1.0
        / ((v29->x * v29->x + v29->y * v29->y + v29->z * v29->z) / pThis->__parent.fTurnMass
         + 1.0 / pThis->__parent.fMass)
        * pointb);
  fFrictionb = -fFriction;
  if ( v26 < fFrictionb )
    v26 = fFrictionb;
  a.x = b.x * v26;
  a.y = b.y * v26;
  *(_QWORD *)&v30.x = *(_QWORD *)&a;
  a.z = b.z * v26;
  v30.z = a.z;
  v31 = (unsigned int)CPhysical::ApplyFrictionForce(&pThis->__parent, COERCE_RWV3D(v30.x), v39.x, v40, v41);
  if ( pointb > 0.1
    && SurfaceInfos_c::GetFrictionEffect(&g_surfaceInfos, point_->ucSurfaceTypeB)
    && (SurfaceInfos_c::GetFrictionEffect(&g_surfaceInfos, point_->ucSurfaceTypeA) == 1
     || (pThis->__parent.__parent.entityInfo & 7) == 2)
    && ((pThis->__parent.__parent.entityInfo & 7) != 2
     || pThis->field_594 != 10
     || !pThis->pDriver
     || fabs(dotProduct(&point_->Normal, &pThis->__parent.__parent.__parent.xyz->matrix.matrix.right)) >= 0.86669999) )
  {
    v35 = pointb * 0.25;
    scaleMatrixRight(&v47, v35, (CMatrix *)&b);
    LODWORD(v39.x) = (_DWORD)b.x;
    v40 = b.y;
    v41 = b.z;
    v36 = scaleMatrixRight(&out, 0.1, (CMatrix *)&point_->Normal);
    vectorOpPlus(&v39, v36);
    CrossProduct(&a, &point_->Normal, &pThis->__parent.vecLinearVelocity);
    CVector::Normalise(&a);
    v32 = v49;
    v33 = v48;
    v34 = 8;
    do
    {
      rand();
      v38 = (double)v37 * 0.000030518509 * 0.40000001 - 0.2;
      v45 = a.y * v38;
      v46 = a.z * v38;
      out.x = a.x * v38 + point_->Position.x;
      out.y = v45 + point_->Position.y;
      out.z = v46 + point_->Position.z;
      fFrictionc = pointb * 12.5;
      Fx_c::AddSparks(&out, &v39, fFrictionc, 1, v47.x, v33, v32, 0.0, 0.1, 1.0);
      --v34;
    }
    while ( v34 );
  }
  return 1;
}