bool CRope::Create(CEntity *pRopeEntity, uint8_t ropeType, CVector vecStartPos, bool bExpires, uint8_t segmentCount, uint8_t flags, CEntity *pEntityToAttach, uint32_t expireTime)
{
	uiID = 0;
	v11 = (CEntity **)&pRopePool[0].pRopeEntity;
	while ( !*(v11 + 37) || *v11 != pRopeEntity )
	{
		v11 += 202;
		++uiID;
		if ( (signed int)v11 >= (signed int)&end_ropes_array )
		{
			uint32_t
				id = 0;
			for(; id < ROPES_MAX && pRopePool[id].ropeType; id++)
			{
				if ( (signed int)v13 >= (signed int)&end_ropes_pool )
					return 0;
			}
			pRope = &pRopePool[id];
		
			pRope->pRopeEntity = pRopeEntity;
			pRope->vecRopeSegments[0] = vecStartPos;
			pRope->vecRopeSegmentsReleased[0] = CVector(0.0f, 0.0f, 0.0f);
			pRope->flags = pRope->flags & 0xF9 | 4 * (flags & 1) | 1;
			pRope->segmentCount = segmentCount;
			pRope->groundHeight = 0;
			pRope->pRopeAttacherObject = 0;
			pRope->pAttachedEntity = 0;

			if ( pEntity && (pEntity->entityInfo & 7) == 2 )
				pRope->ropeLength = 0.9f;
			else
				pRope->ropeLength = 0.5f;
				
			pRope->flags2 = 0;
			pRope->Entity = pEntityToAttach;
			pRope->ropeType = ropeType;
			if ( pEntity )
				CEntity__registerReference(pRope->Entity);
			if ( bExpires )
				pRope->expireTime = expireTime + currentTime;
			else
				pRope->expireTime  = 0;
			switch(pRope->ropeType)
			{
				case 4:
					pRope->fMass = 50.0f;			// i think i need to exchange these two
					pRope->length = 1.6129032f;
				break;
				case 5:
				case 6:
				case 7:
					pRope->fMass = 68.0f;
					pRope->length = 2.1935484f;
				break;
				case 3:
					pRope->fMass = 10.0f;
					pRope->length = 0.32258064f;
				break;
				default:
					pRope->fMass = 20.0f;
					pRope->length = 0.6451613f;
				break;
			}
			if(ropeType < 4 || ropeType > 7)
			{
				uint32_t
					curSegment = 1;
				for(; curSegment < 25; curSegment++) 
				{
					if(IsEven(curSegment))
					{
						pRope->vecRopeSegments[curSegment+1].x = pRope->vecRopeSegments[curSegment].x - pRope->length; 
					}
					else
					{
						pRope->vecRopeSegments[curSegment+1].x = pRope->length + pRope->vecRopeSegments[curSegment].x;
					}
					pRope->vecRopeSegments[curSegment+1].y = pRope->vecRopeSegments[curSegment].y;
					pRope->vecRopeSegments[curSegment+1].z = pRope->vecRopeSegments[curSegment].z;
					pRope->vecRopeReleased[curSegment] = CVector(0.0f, 0.0f, 0.0f);
				}
				if ( curSegment < 32 )
				{
					for(; curSegment < 32; curSegment++)
					{
						if(IsEven(curSegment))
						{
							pRope->vecRopeSegments[curSegment+1].x = pRope->vecRopeSegments[curSegment].x - pRope->length;
						}
						else
						{
							pRope->vecRopeSegments[curSegment+1].x = pRope->length + pRope->vecRopeSegments[curSegment].x;
						}
						pRope->vecRopeSegments[curSegment+1].y = pRope->vecRopeSegments[curSegment].y;
						pRope->vecRopeSegments[curSegment+1].z = pRope->vecRopeSegments[curSegment].z
						/*
						v182 = (int)(v170 + 388);
						*((_DWORD *)v170 + 97) = 0;
						*((_DWORD *)v170 + 98) = 0;
						v170 += 12;
						*(_DWORD *)(v182 + 8) = 0;
						*/
					}
				}
			}
			else
			{
				uint32_t
					curSegment = 1;
				for(; curSegment < 25; curSegment++)
				{
					pRope->vecRopeSegments[curSegment].z = pRope->vecRopeSegments[curSegment].y = pRope->vecRopeSegments[curSegment].x;
					pRope->vecRopeSegments[curSegment+1].x = pRope->vecRopeSegments[curSegment].x - pRope->length;
					pRope->vecRopeSegments[curSegment+1].y = pRope->vecRopeSegments[curSegment].y;
					pRope->vecRopeSegments[curSegment+1].z = pRope->vecRopeSegments[curSegment].z;
				}

				if(curSegment < 32)
				{
					v50 = (char *)&dword_B768B4 + 12 * i + ropeArrayPos;
					for(uint32_t j = 32 - i; j; j--)
					{
						v52 = *((float *)v50 - 1);
						v53 = (int)(v50 + 4);
						v54 = *(float *)v50 - *(float *)(ropeArrayPos + 12020676);
						*((_DWORD *)v50 + 1) = *((_DWORD *)v50 - 2);
						*((_DWORD *)v50 + 2) = LODWORD(v52);
						v50 += 12;
						*(_DWORD *)(v53 + 8) = LODWORD(v54);
					}
					CreateRopeAttacherObject(pRope);
					return 1;
				}
			}
			CreateRopeAttacherObject(pRope);
			return 1;
		}
	}
	pRope->flags |= 1;
	pRope->segmentCount = segmentCount;
	for(uint32_t i = 0; i <= pRope->segmentCount; i++)
	{
		pRope->vecRopeSegments = vecStartPos;
		pRope->vecRopeSegmentsReleased = CVector(0.0f, 0.0f, 0.0f);
	}
	CreateRopeAttacherObject(pRope);
	return 1;
}

