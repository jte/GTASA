/* not actual name for procedure*/
CVector CEntity::GetPos()
{
    if(m_xyz)
    {
        return m_xyz->matrix.matrix.pos;
    }
    else
    {
        return placement.pos;
    }
}

void CRope::Update()
{
    #define MAX_SEGMENTS 32
    float scale_factor = pow(dt, 0.800000011920929);
    CVector2D camera_pos = camera.GetPos();
    CVector2D cam_to_rope = camera_pos - m_vecRopeSegments[0];
    if(cam_to_rope.Length() < 200.0)
    {   /* is visible */
        if(!(m_ucFlags2 & 1))
        {   /* if it has expire time */
            if(currentTime > m_uiHoldEntityExpireTime)
            {
                m_vecRopeSegmentsReleased[0].z -= dt * 0.001500000013038516;
                m_vecRopeSegments[0] += dt * m_vecRopeSegmentsReleased 
            }
        }
        if(m_ucFlags2 & 4)
        {   /* is an entity attached*/
            if(m_global_flags & 7 == 2)
            {
                m_ground_z = CWorld::FindGroundZFor3DCoord(m_vecRopeSegments[0], 0, 0);
            }
            if(m_pAttachedEntity)
            {
                if(m_pAttachedEntity->entityType == ENTITY_TYPE_VEHICLE ||
                   m_pAttachedEntity->m_nModelIndex == miKmbContainerRed||
                   m_pAttachedEntity->m_nModelIndex == miKmbContainerYel||
                   m_pAttachedEntity->m_nModelIndex == miKmbContainerBlue||
                   m_pAttachedEntity->m_nModelIndex == miKmbContainerOpen)
                {   /* either a vehicle or a container */
                    m_ground_z = m_pRopeAttacherObject->GetPos()->z - 0.5;
                }
            }
        }
        for(size_t i = 1; i < MAX_SEGMENTS; i++)
        {
            CVector sav = m_vecRopeSegments[i];
            m_vecRopeSegmentsReleased[i].x += (double)((rand() % 16) - 8) * 0.001000000047497451;
            m_vecRopeSegmentsReleased[i].y += (double)((rand() % 16) - 8) * 0.001000000047497451;
            m_vecRopeSegmentsReleased[i] = m_vecRopeSegmentsReleased[i - 1] + scaleFactor * (m_vecRopeSegmentsReleased[i] - m_vecRopeSegmentsReleased[i - 1])
            m_vecRopeSegments[i].z -= dt * 0.1500000059604645;
            if(m_ucFlags2 & 4)
            { /* is entity attached */
                float ground_clamp = m_ground_z + 0.300000011920929;
                if(m_vecRopeSegments[i].z <= ground_clamp )
                {   /* if going under ground z level; clamp to ground level */
                    m_vecRopeSegments[i].z = ground_clamp;
                }
            }
            CVector cur_to_prev = m_vecRopeSegments[i] - m_vecRopeSegments[i - 1];
            m_vecRopeSegments[i] += cur_to_prev * (m_fRopeSegmentLength / cur_to_prev.Length());
            m_vecRopeSegmentsReleased[i] = (m_vecRopeSegmentsReleased[i] - sav) / dt;
        }
        // convert to switch
        if(m_ucRopeType != 1 && m_ucRopeType != 2 && m_ucRopeType != 4 && m_ucRopeType != 5 && m_ucRopeType != 7 && m_ucRopeType != 6 && m_ucRopeType != 3)
            {
                LABEL_249: // NOT 1,2,3,4,5,6,7
                    if(!(m_ucFlags2 & 1))
                    {
                        if(m_vecRopeSegments[0].z < -50)
                        {
                            CRope::~CRope(this);
                        }
                        m_ucFlags2 &= ~0x1;
                        return;
                    }
            }
        }
        LABEL_37:
        if((m_ucRopeType != 4 || controlledCraneNumber != 1) ||
           (m_ucRopeType != 5 || controlledCraneNumber != 2) ||
           (m_ucRopeType != 7 || controlledCraneNumber != 3) ||
           (m_ucRopeType != 6 || controlledCraneNumber != 4))
        {
            if(m_ucRopeType == 1)
            {
                goto LABEL_69;
            }
            if(m_ucRopeType != 3 && m_ucRopeType != 2)
            {
                goto LABEL_80;
            }
        }
        if(m_ucRopeType != 1 && m_ucRopeType != 2 && m_ucRopeType != 3)
        {
            if(bCraneControls_Enabled_Up)
            {
                float displacement = CPad::GetPad(0)->NewState.ButtonSquare * dt * 0.000009999999747378752;
                if(m_ucRopeType >= 4 && m_ucRopeType <= 7)
                {
                    if(displacement > 0.0)
                    {
                        if(displacement + m_uiRopeSegmentLength < 0.8999999761581421)
                        {
                            CAudioManager::ProcessCrane(104, m_pRopeHolder, 0, 1.0);
                        }
                    }
                }
                m_uiRopeSegmentLength += displacement;
            }
            if(bCraneControls_Enabled_Down)
            {
                float displacement = CPad::GetPad(0)->NewState.ButtonCross * dt * 0.000009999999747378752;
                if(m_ucRopeType >= 4 && m_ucRopeType <= 7)
                {
                    if(displacement > 0.0)
                    {
                        if(displacement + m_uiRopeSegmentLength < 0.8999999761581421)
                        {
                            CAudioManager::ProcessCrane(104, m_pRopeHolder, 0, 1.0);
                        }
                    }
                }
                m_uiRopeSegmentLength -= displacement;               
            }
            LABEL_79:
        }
        LABEL_80:
        m_uiRopeSegmentLength = Clamp(0.009999999776482582, m_uiRopeSegmentLength, 0.8999999761581421);
        if(m_pAttachedEntity)
        {
            float mass_coef = m_pAttachedEntity->m_fMass;
            if(m_pAttachedEntity->GetModelIndex() == MODEL_INDEX_SECURICAR)
            {
                mass_coef *= 750.0;
            }
            mass_coef *= flt_8CD89C * 0.0008333333535119891 + flt_8CD898
            if(mass_coef <= 0.5)
            {
                v213 = mass_coef;
            }
            else
            {
                v213 = 1.0;
            }
            m_pAttachedEntity->colFlag1 = 0;
        }
        else
        {
            m_pRopeAttacherObject->bUsesCollision = true;
            v213 = flt_8CD898;
        }
        // break into another function
        if(m_pAttachedEntity)
        {
            CVector newpos;
            if(UpdateWeightInRope(GetPos(), 0.1, newpos))
            {
                m_pAttachedEntity->SetPos(newpos);
          m_pRopeHolder->GetSpeed(speed, m_vecRopeSegments[0] - m_pRopeHolder->GetPos());
          if ( m_ucRopeType == 4 || m_ucRopeType == 5 || m_ucRopeType == 6 || m_ucRopeType == 7 )
          {
            speed = 0;
          }
          m_pAttachedEntity->m_vecLinearVelocity -= speed;
          CVector dx = m_pAttachedEntity->GetPos() - m_vecRopeSegments[0];
          RwV3D__Normalize(&dx);
          if ( dx * m_pAttachedEntity->m_vecLinearVelocity > 0.0 )
          { // less than 90 deg
            m_pAttachedEntity->m_vecLinearVelocity -= dx * m_pAttachedEntity->m_vecLinearVelocity;
          }
          m_pAttachedEntity->vecLinearVelocity += m_pAttachedEntity->m_vecLinearVelocity * (1.0 - v213);
          m_pRopeHolder->vecLinearVelocity -= m_pAttachedEntity->m_vecLinearVelocity * v213;
          if ( *LODWORD(v59) )
          {
            CMatrix__FromXYZ(&v258, v60->m_xyz);
            v93 = (CMatrix *)v60->__parent.__parent.xyz;
            point.x = -x1.x;
            point.y = -y1;
            point.z = -z1;
            CMatrix__Unknown9(v93, point.x, point.y, point.z);
            CMatrix__FromXYZ(&v249, v60->__parent.__parent.xyz);
            v60->__parent.__parent.xyz->matrix.matrix.right.x = v258.matrix.right.x * 0.8999999761581421 + v249.matrix.right.x * 0.1000000014901161;
            v60->__parent.__parent.xyz->matrix.matrix.right.y = v258.matrix.right.y * 0.8999999761581421 + v249.matrix.right.y * 0.1000000014901161;
            v60->__parent.__parent.xyz->matrix.matrix.right.z = v258.matrix.right.z * 0.8999999761581421 + v249.matrix.right.z * 0.1000000014901161;
            v60->__parent.__parent.xyz->matrix.matrix.top.x = v258.matrix.top.x * 0.8999999761581421 + v249.matrix.top.x * 0.1000000014901161;
            v60->__parent.__parent.xyz->matrix.matrix.top.y = v258.matrix.top.y * 0.8999999761581421 + v249.matrix.top.y * 0.1000000014901161;
            v60->__parent.__parent.xyz->matrix.matrix.top.z = v258.matrix.top.z * 0.8999999761581421 + v249.matrix.top.z * 0.1000000014901161;
            v60->__parent.__parent.xyz->matrix.matrix.at.x = v258.matrix.at.x * 0.8999999761581421 + v249.matrix.at.x * 0.1000000014901161;
            v60->__parent.__parent.xyz->matrix.matrix.at.y = v258.matrix.at.y * 0.8999999761581421 + v249.matrix.at.y * 0.1000000014901161;
            v60->__parent.__parent.xyz->matrix.matrix.at.z = v258.matrix.at.z * 0.8999999761581421 + v249.matrix.at.z * 0.1000000014901161;
          }
          else
          {
            point.x = -x1.x;
            point.y = -y1;
            point.z = -z1;
            CMatrix__Unknown9(&v60->__parent.__parent.xyz->matrix, point.x, point.y, point.z);
          }
            }
            
        }
}