CObject * CRope::CreateRopeAttacherObject()
{
	CObject 
		*pRopeAttacher = this->pRopeAttacherObject;
	if(!pRopeAttacher)
	{
		switch ( this->ropeType )
		{
			case 1:
			case 4:
			case 6:
			case 7:
				id = (unsigned __int16)miIndusMagnet;
			break;
			case 2:
				id = (unsigned __int16)miCjPlayerHarness;
			break;
			case 5:
				id = (unsigned __int16)mimDynWrekingBall;
			break;
			case 3:
				id = (unsigned __int16)miMiniMagnet;
			break;
			default:
				return pRopeObject;
		}
		if ( (ObjectPool__allocate)(380) )
			pRopeAttacher = (CObject *)CObject__constructor2(id, 1);
		else
			pRopeAttacher = 0;
		if(pRopeAttacher)
		{
			CEntity__registerReference(&pRopeAttacher);
			pRopeAttacher->SetPosition(vecRopeStartPos);
			LOBYTE((*ppRopeAttcher)->__parent.pRealTimeShadowInstance) = 5;
			pRopeAttacher->SetStatic(false);
			pRopeAttacher->__parent.__parent.nImmunities |= 0x2000000u;
			pAttachedEntity = 0;
			flags2 = 0;
			return CWorld__add(pRopeAttacher);
		}
	}
	return NULL;
}

inline bool CRope::IsSetToExpire() const
{
	return !(m_flags & 1);
}

inline bool CRope::IsEntityAttached() const
{
	return m_flags & 4;
}

const float
	RopeVisibleDistance = 200.0f;

void CRope::Update()
{
	scaleFactor = pow(dt, 0.800000011920929);
	CVector
		vecCamRopeDist = camera->GetPosition() - m_vecSegments[0];
	if ( vecCamRopeDist.LengthSquared() < RopeVisibleDistance * RopeVisibleDistance )
	{
		if(IsSetToExpire())
		{
			if(GetCurrentTime() > m_expireTime)
			{
				m_vecSegmentsReleased[0].z -= dt * 0.001500000013038516;
				m_vecSegments[0] += dt * vecSegmentsReleased[0];
			}
		}
		if ( IsEntityAttached() )                            // Is Any Entity Attached
		{
			if ( (dword_B7CB4C & 7) == 2 )
				m_groundHeight = CWorld__findGroundZFor3DCoord(m_vecRopeSegments[0], 0, 0);
			if ( m_pAttachedEntity )
			{
				if ( (m_pAttachedEntity->entityInfo & 7) == 2
				|| (attachedEntityID = m_pAttachedEntity->GetModelIndex(), attachedEntityID == (unsigned __int16)miKmbContainerRed)
				|| attachedEntityID == (unsigned __int16)miKmbContainerYel
				|| attachedEntityID == (unsigned __int16)miKmbContainerBlue
				|| attachedEntityID == (unsigned __int16)miKmbContainerOpen )// if either vehicle or container
				{
					m_groundHeight = m_pRopeAttacherObject->GetPosition()->z - 0.5;
				}
			}
		}
		v13 = m_segmentCount + 1;
		if ( v13 < 32 )
		{
			v14 = 32 - v13;
			do
			{
				CVector
					vecSegmentSaved = m_vecSegments[id];
				
				m_vecReleasedSegments[id].x += (long double)((rand() & 15) - 8) * 0.001000000047497451;
				m_vecReleasedSegments[id].y += (long double)((rand() & 15) - 8) * 0.001000000047497451;
				
				m_vecReleasedSegments[id] *= scaleFactor;
				m_vecReleasedSegments[id] += m_vecReleasedSegments[id-1] * (1.0f - scaleFactor);
				
				m_vecReleasedSegments[id].z -= dt * 0.1500000059604645;
				
				if ( IsEntityAttacher() )
				{
					float
						fZ = m_groundHeight + 0.300000011920929;
					if ( m_vecReleasedSegments[id].z <= fZ )
						m_vecReleasedSegments[id].z = fZ;
				}
				CVector
					vecDifference = m_vecSegments[id] - m_vecSegments[id+1];
				m_vecSegments[id] += vecDifference * (m_length / vecDifference.Length);
				m_vecReleasedSegments[id] = (m_vecSegments[id] - m_vecSegmentSaved) / dt;
				--v14;
			}
			while ( v14 );
		}
		if ( m_ropeType != 1 && m_ropeType != 2 )
		{
			v44 = m_ropeType == 4;
			if ( m_ropeType == 4 )
				goto LABEL_37;
			if ( m_ropeType != 5 && m_ropeType != 7 && m_ropeType != 6 && m_ropeType != 3 )
				goto LABEL_249;
		}
		LABEL_37:
		if ( (m_ropeType != 4 || controlledCraneNumber != 1)
		&& (m_ropeType != 5 || controlledCraneNumber != 2)
		&& (m_ropeType != 7 || controlledCraneNumber != 3)
		&& (m_ropeType != 6 || controlledCraneNumber != 4) )
		{
			if ( m_ropeType == 1 )
				goto LABEL_69;
			if ( m_ropeType != 3 && m_ropeType != 2 )
				goto LABEL_80;
		}
		if ( m_ropeType != 1 && m_ropeType != 2 && m_ropeType != 3 )
		{
			if ( bCraneControls_Enabled_Up )
			{
				bDoRaiseWrench = getPad(0)->shiftPressed[0];
				LODWORD(v215) = bDoRaiseWrench;
				v207 = (long double)bDoRaiseWrench * dt * 0.000009999999747378752;
				if ( m_ropeType == 4 || m_ropeType == 5 || m_ropeType == 7 || m_ropeType == 6 )
				{
					if ( v207 > 0.0 )
					{
						if ( v207 + m_ropeLength < 0.8999999761581421 )
							PlayAudioEventForCraneAction((CAudioManager *)&audio, 104, pThisPtr->Entity, 0, 1.0);
					}
				}
				m_ropeLength += v207;
			}
		if ( bCraneControls_Enabled_Down )
		{
			bDoLowerWrench = getPad(0)->spacePressed[0];
			LODWORD(v215) = bDoLowerWrench;
			LowerLevelY = (long double)bDoLowerWrench * dt * 0.000009999999747378752;
			if ( m_ropeType == 4 || m_ropeType == 5 || m_ropeType == 7 || m_ropeType == 6 )
			{
				if ( LowerLevelY > 0.0 )
				{
					if ( pThisPtr->ropeLength - LowerLevelY > 0.009999999776482582 )
						PlayAudioEventForCraneAction((CAudioManager *)&audio, 104, pThisPtr->Entity, 0, 1.0);
				}
			}
			pThisPtr->ropeLength -= LowerLevelY;
			LABEL_79:
		}
	LABEL_80:
	v56 = v43->ropeLength;
	if ( v56 < 0.009999999776482582 )
		v56 = 0.009999999776482582;
	v43->ropeLength = v56;
	if ( v56 > 0.8999999761581421 )
		v56 = 0.8999999761581421;
	v43->ropeLength = v56;
	ppAttachedEntity = &v43->pAttachedEntity;
	LODWORD(scaleFactor) = (char *)v43 + 792;
	if ( v58 )
	{
		fMass = v58->fMass;
		if ( v58->__parent.m_nModelIndex == 428 )
			fMass = 750.0;                        // if it's Securicar(vehicle)
		v60 = fMass * flt_8CD89C * 0.0008333333535119891 + flt_8CD898;
		if ( v60 <= 0.5 )
		{
			entityWeightFactor? = v60;
			LOBYTE(v58->CollisionFlags) = 0;
		}
		else
		{
			LODWORD(entityWeightFactor?) = 0x3F000000u;
			LOBYTE(v58->CollisionFlags) = 0;
		}
	}
	else
	{
		v58 = &v43->pRopeAttacherObject->__parent;
		entityWeightFactor? = flt_8CD898;
		v58->__parent.BitFlags |= 1u;
	}
	if ( v58 )
	{
		v61 = v58->__parent.__parent.xyz;
		if ( v61 )
			RopeStartVector = &v61->matrix.pos;
		else
			RopeStartVector = &v58->__parent.__parent.placement.pos;
		pRope = pThisPtr;
		if ( CRope__Adjust(pAttachedEntity->GetPosition(), 0.1f, &a6))
		{
		v64 = v58->__parent.__parent.xyz;
		if ( v64 )
		{
		LODWORD(v64->matrix.pos.x) = (_DWORD)a6.x;
		v58->__parent.__parent.xyz->matrix.pos.y = _y2;
		v58->__parent.__parent.xyz->matrix.pos.z = _z2;
		}
		else
		{
		v65 = _y2;
		v66 = _z2;
		LODWORD(v58->__parent.__parent.placement.pos.x) = (_DWORD)a6.x;
		v58->__parent.__parent.placement.pos.y = v65;
		v58->__parent.__parent.placement.pos.z = v66;
		}
		v69 = (char *)&v58->vecVelocity;
		v70 = v58->vecVelocity.y;
		v71 = v58->vecVelocity.z;
		a6.x = v58->vecVelocity.x;
		v68 = (CPhysical *)pRope->Entity;
		_z2 = v71;
		v67 = v68->__parent.__parent.xyz;
		_y2 = v70;
		if ( v67 )
		v72 = (int)&v67->matrix.pos;
		else
		v72 = (int)&v68->__parent.__parent.placement;
		v74 = pRope->vecRopeSegments[0].z - *(float *)(v72 + 8);
		v75 = pRope->vecRopeSegments[0].y - *(float *)(v72 + 4);
		point.x = pRope->vecRopeSegments[0].x - *(float *)v72;
		LODWORD(v76.x) = (_DWORD)point.x;
		point.y = v75;
		v76.y = point.y;
		point.z = v74;
		v76.z = point.z;
		sub_542CE0(v68, &releasedRope.x, COERCE_RWV3D(v76.x));
		v73 = pRope->ropeType;
		if ( v73 == 4 || v73 == 5 || v73 == 6 || v73 == 7 )
		{
		LODWORD(point.x) = 0;
		LODWORD(point.y) = 0;
		LODWORD(point.z) = 0;
		LODWORD(releasedRope.x.x) = 0;
		LODWORD(_releasedRope.y) = 0;
		LODWORD(_releasedRope.z) = 0;
		}
		v77 = v58->__parent.__parent.xyz;
		a6.x -= - releasedRope.x.x;
		_y2 -= _releasedRope.y;
		_z2 -= _releasedRope.z;
		if ( v77 )
		v78 = &v77->matrix.pos;
		else
		v78 = &v58->__parent.__parent.placement.pos;
		v80 = v78->z - pRope->vecRopeSegments[0].z;
		v81 = v78->y - pRope->vecRopeSegments[0].y;
		x1 = v78->x - pRope->vecRopeSegments[0].x;
		y1 = v81;
		z1 = v80;
		CVector__normalize(&x1);
		v79 = a6.x;
		dist_squared = z1 * _z2 + x1 * a6.x + y1 * _y2;
		velocity_z = dist_squared;
		if ( dist_squared > 0.0 )
		{
		v79 = v79 - x1 * velocity_z;
		_y2 = _y2 - velocity_z * y1;
		_z2 = _z2 - velocity_z * z1;
		}
		velocity_z = _releasedRope.z + _z2 - v58->vecVelocity.z;
		velocity_final_y = _releasedRope.y + _y2 - v58->vecVelocity.y;
		v83 = v79 + releasedRope.x.x - *(float *)v69;
		v84 = 1.0 - entityWeightFactor?;
		v215 = velocity_z * v84;
		v233 = velocity_final_y * v84;
		outPoint.x = v83 * v84;
		v85 = v215 + v58->vecVelocity.z;
		v86 = v233 + v58->vecVelocity.y;
		point.x = outPoint.x + *(float *)v69;
		*(_DWORD *)v69 = (_DWORD)point.x;
		point.y = v86;
		v58->vecVelocity.y = point.y;
		point.z = v85;
		v87 = velocity_z;
		v58->vecVelocity.z = point.z;
		v88 = (RwV3D *)&pRope->Entity[1];
		v233 = v87 * entityWeightFactor?;
		v215 = velocity_final_y * entityWeightFactor?;
		outPoint.x = v83 * entityWeightFactor?;
		v89 = v88->z - v233;
		v90 = v88->y - v215;
		point.x = v88->x - outPoint.x;
		LODWORD(v88->x) = (_DWORD)point.x;
		point.y = v90;
		v88->y = point.y;
		point.z = v89;
		v88->z = point.z;
		if ( *ppAttachedEntity )
		{
		sub_59BCF0(v58->__parent.__parent.xyz);
		v264 = 0;
		point.x = -x1;
		point.y = -y1;
		point.z = -z1;
		sub_59B7E0(point.x, point.y, point.z);
		sub_59BCF0(v58->__parent.__parent.xyz);
		v58->__parent.__parent.xyz->matrix.right.x = v254 * 0.8999999761581421 + v245 * 0.1000000014901161;
		v58->__parent.__parent.xyz->matrix.right.y = v255 * 0.8999999761581421 + v246 * 0.1000000014901161;
		v58->__parent.__parent.xyz->matrix.right.z = v256 * 0.8999999761581421 + v247 * 0.1000000014901161;
		v58->__parent.__parent.xyz->matrix.top.x = v257 * 0.8999999761581421 + v248 * 0.1000000014901161;
		v58->__parent.__parent.xyz->matrix.top.y = v258 * 0.8999999761581421 + v249 * 0.1000000014901161;
		v58->__parent.__parent.xyz->matrix.top.z = v259 * 0.8999999761581421 + v250 * 0.1000000014901161;
		v58->__parent.__parent.xyz->matrix.at.x = v260 * 0.8999999761581421 + v251 * 0.1000000014901161;
		v58->__parent.__parent.xyz->matrix.at.y = v261 * 0.8999999761581421 + v252 * 0.1000000014901161;
		v58->__parent.__parent.xyz->matrix.at.z = v262 * 0.8999999761581421 + v253 * 0.1000000014901161;
		CMatrix__destructor(&v245);
		v264 = -1;
		CMatrix__destructor(&v254);
		}
		else
		{
		point.x = -x1;
		point.y = -y1;
		point.z = -z1;
		sub_59B7E0(point.x, point.y, point.z);
		}
		}
		if ( *ppAttachedEntity )
		{
		sub_59BBC0((*ppAttachedEntity)->__parent.__parent.__parent.xyz);
		v96 = &(*ppAttachedEntity)->__parent.vecVelocity;
		pRopeObjectVelocity = &pRope->pRopeAttacherObject->__parent.vecVelocity;
		pRopeObjectVelocity->x = v96->x;
		pRopeObjectVelocity->y = v96->y;
		pRopeObjectVelocity->z = v96->z;
		v98 = *ppAttachedEntity;
		AttachedEntityBBox_Z = modelPtrs[(*ppAttachedEntity)->__parent.__parent.m_nModelIndex]->colModel->bbox.inf.z;
		LODWORD(point.x) = 0;
		point.z = AttachedEntityBBox_Z;
		LODWORD(point.y) = 0;
		v100 = transformPoint(&outPoint, v98->__parent.__parent.__parent.xyz, &point);
		v95 = v100->z;
		v92 = pRope->pRopeAttacherObject;
		v94 = v100->y;
		v93 = v100->x;
		v91 = v92->__parent.__parent.__parent.xyz;
		if ( v91 )
		{
		v91->matrix.pos.x = v93;
		v92->__parent.__parent.__parent.xyz->matrix.pos.y = v94;
		v92->__parent.__parent.__parent.xyz->matrix.pos.z = v95;
		}
		else
		{
		v92->__parent.__parent.__parent.placement.pos.x = v93;
		v92->__parent.__parent.__parent.placement.pos.y = v94;
		v92->__parent.__parent.__parent.placement.pos.z = v95;
		}
		}
	}
	pRopeObject = pThisPtr;
	v102 = pThisPtr->vecRopeStartPos.x;
	v103 = pThisPtr->vecRopeStartPos.z;
	y1 = pThisPtr->vecRopeStartPos.y;
	v104 = *ppAttachedEntity == 0;
	x1 = v102;
	z1 = v103;
	if ( !v104 )
	{
	v105 = pThisPtr->ropeType;
	if ( v105 == 4 && controlledCraneNumber == 1
	|| v105 == 5 && controlledCraneNumber == 2
	|| v105 == 7 && controlledCraneNumber == 3
	|| v105 == 6 && controlledCraneNumber == 4
	|| v105 == 1
	|| v105 == 3
	|| v105 == 2 )
	{
	if ( bCraneControls_Enabled_Release )
	{
	v106 = getPad(0);
	if ( (unsigned __int16)CPad__testFireButton(v106) )
	CRope__ReleaseRope(pRopeObject);
	}
	}
	v107 = *ppAttachedEntity;
	if ( *ppAttachedEntity )
	{
	v108 = v107->__parent.__parent.nImmunities;
	if ( v108 & 0x20000000 )
	{
	v107->__parent.__parent.nImmunities = v108 & 0xFDFFFFFF;
	(*ppAttachedEntity)->__parent.__parent.nImmunities &= 0x7FFFFFFFu;
	v109 = pRopeObject->pRopeAttacherObject;
	*ppAttachedEntity = 0;
	v109->__parent.__parent.BitFlags |= 1u;
	pRopeObject->flags2 = 60;
	}
	}
	v110 = *ppAttachedEntity;
	if ( *ppAttachedEntity )
	{
	if ( (v110->__parent.__parent.entityInfo & 7) == 2 )
	{
	if ( v110[4].__parent.pad2 == 9 )
	{
	if ( v110[3].__parent.fBuoyancyConstant )
	CRope__ReleaseRope(pRopeObject);
	}
	}
	}
	goto LABEL_249;
	}
	v111 = pThisPtr->flags2;
	if ( v111 )
	{
	pThisPtr->flags2 = v111 - 1;
	goto LABEL_249;
	}
	v112 = pThisPtr->ropeType - 1;
	v227 = 0;
	v220 = 0;
	v226 = 0;
	v221 = 0;
	v218 = 0;
	v217 = 0;
	v219 = 0;
	v222 = 0;
	switch ( v112 )
	{
	case 3:
	v220 = 1;
	goto LABEL_149;
	case 6:
	v226 = 1;
	goto LABEL_198;
	case 5:
	v221 = 1;
	goto LABEL_149;
	case 2:
	v218 = 1;
	v217 = 1;
	v219 = 1;
	v222 = 1;
	v227 = 1;
	goto LABEL_149;
	case 0:
	LABEL_149:
	v113 = VehiclePool;
	v114 = *((_DWORD *)VehiclePool + 2);
	if ( !v114 )
	goto LABEL_192;
	v115 = 0xA18 * v114;
	break;
	case 1:
	pPedPool = PedPool;
	v143 = PedPool->size;
	if ( !v143 )
	goto LABEL_249;
	v144 = 1988 * v143;
	while ( 1 )
	{
	v145 = *(_BYTE *)(v143-- + pPedPool->flags - 1);
	v144 -= 0x7C4u;
	if ( v145 >= 0 )
	{
	v146 = (CPed *)(v144 + pPedPool->objects);
	if ( v144 + pPedPool->objects )
	{
	if ( v146->pad10[1] != 0x37 )
	{
	if ( !CPed__isPlayer((CPed *)(v144 + pPedPool->objects)) )
	{
	v147 = *(_DWORD *)&v146->pedFlags[0];
	if ( !(BYTE1(v147) & 1) )
	{
	v148 = v146->__parent.__parent.__parent.xyz;
	v149 = v148 ? &v148->matrix.pos : &v146->__parent.__parent.__parent.placement;
	x2 = v149->x;
	y2 = v149->y;
	z2 = v149->z;
	point.x = x2;
	point.y = y2;
	point.z = z2;
	if ( sqrt((z2 - z1) * (z2 - z1) + (y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1)) < 2.5 )
	break;
	}
	}
	}
	}
	}
	if ( !v143 )
	goto LABEL_249;
	}
	v153 = (CObject **)LODWORD(scaleFactor);
	v156 = (CObject **)LODWORD(scaleFactor);
	*LODWORD(scaleFactor) = v146;
	CEntity__registerReference(v156);
	(*v153)->__parent.__parent.nImmunities |= 0x80000000u;
	v157 = *v153;
	v158 = modelPtrs[v146->__parent.__parent.m_nModelIndex]->colModel->bbox.inf.z;
	LODWORD(point.x) = 0;
	point.z = v158;
	LODWORD(point.y) = 0;
	v159 = transformPoint(&outPoint, v157->__parent.__parent.__parent.xyz, &point);
	v154 = pThisPtr;
	CPhysical__SetPosition(&pThisPtr->pRopeAttacherObject->__parent, v159);
	v155 = z1 - _z2;
	v154->pRopeAttacherObject->__parent.__parent.BitFlags &= 0xFFFFFFFEu;
	if ( v155 > 0.0 )
	{
	v160 = v154->ropeLength - v155 * 1.0 / v154->fMass - 1.0 / v154->fMass * v154->length;
	v154->ropeLength = min(v160, 0.009999999776482582);
	}
	goto LABEL_249;
	default:
	goto LABEL_249;
	}
	while ( 1 )
	{
	v116 = *(_BYTE *)(v114-- + *((_DWORD *)v113 + 1) - 1);
	v115 -= 2584;
	if ( v116 >= 0 )
	{
	v117 = (CObject *)(v115 + *(_DWORD *)v113);
	if ( v115 + *(_DWORD *)v113 )
	{
	v118 = *(_DWORD *)(v115 + 1428 + *(_DWORD *)v113);
	if ( !v118
	|| v118 == 9 && !*(_DWORD *)(v115 + 1120 + *(_DWORD *)v113)
	|| v118 == 1
	|| (v119 = *(_WORD *)(v115 + 34 + *(_DWORD *)v113), v119 == 473)
	|| v119 == 539 )
	{
	if ( !(*(_DWORD *)(v115 + 64 + *(_DWORD *)v113) & 0x20000000) )
	{
	if ( *(_BYTE *)(v115 + 1069 + *(_DWORD *)v113) & 0x10 )
	{
	if ( !v227 || *(_WORD *)(v115 + 34 + *(_DWORD *)v113) == 564 )
	{
	if ( !(*(_BYTE *)(v115 + 1192 + *(_DWORD *)v113) & 0x60) )
	{
	v120 = *(CMatrix **)(v115 + 20 + *(_DWORD *)v113);
	point.z = modelPtrs[*(_WORD *)(v115 + 34 + *(_DWORD *)v113)]->colModel->bbox.inf.z;
	LODWORD(point.x) = 0;
	LODWORD(point.y) = 0;
	v121 = transformPoint(&outPoint, v120, &point);
	v122 = v121->x;
	v123 = v121->y;
	v124 = v121->z;
	a6.x = v122;
	_y2 = v123;
	_z2 = v124;
	if ( sqrt((v122 - x1) * (v122 - x1) + (v124 - z1) * (v124 - z1) + (v123 - y1) * (v123 - y1)) < 2.5 )
	break;
	}
	}
	}
	}
	}
	}
	}
	if ( !v114 )
	goto LABEL_191;
	}
	v125 = (CObject **)LODWORD(scaleFactor);
	v126 = (CObject **)LODWORD(scaleFactor);
	*LODWORD(scaleFactor) = v117;
	CEntity__registerReference(v126);
	(*v125)->__parent.__parent.nImmunities |= 0x80000000u;
	if ( (v117->__parent.__parent.entityInfo & 0xF8) == 16 )
	v117->__parent.__parent.entityInfo = v117->__parent.__parent.entityInfo & 7 | 0x18;
	v133 = *v125;
	point.z = modelPtrs[v117->__parent.__parent.m_nModelIndex]->colModel->bbox.inf.z;
	LODWORD(point.x) = 0;
	LODWORD(point.y) = 0;
	v134 = transformPoint(&outPoint, v133->__parent.__parent.__parent.xyz, &point);
	v132 = v134->z;
	v128 = pThisPtr;
	v131 = v134->y;
	v129 = pThisPtr->pRopeAttacherObject;
	v130 = v134->x;
	v127 = v129->__parent.__parent.__parent.xyz;
	if ( v127 )
	{
	v127->matrix.pos.x = v130;
	v129->__parent.__parent.__parent.xyz->matrix.pos.y = v131;
	v129->__parent.__parent.__parent.xyz->matrix.pos.z = v132;
	}
	else
	{
	v129->__parent.__parent.__parent.placement.pos.x = v130;
	v129->__parent.__parent.__parent.placement.pos.y = v131;
	v129->__parent.__parent.__parent.placement.pos.z = v132;
	}
	v135 = z1 - _z2;
	v128->pRopeAttacherObject->__parent.__parent.BitFlags &= 0xFFFFFFFEu;
	if ( v135 > 0.0 )
	{
	v137 = v135;
	invMass = 1.0 / v128->fMass;
	v136 = v128->ropeLength - v137 * invMass - invMass * v128->length;
	UNDEF(v139);
	v128->ropeLength = v136;
	if ( v140 | v141 )
	v136 = 0.009999999776482582;
	v128->ropeLength = v136;
	}
	LABEL_191:
	ppAttachedEntity = (CObject **)LODWORD(scaleFactor);
	LABEL_192:
	if ( v220 || v221 || v218 || v217 || v219 || v222 )
	{
	LABEL_198:
	if ( !*ppAttachedEntity )
	{
	pObjectPool = (int)ObjectPool;
	v162 = *((_DWORD *)ObjectPool + 2);
	LODWORD(v215) = ObjectPool;
	if ( v162 )
	{
	v163 = 0x19C * v162;
	do
	{
	--v162;
	v163 -= 412;
	v164 = (*(_BYTE *)(v162 + *(_DWORD *)(pObjectPool + 4)) & 0x80) == 0;
	LODWORD(v233) = v163;
	if ( v164 )
	{
	v165 = (CPhysical *)(v163 + *(_DWORD *)pObjectPool);
	if ( v163 + *(_DWORD *)pObjectPool )
	{
	if ( v165[1].__parent.__parent.__vmt & 0x40000 )
	{
	if ( v220
	&& ((v166 = v165->__parent.m_nModelIndex, v166 == (unsigned __int16)miKmbContainerRed)
	|| v166 == (unsigned __int16)miKmbContainerYel
	|| v166 == (unsigned __int16)miKmbContainerBlue
	|| v166 == (unsigned __int16)miBarrel4
	|| v166 == (unsigned __int16)miKmbContainerOpen)
	|| v226 && (v166 = v165->__parent.m_nModelIndex, v166 == (unsigned __int16)miBlockpallet)
	|| v221
	&& ((v166 = v165->__parent.m_nModelIndex, v166 == (unsigned __int16)miDynQuarryrock01)
	|| v166 == (unsigned __int16)miDynQuarryrock02
	|| v166 == (unsigned __int16)miDynQuarryrock03
	|| v166 == (unsigned __int16)miDeadTiedCop)
	&& !LODWORD(v165->vecCollisionLastPos.z)
	|| v218 && (v166 = v165->__parent.m_nModelIndex, v166 == (unsigned __int16)miWongDish)
	|| v217 && (v166 = v165->__parent.m_nModelIndex, v166 == (unsigned __int16)word_8CD734)
	|| v219 && (v166 = v165->__parent.m_nModelIndex, v166 == (unsigned __int16)word_8CD738)
	|| v222 && (v166 = v165->__parent.m_nModelIndex, v166 == (unsigned __int16)word_8CD73C) )
	{
	fAttachedEntityColBoxZ = modelPtrs[v166]->colModel->bbox.inf.z;
	v170 = v165->__parent.__parent.xyz;
	point.z = fAttachedEntityColBoxZ;
	LODWORD(point.x) = 0;
	LODWORD(point.y) = 0;
	v171 = transformPoint(&outPoint, v170, &point);
	v172 = v171->x;
	v173 = v171->y;
	v174 = v171->z;
	a6.x = v172;
	_z2 = v174;
	v167 = v165->__parent.m_nModelIndex;
	v168 = 0;
	_y2 = v173;
	if ( v167 == (unsigned __int16)miKmbContainerRed
	|| v167 == (unsigned __int16)miKmbContainerYel
	|| v167 == (unsigned __int16)miKmbContainerBlue
	|| v167 == (unsigned __int16)miKmbContainerOpen )
	{
	v175 = modelPtrs[v167]->colModel->bbox.inf.z;
	v176 = v165->__parent.__parent.xyz;
	v240 = -v175;
	LODWORD(v238.x) = 0;
	v239 = 0;
	transformPoint(&releasedRope.x, v176, &v238);
	if ( _releasedRope.z > (long double)_z2 )
	{
	LODWORD(a6.x) = (_DWORD)releasedRope.x.x;
	_y2 = _releasedRope.y;
	_z2 = _releasedRope.z;
	v168 = 2;
	}
	v178 = v165->__parent.__parent.xyz;
	releasedRopePos.x_scaled.x = modelPtrs[v165->__parent.m_nModelIndex]->colModel->bbox.inf.z;
	v242 = 0;
	v243 = 0;
	v177 = transformPoint(&v263, v178, &releasedRopePos.x_scaled);
	releasedRope.x.x = v177->x;
	_releasedRope.y = v177->y;
	_releasedRope.z = v177->z;
	if ( _releasedRope.z > (long double)_z2 )
	{
	v179 = LODWORD(v177->y);
	a6.x = v177->x;
	v180 = LODWORD(v177->z);
	LODWORD(_y2) = v179;
	LODWORD(_z2) = v180;
	v168 = 3;
	}
	v182 = modelPtrs[v165->__parent.m_nModelIndex]->colModel->bbox.inf.z;
	v183 = v165->__parent.__parent.xyz;
	v236 = 0;
	velocity_final_x.x = -v182;
	LODWORD(v237) = 0;
	v181 = transformPoint(&v244, v183, &velocity_final_x);
	releasedRope.x.x = v181->x;
	_releasedRope.y = v181->y;
	_releasedRope.z = v181->z;
	if ( _releasedRope.z > (long double)_z2 )
	{
	v184 = v181->x;
	v185 = LODWORD(v181->y);
	v186 = LODWORD(v181->z);
	a6.x = v184;
	LODWORD(_y2) = v185;
	LODWORD(_z2) = v186;
	v168 = 1;
	}
	}
	if ( sqrt((_z2 - z1) * (_z2 - z1) + (_y2 - y1) * (_y2 - y1) + (a6.x - x1) * (a6.x - x1)) < 2.5 )
	{
	*ppAttachedEntity = (CObject *)v165;
	CEntity__registerReference(ppAttachedEntity);
	(*ppAttachedEntity)->__parent.__parent.nImmunities |= 0x80000000u;
	v189 = *ppAttachedEntity;
	v237 = modelPtrs[v165->__parent.m_nModelIndex]->colModel->bbox.inf.z;
	LODWORD(velocity_final_x.x) = 0;
	v236 = 0;
	v190 = transformPoint(&v244, v189->__parent.__parent.__parent.xyz, &velocity_final_x);
	v188 = pThisPtr;
	CPhysical__SetPosition(&pThisPtr->pRopeAttacherObject->__parent, v190);
	v188->pRopeAttacherObject->__parent.__parent.BitFlags &= 0xFFFFFFFEu;
	v187 = (*ppAttachedEntity)->__parent.__parent.BitFlags;
	if ( v187 & 4 || v187 & 0x40000 )
	{
	(*(void (__stdcall **)(_DWORD))((*ppAttachedEntity)->__parent.__parent.__parent.__vmt + 16))(0);
	CPhysical__AddToMovingList(*ppAttachedEntity);
	}
	v191 = z1 - _z2;
	(*ppAttachedEntity)->__parent.__parent.nImmunities |= 0x2000000u;
	if ( v191 > 0.0 )
	{
	v193 = v191;
	invMass_ = 1.0 / v188->fMass;
	v192 = v188->ropeLength - v193 * invMass_ - invMass_ * v188->length;
	UNDEF(v195);
	v188->ropeLength = v192;
	if ( v196 | v197 )
	v192 = 0.009999999776482582;
	v188->ropeLength = v192;
	}
	if ( v168 > 0 )
	{
	do
	{
	v198 = (*ppAttachedEntity)->__parent.__parent.__parent.xyz;
	v199 = (*ppAttachedEntity)->__parent.__parent.__parent.xyz;
	point.x = v199->matrix.right.x;
	v200 = v199->matrix.right.y;
	v201 = v199->matrix.right.z;
	releasedRope.x.x = -point.x;
	point.y = v200;
	point.z = v201;
	v198->matrix.right.x = v198->matrix.at.x;
	v198->matrix.right.y = v198->matrix.at.y;
	v198->matrix.right.z = v198->matrix.at.z;
	v202 = (*ppAttachedEntity)->__parent.__parent.__parent.xyz;
	_releasedRope.y = -v200;
	v203 = _releasedRope.y;
	v202 = (CMatrix *)((char *)v202 + 32);
	v204 = -point.z;
	--v168;
	LODWORD(v202->matrix.right.x) = (_DWORD)releasedRope.x.x;
	_releasedRope.z = v204;
	v205 = _releasedRope.z;
	v202->matrix.right.y = v203;
	v202->matrix.right.z = v205;
	}
	while ( v168 );
	}
	break;
	}
	v163 = LODWORD(v233);
	pObjectPool = LODWORD(v215);
	}
	}
	}
	}
	}
	while ( v162 );
	}
	}
	}
	LABEL_249:
	JUMPOUT(*(int *)loc_558CAE);
	}
	LABEL_69:
	v53 = getPad(0);
	v52 = sub_53FC10(v53);
	v51 = 0;
	v49 = v52 == 0;
	v50 = v52 < 0;
	if ( v52 < 0 )
	{
	if ( bCraneControls_Enabled_Up )
	goto LABEL_ProcessRaiseWrench;
	v51 = 0;
	v49 = v52 == 0;
	v50 = v52 < 0;
	}
	if ( (unsigned __int8)(v50 ^ v51) | v49 || !bCraneControls_Enabled_Down )
	{
	v54 = pThisPtr;
	goto LABEL_76;
	}
	LABEL_ProcessRaiseWrench:
	LODWORD(v215) = v52;
	v54 = pThisPtr;
	pThisPtr->ropeLength = pThisPtr->ropeLength - (long double)v52 * dt * 0.000009999999747378752;
	LABEL_76:
	v55 = v54->ropeLength;
	if ( v55 > 0.8399999737739563 )
	v55 = 0.8399999737739563;
	v54->ropeLength = v55;
	goto LABEL_79;
	}
}

char __cdecl CRope::Unknown(int a1, float a2, RwV3D *a3, CVector *pVecSegmentReleased)
{
	id = 0;
	pCrane = (int)&pRopePool[0].pRopeEntity;
	while ( !*(_BYTE *)(pCrane + 37) || *(_DWORD *)pCrane != a1 )
	{
		pCrane += 808;
		++id;
		if ( pCrane >= (signed int)&end_ropes_array )
			return 0;
	}
	v7 = a2;
	if ( a2 >= 0.0 )
	{
		if ( v7 > 0.9990000128746033 )
			v7 = 0.9990000128746033;
	}
	else
	{
		v7 = 0.0;
	}
	CRope
		*pRope = &pRopePool[id];
	v9 = v7 * 31.0;
	*(_QWORD *)&v10 = (unsigned __int64)v9;
	v11 = v9 - (long double)(signed int)(unsigned __int64)v9;
	v10 = pRope->vecRopeSegments[(_DWORD)v10];
	v12 = v11 * pRope->vecRopeSegments[0].y;
	v13 = v11 * pRope->vecRopeSegments[0].z;
	v14 = (1.0 - v11) * ->x;
	v15 = (1.0 - v11) * v10->y;
	v16 = (1.0 - v11) * v10->z;
	v17 = v14 + v11 * pRope->vecRopeSegments[0].x;
	a3->x = v17;
	v18 = v15 + v12;
	a3->y = v18;
	v19 = v16 + v13;
	a3->z = v19;
	if(vecSegmentReleased)
	{
		*pVecSegmentReleased = vecRopeSegmentsReleased[0];
	}
	return 1;
}

bool CRope::Adjust(CVector vecStartPos, int a5, RwV3D *pOutVector)
{
	CVector
		&vecLastSegment = vecSegments[segmentCount];
	pThis->vecRopeStartPos = vecRopeStartPos;
	unusedLength = (long double)(31 - segmentCount) * m_length;
	CVector
		vecDistance = vecStartPos - vecLastSegment;
	dist = vecDistance.Length();
	if(dist >= unusedLength)
	{
		if(pOutVector)
			*pOutVector = vecDistance * (unusedLength / dist) + vecLastSegment;
		id = pThis->segmentCount + 1;
		if ( id < 32 )
		{
			for(uint32_t i = segmentCount + 1; i < 32; i++)
			{
				vecDistance.Normalize();
				vecDistance *= m_length;
				vecSegments[id] = vecDistance * (i - segmentCount) + vecLastSegment;
			}
		}
		return 1;
	}
	else
	{
		v14 = 6;
		do
		{
			for ( i = 31; i > pThis->segmentCount + 1; --i )
			{
				v16 = (int)((char *)pThis + 12 * i);
				v18 = vecSegments[i].x - *(float *)(v16 - 12);
				v17 = (RwV3D *)(v16 - 12);
				v19 = *(float *)(v16 + 4) - *(float *)(v16 - 12 + 4);
				v20 = *(float *)(v16 + 8);
				LODWORD(v73) = *(_DWORD *)(v16 - 12);
				v21 = v20 - *(float *)(v16 - 12 + 8);
				v22 = *(_DWORD *)(v16 - 12 + 4);
				LODWORD(v75) = *(_DWORD *)(v16 - 12 + 8);
				LODWORD(v74) = v22;
				if ( sqrt(v18 * v18 + v21 * v21 + v19 * v19) <= pThis->length )
				{
					i = 0;
				}
				else
				{
					v23 = *(float *)v16 - v17->x;
					v24 = *(float *)(v16 + 4) - v17->y;
					v25 = *(float *)(v16 + 8) - v17->z;
					x = v17->x - *(float *)v16;
					_y = v17->y - *(float *)(v16 + 4);
					_z = v17->z - *(float *)(v16 + 8);
					v26 = pThis->length / sqrt(v23 * v23 + v25 * v25 + v24 * v24);
					v85 = _y * v26;
					v86 = _z * v26;
					v70 = x * v26 + *(float *)v16;
					v71 = v85 + *(float *)(v16 + 4);
					v27 = v86 + *(float *)(v16 + 8);
					v17->x = v70;
					v17->y = v71;
					v72 = v27;
					v17->z = v72;
					v28 = v17->x - v73;
					v29 = v17->y - v74;
					v89 = v17->z - v75;
					v30 = 1.0 / dt;
					v31 = v30;
					v76 = v30 * v28;
					v77 = v29 * v31;
					v32 = (RwV3D *)((char *)pThis->vecRopeSegments + 4 * (3 * i + 93));
					v33 = v77;
					v34 = v89 * v31;
					v32->x = v76;
					v32->y = v33;
					v78 = v34;
					v32->z = v78;
				}
			}
			v35 = pThis->segmentCount + 1;
			if ( v35 < 31 )
			{
			v37 = (RwV4D *)((char *)pThis + 12 * v35 - 4);
			v36 = 31 - v35;
			do
			{
			v40 = v37->y - *((float *)v37 - 2);
			v39 = v37->z - *((float *)v37 - 1);
			v62 = v39;
			v38 = v37->w - v37->x;
			v63 = v38;
			if ( sqrt(v40 * v40 + v38 * v38 + v39 * v39) > pThis->length )
			{
			v80 = v39;
			v81 = v38;
			v79 = v40;
			v41 = pThis->length / sqrt(v40 * v40 + v63 * v63 + v62 * v62);
			v87 = v62 * v41;
			v88 = v63 * v41;
			v82 = v79 * v41 + *((float *)v37 - 2);
			v83 = v87 + *((float *)v37 - 1);
			v42 = v88 + v37->x;
			v37->y = v82;
			v37->z = v83;
			v84 = v42;
			v37->w = v84;
			}
			v37 = (RwV4D *)((char *)v37 + 12);
			--v36;
			}
			while ( v36 );
			}
			--v14;
		}
		while ( v14 );
		v43 = 31;
		if ( pThis->segmentCount + 1 < 31 )
		{
		v44 = (int)&pThis->vecRopeSegments[30].z;
		while ( 1 )
		{
		v47 = *(float *)(v44 + 4) - *(float *)(v44 - 8);
		v46 = *(float *)(v44 + 8) - *(float *)(v44 - 4);
		v45 = *(float *)(v44 + 12) - *(float *)v44;
		if ( sqrt(v47 * v47 + v45 * v45 + v46 * v46) <= pThis->length )
		break;
		--v43;
		v44 -= 0xCu;
		v82 = *(float *)(v44 + 4) - *(float *)(v44 + 0x10);
		v83 = *(float *)(v44 + 8) - *(float *)(v44 + 0x14);
		v84 = *(float *)(v44 + 0xC) - *(float *)(v44 + 0x18);
		v49 = v45;
		v48 = v46;
		v50 = pThis->length / sqrt(v47 * v47 + v49 * v49 + v48 * v48);
		v87 = v83 * v50;
		v88 = v84 * v50;
		v79 = v82 * v50 + *(float *)(v44 + 16);
		v80 = v87 + *(float *)(v44 + 20);
		v51 = v88 + *(float *)(v44 + 0x18);
		*(_DWORD *)(v44 + 4) = LODWORD(v79);
		*(_DWORD *)(v44 + 8) = LODWORD(v80);
		v81 = v51;
		*(_DWORD *)(v44 + 12) = LODWORD(v81);
		if ( v43 <= pThis->segmentCount + 1 )
		return 0;
		}
		}
		return 0;
	}
}
