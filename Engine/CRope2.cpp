signed int __cdecl CRope::Create(CEntity *pRopeAttachToEntity, char ropeType, CVector vecStartPos, bool bExpires, char segmentCount, char flags, CEntity *pEntity, int expireTime)
{
  v12 = 0;
  uiID = 0;
  v11 = (CEntity **)&pRopePool[0].m_pRopeAttachToEntity;
    while ( !pRopePool[i].m_ucRopeType || pRopePool[i].m_pRopeAttachToEntity != pRopeAttachToEntity )
    {
        v11 += 202;
        ++uiID;
        if ( (signed int)v11 >= (signed int)&end_ropes_array )
        {
            id = 0;
            v13 = (CRope *)((char *)pRopePool + 805);
            while ( LOBYTE(v13->m_vecRopeSegments[0].x) )
            {
            ++v13;
            ++id;
            if ( (signed int)v13 >= (signed int)&end_ropes_pool )
            return 0;
            }
            ropeArrayPos = id;
            pRopePool[i].m_pRopeAttachToEntity = pRopeAttachToEntity;
            pRope = &pRopePool[i];
            pRopePool[i].m_vecRopeSegments[0] = vecStartPos;
            pRopePool[i].m_vecRopeSegmentsReleased[0] = CVector();
            newFlags = pRopePool[i].m_ucFlags2 & 0xF9 | 4 * (flags & 1) | 1;
            pRopePool[i].m_uiSegmentCount = segmentCount;
            pRopePool[i].m_ucFlags2 = newFlags;
            pRopePool[i].m_pad4 = 0.0f;
            pRopePool[i].m_pRopeAttacherObject = NULL;
            pRopePool[i].m_pAttachedEntity = NULL;
            if ( pEntity && (pEntity->entityInfo & 7) == 2 )// if entity is vehicle
            pRopePool[i].m_uiRopeSegmentLength = 0.9f;
            else
            pRopePool[i].m_uiRopeSegmentLength = 0.5f;
            pRopePool[i].m_ucFlags1 = 0;
            pRopePool[i].m_pRopeHolder = pEntity;
            pRopePool[i].m_ucRopeType = ropeType;
            if(pEntity)
                CEntity__registerReference(pEntity, &pRopePool[i].m_pRopeHolder);
            if(bExpires)
                pRopePool[i].m_uiHoldEntityExpireTime = expireTime + LODWORD(currentTime);
            else
                pRopePool[i].m_uiHoldEntityExpireTime = 0;
            switch(pRopePool[i].m_ucRopeType)
            {
                case 4:
                    pRopePool[i].m_fMass = 50.0f;
                    pRopePool[i].m_fRopeSegmentLength = 1.6129032f;
                break;
                case 5:
                case 6:
                case 7:
                    pRopePool[i].m_fMass = 68.0f;
                    pRopePool[i].m_fRopeSegmentLength = 2.1935484f;
                break;
                case 3:
                    pRopePool[i].m_fMass = 10.0f;
                    pRopePool[i].m_fRopeSegmentLength = 0.32258064f;
                break;
                default:
                    pRopePool[i].m_fMass = 20.0f;
                    pRopePool[i].m_fRopeSegmentLength = 0.6451613f;
                break;
            }
            if(pRopePool[i].m_ucRopeType < 4 || pRopePool[i].m_ucRopeType > 7u)
            {
                size_t seg = 1;
                for(; seg < 32; seg++)
                {
                    if(seg & 1) /* odd */
                    {
                        pRopePool[i].m_vecRopeSegments[seg].x = pRopePool[i].m_fRopeSegmentLength + pRopePool[i].m_vecSegments[seg - 1].x;
                       
                    }
                    else /* even */
                    {
                         pRopePool[i].m_vecRopeSegments[seg].x = pRopePool[i].m_vecSegments[seg - 1].x - pRopePool[i].m_fRopeSegmentLength;
                    }
                    pRopePool[i].m_vecRopeSegments[seg].y = pRopePool[i].m_vecRopeSegments[seg - 1].y;
                    pRopePool[i].m_vecRopeSegments[seg].z = pRopePool[i].m_vecRopeSegments[seg - 1].z;
                    pRopePool[i].m_vecRopeSegmentsReleased[seg - 1].x = CVector();
                }
            }
            else
            {
                size_t seg = 1;
                for(; seg < 32; seg++)
                {
                    pRopePool[i].m_vecRopeSegments[seg].x = pRopePool[i].m_vecRopeSegments[seg - 1].x - pRopePool[i].m_fRopeSegmentLength;
                    pRopePool[i].m_vecRopeSegments[seg].y = pRopePool[i].m_vecRopeSegments[seg - 1].y;
                    pRopePool[i].m_vecRopeSegments[seg].z = pRopePool[i].m_vecRopeSegments[seg - 1].z;
                }
            }
            CRope::CreateRopeAttacherObject(pRope);
            return 1;
        }
    }
    pRopePool[i].m_vecRopeSegments[0] = vecStartPos;
    pRopePool[i].m_vecRopeSegmentsReleased[0] = CVector();
    pRopePool[i].m_ucFlags2 |= 1u;
    pRopePool[i].m_uiSegmentCount = segmentCount;
    for(size_t seg = 0; seg < pRopePool[i].m_uiSegmentCount; seg++)
    {
        pRopePool[i].m_vecRopeSegments[seg] = vecStartPos;
        pRopePool[i].m_vecRopeSegmentsReleased[seg] = CVector();
    }
    CRope::CreateRopeAttacherObject(&pRopePool[i]);
    return 1;
}

CEntity *__thiscall CRope::AttachEntity(CRope *this, CEntity *pEntityToAttach)
{
    CEntity *pAttachedEntity; // eax@1
    CRope *pThis; // ebx@1
    CObject **ppEntity; // esi@1
    XYZ *pPos; // eax@4
    CObject *pRopeAttacherObject; // ecx@4
    long double x; // fst5@4
    long double y; // fst6@4
    long double z; // fst7@4
    CEntity *pEntity; // eax@4
    long double EntityBoundingBoxZ; // fst7@4
    RwV3D *boundingBoxNormal; // eax@4
    RwV3D point; // [sp+0h] [bp-18h]@4
    RwV3D outPoint; // [sp+Ch] [bp-Ch]@4

    pThis = this;
    pAttachedEntity = &this->m_pAttachedEntity->__parent.__parent;
    ppEntity = &this->m_pAttachedEntity;
    if ( pAttachedEntity != pEntityToAttach )
    {
        if ( pAttachedEntity )
            CRope::ReleaseRope(this);
        *ppEntity = (CObject *)pEntityToAttach;
        CEntity__registerReference(pEntityToAttach, (int)ppEntity);
        pEntity = (CEntity *)*ppEntity;
        EntityBoundingBoxZ = modelPtrs[pEntityToAttach->m_nModelIndex]->pColModel->boundingBox.min.z;
        point.z = EntityBoundingBoxZ;
        point = CVector(0.0f, 0.0f, EntityBoundingBoxZ);
        boundingBoxNormal = transformPoint(&outPoint, &pEntity->__parent.xyz->matrix, &point);
        z = boundingBoxNormal->z;
        pRopeAttacherObject = pThis->m_pRopeAttacherObject;
        y = boundingBoxNormal->y;
        x = boundingBoxNormal->x;
        pPos = pRopeAttacherObject->__parent.__parent.__parent.xyz;
        if ( pPos )
        {
        pPos->matrix.matrix.pos.x = x;
        pRopeAttacherObject->__parent.__parent.__parent.xyz->matrix.matrix.pos.y = y;
        pRopeAttacherObject->__parent.__parent.__parent.xyz->matrix.matrix.pos.z = z;
        }
        else
        {
        pRopeAttacherObject->__parent.__parent.__parent.placement.pos.x = x;
        pRopeAttacherObject->__parent.__parent.__parent.placement.pos.y = y;
        pRopeAttacherObject->__parent.__parent.__parent.placement.pos.z = z;
        }
        pThis->m_pRopeAttacherObject->__parent.__parent.BitFlags &= 0xFFFFFFFEu;
        pAttachedEntity = (CEntity *)*ppEntity;
        pAttachedEntity->nImmunities |= 0x2000000u;
        LOBYTE(pAttachedEntity) = ;
        if ( (pEntityToAttach->entityInfo & 7) == 2 )// vehicle
        {
            if ( (pEntityToAttach->entityInfo & 0xF8) == 16 )
            {
                pEntityToAttach->entityInfo = pEntityToAttach->entityInfo & 7 | 24;
            }
        }
        else
        {
            if ( (pEntityToAttach->entityInfo & 7) == 4 )// object
            {
                pAttachedEntity = (CEntity *)(*ppEntity)->__parent.__parent.BitFlags;
                if ( (unsigned __int8)pAttachedEntity & 4 || (unsigned int)pAttachedEntity & 0x40000 )
                {
                    (*(void (__stdcall **)(_DWORD))((*ppEntity)->__parent.__parent.__parent.__vmt + 16))(0);
                    pAttachedEntity = (CEntity *)CPhysical::AddToMovingList(&(*ppEntity)->__parent);
                    (*ppEntity)->__parent.colFlags1 = 0;
                }
            }
        }
    }
    return pAttachedEntity;
}

void __fastcall CRope::Process(CRope *this)
{
  CRope *pThis; // esi@1
  RwV3D *cameraPos; // eax@2
  long double y; // fst7@4
  long double x; // fst6@4
  char flags; // al@5
  long double release_z_pos; // fst7@7
  long double release_y_pos; // fst6@7
  CEntity *pAttachedEntity; // eax@12
  int attachedEntityID; // eax@14
  CObject *v10; // eax@18
  XYZ *v11; // ecx@18
  RwV3D *pRopePlacement; // eax@19
  int v13; // eax@22
  int v14; // ebx@23
  int releasedRopePos.y; // esi@23
  CRope *pThisPointer; // edx@24
  int __x; // ebp@24
  RwV3D *releasedRopePos; // edi@24
  float __y; // ecx@24
  float __z; // edx@24
  char v21; // al@24
  char v22; // al@24
  long double releasedRopePos.z_scaled; // fst7@24
  float v24; // ST24_4@24
  long double velocity_final_z; // fst6@24
  long double releasedRope.z; // fst7@24
  long double releasedRope.y; // fst6@24
  long double v28; // fst7@25
  long double v29; // fst7@28
  float v30; // ST24_4@28
  long double _y; // fst6@28
  long double _x; // fst4@28
  float v33; // ST24_4@28
  long double v34; // t0@28
  long double v35; // fst6@28
  long double v36; // fst5@28
  long double v37; // fst7@28
  long double v38; // fst6@28
  long double v39; // fst7@28
  long double v40; // fst6@28
  float v41; // ST24_4@28
  long double v42; // fst7@28
  long double v43; // fst6@28
  char modelIndex_; // al@29
  CRope *v45; // edx@29
  char v46; // zf@31
  char ropeType; // al@52
  int bDoRaiseWrench; // edx@52
  char v49; // al@61
  int bDoLowerWrench; // edx@61
  unsigned __int8 v51; // zf@69
  char v52; // sf@69
  char v53; // of@69
  __int16 v54; // ax@69
  CPad *v55; // eax@69
  CRope *v56; // ecx@74
  long double v57; // fst7@76
  long double v58; // fst7@80
  CObject **ppAttachedEntity; // ebp@84
  CPhysical *v60; // esi@84
  long double fMass; // fst7@85
  long double v62; // fst7@87
  XYZ *v63; // eax@92
  RwV3D *RopeStartVector; // eax@93
  CRope *pRope; // ebx@95
  XYZ *v66; // eax@96
  float v67; // ecx@98
  float v68; // edx@98
  XYZ *v69; // eax@99
  CPhysical *v70; // ecx@99
  char *v71; // edi@99
  float v72; // edx@99
  float v73; // eax@99
  RwV3D *v74; // eax@100
  char v75; // al@102
  long double v76; // fst7@102
  long double v77; // fst6@102
  _BYTE v78[12]; // ST08_12@102
  XYZ *v79; // eax@107
  RwV3D *v80; // eax@108
  long double v81; // fst7@110
  long double v82; // fst7@110
  long double v83; // fst6@110
  long double dist_squared; // fst6@110
  long double v85; // fst7@112
  long double v86; // fst6@112
  long double v87; // fst6@112
  long double v88; // fst5@112
  long double v89; // fst6@112
  RwV3D *v90; // eax@112
  long double v91; // fst7@112
  long double v92; // fst6@112
  int v93; // ecx@113
  XYZ *v94; // eax@116
  CObject *v95; // ecx@116
  long double v96; // fst5@116
  long double v97; // fst6@116
  long double v98; // fst7@116
  RwV3D *v99; // edx@116
  RwV3D *pRopeObjectVelocity; // eax@116
  CObject *v101; // eax@116
  long double AttachedEntityBBox_Z; // fst7@116
  RwV3D *v103; // eax@116
  CRope *pRopeObject; // esi@119
  float v105; // edx@119
  float v106; // ecx@119
  char v107; // al@120
  CPad *v108; // eax@132
  CObject *v109; // eax@134
  int v110; // ecx@135
  CObject *v111; // eax@136
  CObject *v112; // eax@137
  char v113; // al@142
  int v114; // eax@144
  CPool *v115; // ebx@149
  int v116; // edi@149
  int v117; // ebp@150
  char v118; // cl@151
  CVehicle__v1 *v119; // esi@152
  int v120; // eax@153
  __int16 v121; // ax@157
  CMatrix *v122; // ST0C_4@164
  RwV3D *v123; // eax@164
  float v124; // ecx@164
  float v125; // edx@164
  float v126; // eax@164
  CObject **v127; // edi@167
  int v128; // ST10_4@167
  XYZ *v129; // eax@169
  CRope *v130; // edx@169
  CObject *v131; // ecx@169
  long double v132; // fst5@169
  long double v133; // fst6@169
  long double v134; // fst7@169
  CObject *v135; // edx@169
  RwV3D *v136; // eax@169
  long double v137; // fst7@172
  long double v138; // fst7@173
  long double v139; // fst6@173
  long double invMass; // fst7@173
  __int16 v141; // fps@173
  unsigned __int8 v142; // c0@173
  unsigned __int8 v143; // c3@173
  CPool *pPedPool; // ebp@176
  int v145; // edi@176
  int v146; // ebx@177
  char v147; // al@178
  CPed *v148; // esi@179
  int v149; // eax@182
  XYZ *v150; // eax@183
  RwV3D *v151; // eax@184
  float x2; // ecx@186
  float y2; // edx@186
  float z2; // eax@186
  CObject **v155; // edi@189
  CRope *v156; // esi@189
  long double v157; // fst7@189
  int v158; // ST10_4@189
  CObject *v159; // ecx@189
  long double v160; // fst7@189
  RwV3D *v161; // eax@189
  float v162; // ST0C_4@190
  int pObjectPool; // edx@199
  int v164; // ebx@199
  int v165; // ecx@200
  CPhysical *v166; // esi@202
  int v167; // eax@205
  int v168; // eax@226
  signed int v169; // edi@226
  long double fAttachedEntityColBoxZ; // fst7@226
  CMatrix *v171; // eax@226
  RwV3D *v172; // eax@226
  float v173; // ecx@226
  float v174; // edx@226
  float v175; // eax@226
  long double v176; // fst7@230
  CMatrix *v177; // eax@230
  RwV3D *v178; // ecx@232
  CMatrix *v179; // eax@232
  float v180; // edx@233
  float v181; // eax@233
  RwV3D *v182; // ecx@234
  long double v183; // fst7@234
  CMatrix *v184; // ST0C_4@234
  float v185; // edx@235
  float v186; // eax@235
  float v187; // ecx@235
  int v188; // eax@240
  CRope *v189; // esi@240
  CObject *v190; // edx@240
  RwV3D *v191; // eax@240
  long double v192; // fst7@243
  long double v193; // fst7@244
  long double v194; // fst6@244
  long double invMass_; // fst7@244
  __int16 v196; // fps@244
  unsigned __int8 v197; // c0@244
  unsigned __int8 v198; // c3@244
  XYZ *v199; // eax@248
  XYZ *v200; // edx@248
  float v201; // ecx@248
  float v202; // edx@248
  XYZ *v203; // eax@248
  float v204; // edx@248
  long double v205; // fst7@248
  float v206; // ecx@248
  CRope *v207; // esi@249
  float startPosZ_normalized; // [sp+18h] [bp-13Ch]@24
  float entityWeightFactor?a; // [sp+18h] [bp-13Ch]@52
  float LowerLevelY; // [sp+18h] [bp-13Ch]@61
  float entityWeightFactor?; // [sp+18h] [bp-13Ch]@88
  RwV3D point; // [sp+1Ch] [bp-138h]@28
  CRope *pThisPtr; // [sp+28h] [bp-12Ch]@1
  RwV3D a6; // [sp+2Ch] [bp-128h]@24
  float _y2; // [sp+30h] [bp-124h]@24
  float _z2; // [sp+34h] [bp-120h]@24
  float v217; // [sp+38h] [bp-11Ch]@28
  float velocity_final_y; // [sp+3Ch] [bp-118h]@24
  char v219; // [sp+42h] [bp-112h]@144
  char v220; // [sp+43h] [bp-111h]@144
  char v221; // [sp+44h] [bp-110h]@144
  char v222; // [sp+45h] [bp-10Fh]@144
  char v223; // [sp+46h] [bp-10Eh]@144
  char v224; // [sp+47h] [bp-10Dh]@144
  float x1; // [sp+48h] [bp-10Ch]@28
  float y1; // [sp+4Ch] [bp-108h]@28
  float z1; // [sp+50h] [bp-104h]@28
  char v228; // [sp+56h] [bp-FEh]@144
  char v229; // [sp+57h] [bp-FDh]@144
  float velocity_z; // [sp+58h] [bp-FCh]@23
  RwV3D releasedRope.x; // [sp+5Ch] [bp-F8h]@24
  float _releasedRope.y; // [sp+60h] [bp-F4h]@24
  float _releasedRope.z; // [sp+64h] [bp-F0h]@24
  float scaleFactor; // [sp+68h] [bp-ECh]@1
  float v235; // [sp+6Ch] [bp-E8h]@112
  RwV3D outPoint; // [sp+70h] [bp-E4h]@28
  RwV3D velocity_final_x; // [sp+7Ch] [bp-D8h]@24
  int v238; // [sp+80h] [bp-D4h]@234
  float v239; // [sp+84h] [bp-D0h]@234
  RwV3D v240; // [sp+88h] [bp-CCh]@28
  int v241; // [sp+8Ch] [bp-C8h]@230
  float v242; // [sp+90h] [bp-C4h]@230
  RwV3D releasedRopePos.x_scaled; // [sp+94h] [bp-C0h]@24
  int v244; // [sp+98h] [bp-BCh]@232
  int v245; // [sp+9Ch] [bp-B8h]@232
  RwV3D v246; // [sp+A0h] [bp-B4h]@234
  float v247; // [sp+ACh] [bp-A8h]@113
  float v248; // [sp+B0h] [bp-A4h]@113
  float v249; // [sp+B4h] [bp-A0h]@113
  float v250; // [sp+BCh] [bp-98h]@113
  float v251; // [sp+C0h] [bp-94h]@113
  float v252; // [sp+C4h] [bp-90h]@113
  float v253; // [sp+CCh] [bp-88h]@113
  float v254; // [sp+D0h] [bp-84h]@113
  float v255; // [sp+D4h] [bp-80h]@113
  CMatrix thisa; // [sp+F4h] [bp-60h]@113
  RwV3D v257; // [sp+13Ch] [bp-18h]@232
  EHRegistrationNode __$EHRec$; // [sp+148h] [bp-Ch]@113

    scaleFactor = pow(0.800000011920929, dt);
    if ( camera.__parent.xyz )
        cameraPos = &camera.__parent.xyz->matrix.matrix.pos;
    else
        cameraPos = &camera.__parent.placement.pos;
    y = cameraPos->y - this->m_vecRopeSegments[0].y;
    x = cameraPos->x - this->m_vecRopeSegments[0].x;
    if ( sqrt(x * x + y * y) >= 200.0 )           // rope visible within 200 units
        return;
    flags = this->m_ucFlags2;
    
    if ( !(flags & 1) && LODWORD(currentTime) > this->m_uiHoldEntityExpireTime )
    {
        this->m_vecRopeSegmentsReleased[0].z -= dt * 0.0015;
        this->m_vecRopeSegments[0] += dt * this->m_vecRopeSegmentsReleased[0];
    }
    if ( flags & 4 && (g_GlobalRenderFlags & 7) == 2 )
        this->m_pad4 = CWorld::findGroundZFor3DCoord(this->m_vecRopeSegments[0], 0, 0);
    if ( pThis->m_ucFlags2 & 4 )                  // Flag (4) : Is Any Entity Attached
    {
        pAttachedEntity = &pThis->m_pAttachedEntity->__parent.__parent;
        if ( pAttachedEntity )
        {
            if ( (pAttachedEntity->entityInfo & 7) == 2
            || (attachedEntityID = pAttachedEntity->m_nModelIndex, attachedEntityID == (unsigned __int16)miKmbContainerRed)
            || attachedEntityID == (unsigned __int16)miKmbContainerYel
            || attachedEntityID == (unsigned __int16)miKmbContainerBlue
            || attachedEntityID == (unsigned __int16)miKmbContainerOpen )// if either vehicle or container
            {
                v11 = pThis->m_pRopeAttacherObject->__parent.__parent.__parent.xyz;
                if ( v11 )
                    pRopePlacement = &v11->matrix.matrix.pos;
                else
                    pRopePlacement = &pThis->m_pRopeAttacherObject->__parent.__parent.__parent.placement.pos;
                pThis->m_pad4 = pRopePlacement->z - 0.5;
            }
        }
    }
    if ( pThis->m_uiSegmentCount < 32 - 1 )
    {
        for(size_t seg =  pThis->m_uiSegmentCount + 1; seg < 32; seg++)
        {
        releasedRopePos.y = (int)&pThisPtr->m_vecRopeSegmentsReleased[v13].y;
        do
        {
            CVector
                vecSavedSegment = m_vecRopeSegments[seg].x;
            m_vecRopeSegmentsReleased[seg].x += (long double)((rand() & 15) - 8) * 0.001;
            m_vecRopeSegmentsReleased[seg].y += (long double)((rand() & 15) - 8) * 0.001;
            m_vecRopeSegmentsReleased[seg] = ((1.0 - scaleFactor) * m_vecRopeSegmentsReleased[seg - 1]) + (scaleFactor * m_vecRopeSegmentsReleased[seg]);
            m_vecRopeSegments[seg].z -= dt * 0.15000001;
            
            if(pThisPointer->m_ucFlags2 & 4)
            {
                float z = pThisPointer->m_pad4 + 0.30000001;
                if(m_vecRopeSegments[seg].z <= z)
                {
                    m_vecRopeSegments[seg].z = z;
                }
            }
            CVector
                vecDiff = m_vecRopeSegments[seg] - m_vecRopeSegments[seg - 1];
            m_vecRopeSegments[seg] = vecDiff * (m_fRopeSegmentLength / vecDiff.Length()) + m_vecRopeSegments[seg - 1];
            
            m_vecRopeSegmentsReleased[seg] = (1.0 / dt) * (m_vecRopeSegments[seg] - vecSavedSegment);
        }
    }
    if ( m_ucRopeType != 1 && m_ucRopeType != 2 )
    {
    v46 = m_ucRopeType == 4;
    if ( m_ucRopeType == 4 )
    goto LABEL_37;
    if ( m_ucRopeType != 5 && m_ucRopeType != 7 && m_ucRopeType != 6 && m_ucRopeType != 3 )
    goto LABEL_249;
    }
    v46 = m_ucRopeType == 4;
    LABEL_37:
    if ( (!v46 || controlledCraneNumber != 1)
    && (m_ucRopeType != 5 || controlledCraneNumber != 2)
    && (m_ucRopeType != 7 || controlledCraneNumber != 3)
    && (m_ucRopeType != 6 || controlledCraneNumber != 4) )
    {
    if ( m_ucRopeType == 1 )
    goto LABEL_69;
    if ( m_ucRopeType != 3 && m_ucRopeType != 2 )
    goto LABEL_80;
    }
    if ( m_ucRopeType == 1 || m_ucRopeType == 2 || m_ucRopeType == 3 )
    {
LABEL_69:
    v54 = CPad::GetSteeringLeftRight(CPad::GetPad(0));
    v53 = 0;
    v51 = v54 == 0;
    v52 = v54 < 0;
    if ( v54 < 0 )
    {
      if ( bCraneControls_Enabled_Up )
        goto LABEL_ProcessRaiseWrench;
      v53 = 0;
      v51 = v54 == 0;
      v52 = v54 < 0;
    }
    if ( (unsigned __int8)(v52 ^ v53) | v51 || !bCraneControls_Enabled_Down )
    {
      goto LABEL_76;
    }
LABEL_ProcessRaiseWrench:
    LODWORD(v217) = v54;
    pThisPtr->m_uiRopeSegmentLength -= (long double)v54 * dt * 0.0000099999997;
LABEL_76:
    v57 = v56->m_uiRopeSegmentLength;
    if ( v57 > 0.83999997 )
      v57 = 0.83999997;
    v56->m_uiRopeSegmentLength = v57;
    goto LABEL_79;
  }
    if(bCraneControls_Enabled_Up)
    {
        float moveUp = (long double)CPad::GetPad(0)->NewState.ButtonSquare * dt * 0.0000099999997;
        if((m_ucRopeType == 4 || m_ucRopeType == 5 || m_ucRopeType == 7 || m_ucRopeType == 6) && moveUp > 0.0 && moveUp + pThisPtr->m_uiRopeSegmentLength < 0.89999998)
        {
            CAudioManager::ProcessCrane((CAudioManager *)&audio, 104, pThisPtr->m_pRopeHolder, 0, 1.0);
        }
        m_uiRopeSegmentLength += moveUp;
    }
    if(bCraneControls_Enabled_Down)
    {
        float moveDown = (long double)CPad::GetPad(0)->NewState.ButtonCross * dt * 0.0000099999997;
        if((m_ucRopeType == 4 || m_ucRopeType == 5 || m_ucRopeType == 7 || m_ucRopeType == 6) && moveDown > 0.0 && m_uiRopeSegmentLength - moveDown > 0.0099999998)
        {
            CAudioManager::ProcessCrane((CAudioManager *)&audio, 104, pThisPtr->m_pRopeHolder, 0, 1.0);
        }
        pThisPtr->m_uiRopeSegmentLength -= moveDown;
        LABEL_79:
    }
LABEL_80:
  v58 = v45->m_uiRopeSegmentLength;
  if ( v58 < 0.0099999998 )
    v58 = 0.0099999998;
  v45->m_uiRopeSegmentLength = v58;
  if ( v58 > 0.89999998 )
    v58 = 0.89999998;
  v60 = &v45->m_pAttachedEntity->__parent;
  v45->m_uiRopeSegmentLength = v58;
  ppAttachedEntity = &v45->m_pAttachedEntity;
  LODWORD(scaleFactor) = (char *)v45 + 792;
  if ( v60 )
  {
    fMass = v60->fMass;
    if ( v60->__parent.m_nModelIndex == 428 )
      fMass = 750.0;                            // if it's Securicar(vehicle)
    v62 = fMass * flt_8CD89C * 0.00083333335 + flt_8CD898;
    if ( v62 <= 0.5 )
    {
      entityWeightFactor? = v62;
      v60->colFlags1 = 0;
    }
    else
    {
      entityWeightFactor? = 0.5;
      v60->colFlags1 = 0;
    }
  }
  else
  {
    v60 = &v45->m_pRopeAttacherObject->__parent;
    entityWeightFactor? = flt_8CD898;
    v60->__parent.BitFlags |= 1u;
  }
    if ( v60 )
    {
        v63 = v60->__parent.__parent.xyz;
        if ( v63 )
        RopeStartVector = &v63->matrix.matrix.pos;
        else
        RopeStartVector = &v60->__parent.__parent.placement.pos;
        pRope = pThisPtr;
        if(CRope::Adjust(pThisPtr, RopeStartVector->x, RopeStartVector->y, RopeStartVector->z, 0.1f, &a6))
        {
            v66 = v60->__parent.__parent.xyz;
            if ( v66 )
            {
            LODWORD(v66->matrix.matrix.pos.x) = (_DWORD)a6.x;
            v60->__parent.__parent.xyz->matrix.matrix.pos.y = _y2;
            v60->__parent.__parent.xyz->matrix.matrix.pos.z = _z2;
            }
            else
            {
            v67 = _y2;
            v68 = _z2;
            LODWORD(v60->__parent.__parent.placement.pos.x) = (_DWORD)a6.x;
            v60->__parent.__parent.placement.pos.y = v67;
            v60->__parent.__parent.placement.pos.z = v68;
            }
            v70 = (CPhysical *)pRope->m_pRopeHolder;
            v69 = v70->__parent.__parent.xyz;
            if ( v69 )
                v74 = &v69->matrix.matrix.pos;
            else
                v74 = &v70->__parent.__parent.placement.pos;
            point.x = pRope->m_vecRopeSegments[0].x - v74->x;
            *(_DWORD *)v78 = (_DWORD)point.x;
            point.y = pRope->m_vecRopeSegments[0].y - v74->y;
            *(_DWORD *)&v78[4] = LODWORD(point.y);
            point.z = pRope->m_vecRopeSegments[0].z - v74->z;
            *(_DWORD *)&v78[8] = LODWORD(point.z);
            CPhysical::GetSpeed(v70, &releasedRope.x, *(RwV3D *)v78);
            v75 = pRope->m_ucRopeType;
            if ( v75 == 4 || v75 == 5 || v75 == 6 || v75 == 7 )
            {
                point = CVector();
                LODWORD(releasedRope.x.x) = 0;
                _releasedRope.y = 0.0;
                _releasedRope.z = 0.0;
            }
            v79 = v60->__parent.__parent.xyz;
            CVector
                vecVelocityDiff = v60 - 
            a6.x = v60->vecVelocity.x - releasedRope.x.x;
            _y2 = v60->vecVelocity.y - _releasedRope.y;
            _z2 = v60->vecVelocity.z - _releasedRope.z;
            if ( v79 )
                v80 = &v79->matrix.matrix.pos;
            else
                v80 = &v60->__parent.__parent.placement.pos;
            CVector
                vecDiff = v80 - pRope->m_vecRopeSegments[0];
            vecDiff.Normalize();
            v81 = a6.x;
            dist_squared = vecDiff.z * _z2 + vecDiff.x * a6.x + vecDiff.y * _y2;
            if(dist_squared > 0.0)
            {
                v81 -= dist_squared * vecDiff.x;
                _y2 -= dist_squared * vecDiff.y;
                _z2 -= dist_squared * vecDiff.z;
            }
            v60->vecVelocity.x += (v81 + releasedRope.x.x - v60->vecVelocity.x) * (1.0 - entityWeightFactor?);
            v60->vecVelocity.y += (_releasedRope.y + _y2 - v60->vecVelocity.y) * (1.0 - entityWeightFactor?);
            v60->vecVelocity.z += (_releasedRope.z + _z2 - v60->vecVelocity.z) * (1.0 - entityWeightFactor?);
            m_pRopeHolder->vecVelocity.x -= (v81 + releasedRope.x.x - v60->vecVelocity.x) * entityWeightFactor?;
            m_pRopeHolder->vecVelocity.x -= (_releasedRope.y + _y2 - v60->vecVelocity.y) * entityWeightFactor?;
            m_pRopeHolder->vecVelocity.x -= (_releasedRope.z + _z2 - v60->vecVelocity.z) * entityWeightFactor?;
            if(*ppAttachedEntity)
            {
                CMatrix::Unknown14(&thisa, v60->__parent.__parent.xyz);
                v93 = (int)v60->__parent.__parent.xyz;
                point = -vecDiff;
                CMatrix::Unknown9(v93, point.x, point.y, point.z);
                CMatrix::Unknown14((CMatrix *)&v247, v60->__parent.__parent.xyz);
                v60->__parent.__parent.xyz->matrix.matrix.right.x = thisa.matrix.right.x * 0.89999998 + v247 * 0.1;
                v60->__parent.__parent.xyz->matrix.matrix.right.y = thisa.matrix.right.y * 0.89999998 + v248 * 0.1;
                v60->__parent.__parent.xyz->matrix.matrix.right.z = thisa.matrix.right.z * 0.89999998 + v249 * 0.1;
                v60->__parent.__parent.xyz->matrix.matrix.top.x = thisa.matrix.top.x * 0.89999998 + v250 * 0.1;
                v60->__parent.__parent.xyz->matrix.matrix.top.y = thisa.matrix.top.y * 0.89999998 + v251 * 0.1;
                v60->__parent.__parent.xyz->matrix.matrix.top.z = thisa.matrix.top.z * 0.89999998 + v252 * 0.1;
                v60->__parent.__parent.xyz->matrix.matrix.at.x = thisa.matrix.at.x * 0.89999998 + v253 * 0.1;
                v60->__parent.__parent.xyz->matrix.matrix.at.y = thisa.matrix.at.y * 0.89999998 + v254 * 0.1;
                v60->__parent.__parent.xyz->matrix.matrix.at.z = thisa.matrix.at.z * 0.89999998 + v255 * 0.1;
                CMatrix::destructor((CMatrix *)&v247);
                CMatrix::destructor(&thisa);
            }
            else
            {
                point.x = -x1;
                point.y = -y1;
                point.z = -z1;
                CMatrix::Unknown9((int)v60->__parent.__parent.xyz, point.x, point.y, point.z);
            }
        }
        if ( *ppAttachedEntity )
        {
            CMatrix::Copy2(&pRope->m_pRopeAttacherObject->__parent.__parent.__parent.xyz->matrix, (*ppAttachedEntity)->__parent.__parent.__parent.xyz);
            m_pRopeAttacherObject->vecVelocity = m_pAttachedEntity->vecVelocity;
            AttachedEntityBBox_Z = modelPtrs[m_pAttachedEntity->__parent.__parent.m_nModelIndex]->pColModel->boundingBox.min.z;
            CVector point(0.0f, 0.0f, AttachedEntityBBox_Z);
            v103 = transformPoint(&outPoint, m_pAttachedEntity->__parent.__parent.__parent.xyz->matrix, &point);
            if ( m_pRopeAttacherObject->__parent.__parent.__parent.xyz )
            {
                m_pRopeAttacherObject->__parent.__parent.__parent.xyz->matrix.matrix.pos.x = v103->x;
                m_pRopeAttacherObject->__parent.__parent.__parent.xyz->matrix.matrix.pos.y = v103->y;
                m_pRopeAttacherObject->__parent.__parent.__parent.xyz->matrix.matrix.pos.z = v103->z;
            }
            else
            {
                m_pRopeAttacherObject->__parent.__parent.__parent.placement.pos.x = v103->x;
                m_pRopeAttacherObject->__parent.__parent.__parent.placement.pos.y = v103->y;
                m_pRopeAttacherObject->__parent.__parent.__parent.placement.pos.z = v103->z;
            }
        }
    }
    v105 = pThisPtr->m_vecRopeSegments[31].x;
    v106 = pThisPtr->m_vecRopeSegments[31].z;
    y1 = pThisPtr->m_vecRopeSegments[31].y;
    x1 = v105;
    z1 = v106;
    if(m_pAttachedEntity)
    {
        if ( m_ucRopeType == 4 && controlledCraneNumber == 1
        || m_ucRopeType == 5 && controlledCraneNumber == 2
        || m_ucRopeType == 7 && controlledCraneNumber == 3
        || m_ucRopeType == 6 && controlledCraneNumber == 4
        || m_ucRopeType == 1
        || m_ucRopeType == 3
        || m_ucRopeType == 2 )
        {
            if ( bCraneControls_Enabled_Release )
            {
                if ( CPad::TestFireButton(CPad::GetPad(0)) )
                    CRope::ReleaseRope(this);
            }
        }
        if(m_pAttachedEntity->__parent.nImmunities & bDestroyed)
        {
            m_pAttachedEntity->__parent.nImmunities &= 0xFDFFFFFF;
            m_pAttachedEntity->__parent.nImmunities &= 0x7FFFFFFFu;
            m_pAttachedEntity = NULL;
            m_pRopeAttacherObject->__parent.__parent.BitFlags |= 1u;
            m_ucFlags1 = 60;
        }
        if(m_pAttachedEntity && (m_pAttachedEntity->__parent.__parent.entityInfo & 7) == 2 && (CVehicle*)v112->field_594 == 9 && (CVehicle*)v112->pDriver)
        {
            CRope::ReleaseRope(pRopeObject);
        }
        goto LABEL_249;
    }
  v113 = pThisPtr->m_ucFlags1;
  if ( v113 )
  {
    pThisPtr->m_ucFlags1 = v113 - 1;
    goto LABEL_249;
  }
  v114 = pThisPtr->m_ucRopeType - 1;
  v229 = 0;
  v222 = 0;
  v228 = 0;
  v223 = 0;
  v220 = 0;
  v219 = 0;
  v221 = 0;
  v224 = 0;
  switch ( v114 )
  {
    case 3:
      v222 = 1;
      goto LABEL_149;
    case 6:
      v228 = 1;
      goto LABEL_198;
    case 5:
      v223 = 1;
      goto LABEL_149;
    case 2:
      v220 = 1;
      v219 = 1;
      v221 = 1;
      v224 = 1;
      v229 = 1;
      goto LABEL_149;
    case 0:
LABEL_149:
      v115 = VehiclePool;
      v116 = VehiclePool->size;
      if ( !v116 )
        goto LABEL_192;
      v117 = 0x286 * v116;
      break;
    case 1:
      pPedPool = PedPool;
      v145 = PedPool->size;
      if ( !v145 )
        goto LABEL_249;
      v146 = 497 * v145;
      while ( 1 )
      {
        v147 = *(_BYTE *)(v145-- + pPedPool->flags - 1);
        v146 -= 0x1F1u;
        if ( v147 >= 0 )
        {
          v148 = (CPed *)&pPedPool->objects[v146];
          if ( v148 )
          {
            if ( v148->field_530 != 0x37 )
            {
              if ( !CPed__isPlayer((CPed *)&pPedPool->objects[v146]) )
              {
                v149 = *(_DWORD *)&v148->pedFlags[0];
                if ( !(BYTE1(v149) & 1) )
                {
                  v150 = v148->__parent.__parent.__parent.xyz;
                  v151 = v150 ? &v150->matrix.matrix.pos : &v148->__parent.__parent.__parent.placement;
                  x2 = v151->x;
                  y2 = v151->y;
                  z2 = v151->z;
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
        if ( !v145 )
          goto LABEL_249;
      }
      v155 = (CObject **)LODWORD(scaleFactor);
      v158 = LODWORD(scaleFactor);
      *LODWORD(scaleFactor) = v148;
      CEntity__registerReference(&v148->__parent.__parent, v158);
      (*v155)->__parent.nImmunities |= 0x80000000u;
      v159 = *v155;
      v160 = modelPtrs[v148->__parent.__parent.m_nModelIndex]->pColModel->boundingBox.min.z;
      CVector point(0.0f, 0.0f, v160);
      v161 = transformPoint(&outPoint, &v159->__parent.__parent.__parent.xyz->matrix, &point);
      CPhysical::SetPosition(&pThisPtr->m_pRopeAttacherObject->__parent, v161);
      v157 = z1 - _z2;
      m_pRopeAttacherObject->__parent.__parent.BitFlags &= 0xFFFFFFFEu;
      if ( v157 > 0.0 )
      {
        v162 = m_uiRopeSegmentLength - v157 / m_fMass - m_fRopeSegmentLength / m_fMass;
        m_uiRopeSegmentLength = min(v162, 0.0099999998);
      }
      goto LABEL_249;
    default:
      goto LABEL_249;
  }
    while ( 1 )
    {
        v118 = *(_BYTE *)(v116-- + v115->flags - 1);
        v117 -= 646;
        if ( v118 >= 0 )
        {
            v119 = (CVehicle__v1 *)&v115->objects[v117];
            if ( v119 )
            {
                if ( (v120 = v119->field_594) == 0
                || v120 == 9 && !v119->pDriver
                || v120 == 1
                || (v121 = v119->__parent.__parent.m_nModelIndex, v121 == 473)
                || v121 == 539 )
                {
                    if ( !(v119->__parent.nImmunities & 0x20000000)
                    && BYTE1(v119->VehicleFlags[1]) & 0x10
                    && (!v229 || v119->__parent.__parent.m_nModelIndex == 564) )
                    {
                        if ( !(v119->field_4A8 & 0x60) )
                        {
                            v122 = (CMatrix *)v119->__parent.__parent.__parent.xyz;
                            point.z = modelPtrs[v119->__parent.__parent.m_nModelIndex]->pColModel->boundingBox.min.z;
                            LODWORD(point.x) = 0;
                            LODWORD(point.y) = 0;
                            v123 = transformPoint(&outPoint, v122, &point);
                            v124 = v123->x;
                            v125 = v123->y;
                            v126 = v123->z;
                            a6.x = v124;
                            _y2 = v125;
                            _z2 = v126;
                            if ( sqrt((v124 - x1) * (v124 - x1) + (v126 - z1) * (v126 - z1) + (v125 - y1) * (v125 - y1)) < 2.5 )
                                break;
                        }
                    }
                }
            }
        }
        if ( !v116 )
            goto LABEL_191;
    }
  v127 = (CObject **)LODWORD(scaleFactor);
  v128 = LODWORD(scaleFactor);
  *LODWORD(scaleFactor) = v119;
  CEntity__registerReference(&v119->__parent.__parent, v128);
  (*v127)->__parent.nImmunities |= 0x80000000u;
  if ( (v119->__parent.__parent.entityInfo & 0xF8) == 16 )
    v119->__parent.__parent.entityInfo = v119->__parent.__parent.entityInfo & 7 | 0x18;
  v135 = *v127;
  point.z = modelPtrs[v119->__parent.__parent.m_nModelIndex]->pColModel->boundingBox.min.z;
  LODWORD(point.x) = 0;
  LODWORD(point.y) = 0;
  v136 = transformPoint(&outPoint, &v135->__parent.__parent.__parent.xyz->matrix, &point);
  v134 = v136->z;
  v130 = pThisPtr;
  v133 = v136->y;
  v131 = pThisPtr->m_pRopeAttacherObject;
  v132 = v136->x;
  v129 = v131->__parent.__parent.__parent.xyz;
  if ( v129 )
  {
    v129->matrix.matrix.pos.x = v132;
    v131->__parent.__parent.__parent.xyz->matrix.matrix.pos.y = v133;
    v131->__parent.__parent.__parent.xyz->matrix.matrix.pos.z = v134;
  }
  else
  {
    v131->__parent.__parent.__parent.placement.pos.x = v132;
    v131->__parent.__parent.__parent.placement.pos.y = v133;
    v131->__parent.__parent.__parent.placement.pos.z = v134;
  }
  v137 = z1 - _z2;
  v130->m_pRopeAttacherObject->__parent.__parent.BitFlags &= 0xFFFFFFFEu;
  if ( v137 > 0.0 )
  {
    v139 = v137;
    invMass = 1.0 / v130->m_fMass;
    v138 = v130->m_uiRopeSegmentLength - v139 * invMass - invMass * v130->m_fRopeSegmentLength;
    UNDEF(v141);
    v130->m_uiRopeSegmentLength = v138;
    if ( v142 | v143 )
      v138 = 0.0099999998;
    v130->m_uiRopeSegmentLength = v138;
  }
LABEL_191:
  ppAttachedEntity = (CObject **)LODWORD(scaleFactor);
LABEL_192:
  if ( v222 || v223 || v220 || v219 || v221 || v224 )
  {
LABEL_198:
    if ( *ppAttachedEntity
      || (pObjectPool = (int)ObjectPool, v164 = ObjectPool->size, LODWORD(v217) = ObjectPool, !v164) )
      goto LABEL_249;
    v165 = 0x19C * v164;
    do
    {
      --v164;
      v165 -= 412;
      v46 = (*(_BYTE *)(v164 + *(_DWORD *)(pObjectPool + 4)) & 0x80) == 0;
      LODWORD(v235) = v165;
      if ( v46 )
      {
        v166 = (CPhysical *)(v165 + *(_DWORD *)pObjectPool);
        if ( v166 )
        {
          if ( LODWORD(v166[1].__parent.__parent.placement.pos.y) & 0x40000 )
          {
            if ( v222
              && ((v167 = v166->__parent.m_nModelIndex, v167 == (unsigned __int16)miKmbContainerRed)
               || v167 == (unsigned __int16)miKmbContainerYel
               || v167 == (unsigned __int16)miKmbContainerBlue
               || v167 == (unsigned __int16)miBarrel4
               || v167 == (unsigned __int16)miKmbContainerOpen)
              || v228 && (v167 = v166->__parent.m_nModelIndex, v167 == (unsigned __int16)miBlockpallet)
              || v223
              && ((v167 = v166->__parent.m_nModelIndex, v167 == (unsigned __int16)miDynQuarryrock01)
               || v167 == (unsigned __int16)miDynQuarryrock02
               || v167 == (unsigned __int16)miDynQuarryrock03
               || v167 == (unsigned __int16)miDeadTiedCop)
              && !v166->pAttachedEntity
              || v220 && (v167 = v166->__parent.m_nModelIndex, v167 == (unsigned __int16)miWongDish)
              || v219 && (v167 = v166->__parent.m_nModelIndex, v167 == (unsigned __int16)word_8CD734)
              || v221 && (v167 = v166->__parent.m_nModelIndex, v167 == (unsigned __int16)word_8CD738)
              || v224 && (v167 = v166->__parent.m_nModelIndex, v167 == (unsigned __int16)word_8CD73C) )
            {
              fAttachedEntityColBoxZ = modelPtrs[v167]->pColModel->boundingBox.min.z;
              v171 = (CMatrix *)v166->__parent.__parent.xyz;
              point.z = fAttachedEntityColBoxZ;
              LODWORD(point.x) = 0;
              LODWORD(point.y) = 0;
              v172 = transformPoint(&outPoint, v171, &point);
              v173 = v172->x;
              v174 = v172->y;
              v175 = v172->z;
              a6.x = v173;
              _z2 = v175;
              v168 = v166->__parent.m_nModelIndex;
              v169 = 0;
              _y2 = v174;
              if ( v168 == (unsigned __int16)miKmbContainerRed
                || v168 == (unsigned __int16)miKmbContainerYel
                || v168 == (unsigned __int16)miKmbContainerBlue
                || v168 == (unsigned __int16)miKmbContainerOpen )
              {
                v176 = modelPtrs[v168]->pColModel->boundingBox.min.z;
                v177 = (CMatrix *)v166->__parent.__parent.xyz;
                v242 = -v176;
                LODWORD(v240.x) = 0;
                v241 = 0;
                transformPoint(&releasedRope.x, v177, &v240);
                if ( _releasedRope.z > (long double)_z2 )
                {
                  LODWORD(a6.x) = (_DWORD)releasedRope.x.x;
                  _y2 = _releasedRope.y;
                  _z2 = _releasedRope.z;
                  v169 = 2;
                }
                v179 = (CMatrix *)v166->__parent.__parent.xyz;
                releasedRopePos.x_scaled.x = modelPtrs[v166->__parent.m_nModelIndex]->pColModel->boundingBox.min.z;
                v244 = 0;
                v245 = 0;
                v178 = transformPoint(&v257, v179, &releasedRopePos.x_scaled);
                releasedRope.x.x = v178->x;
                _releasedRope.y = v178->y;
                _releasedRope.z = v178->z;
                if ( _releasedRope.z > (long double)_z2 )
                {
                  v180 = v178->y;
                  a6.x = v178->x;
                  v181 = v178->z;
                  _y2 = v180;
                  _z2 = v181;
                  v169 = 3;
                }
                v183 = modelPtrs[v166->__parent.m_nModelIndex]->pColModel->boundingBox.min.z;
                v184 = (CMatrix *)v166->__parent.__parent.xyz;
                v238 = 0;
                velocity_final_x.x = -v183;
                v239 = 0.0;
                v182 = transformPoint(&v246, v184, &velocity_final_x);
                releasedRope.x.x = v182->x;
                _releasedRope.y = v182->y;
                _releasedRope.z = v182->z;
                if ( _releasedRope.z > (long double)_z2 )
                {
                  v185 = v182->x;
                  v186 = v182->y;
                  v187 = v182->z;
                  a6.x = v185;
                  _y2 = v186;
                  _z2 = v187;
                  v169 = 1;
                }
              }
              if ( sqrt((_z2 - z1) * (_z2 - z1) + (_y2 - y1) * (_y2 - y1) + (a6.x - x1) * (a6.x - x1)) < 2.5 )
              {
                *ppAttachedEntity = (CObject *)v166;
                CEntity__registerReference(&v166->__parent, (int)ppAttachedEntity);
                (*ppAttachedEntity)->__parent.nImmunities |= 0x80000000u;
                v190 = *ppAttachedEntity;
                v239 = modelPtrs[v166->__parent.m_nModelIndex]->pColModel->boundingBox.min.z;
                LODWORD(velocity_final_x.x) = 0;
                v238 = 0;
                v191 = transformPoint(&v246, &v190->__parent.__parent.__parent.xyz->matrix, &velocity_final_x);
                v189 = pThisPtr;
                CPhysical::SetPosition(&pThisPtr->m_pRopeAttacherObject->__parent, v191);
                v189->m_pRopeAttacherObject->__parent.__parent.BitFlags &= 0xFFFFFFFEu;
                v188 = (*ppAttachedEntity)->__parent.__parent.BitFlags;
                if ( v188 & 4 || v188 & 0x40000 )
                {
                  (*(void (__stdcall **)(_DWORD))((*ppAttachedEntity)->__parent.__parent.__parent.__vmt + 16))(0);
                  CPhysical::AddToMovingList(&(*ppAttachedEntity)->__parent);
                }
                v192 = z1 - _z2;
                (*ppAttachedEntity)->__parent.nImmunities |= 0x2000000u;
                if ( v192 > 0.0 )
                {
                  v194 = v192;
                  invMass_ = 1.0 / v189->m_fMass;
                  v193 = v189->m_uiRopeSegmentLength - v194 * invMass_ - invMass_ * v189->m_fRopeSegmentLength;
                  UNDEF(v196);
                  v189->m_uiRopeSegmentLength = v193;
                  if ( v197 | v198 )
                    v193 = 0.0099999998;
                  v189->m_uiRopeSegmentLength = v193;
                }
                if ( v169 > 0 )
                {
                  do
                  {
                    v199 = (*ppAttachedEntity)->__parent.__parent.__parent.xyz;
                    v200 = (*ppAttachedEntity)->__parent.__parent.__parent.xyz;
                    point.x = v200->matrix.matrix.right.x;
                    v201 = v200->matrix.matrix.right.y;
                    v202 = v200->matrix.matrix.right.z;
                    releasedRope.x.x = -point.x;
                    point.y = v201;
                    point.z = v202;
                    v199->matrix.matrix.right.x = v199->matrix.matrix.at.x;
                    v199->matrix.matrix.right.y = v199->matrix.matrix.at.y;
                    v199->matrix.matrix.right.z = v199->matrix.matrix.at.z;
                    v203 = (*ppAttachedEntity)->__parent.__parent.__parent.xyz;
                    _releasedRope.y = -v201;
                    v204 = _releasedRope.y;
                    v203 = (XYZ *)((char *)v203 + 32);
                    v205 = -point.z;
                    --v169;
                    LODWORD(v203->matrix.matrix.right.x) = (_DWORD)releasedRope.x.x;
                    _releasedRope.z = v205;
                    v206 = _releasedRope.z;
                    v203->matrix.matrix.right.y = v204;
                    v203->matrix.matrix.right.z = v206;
                  }
                  while ( v169 );
                }
                break;
              }
              v165 = LODWORD(v235);
              pObjectPool = LODWORD(v217);
            }
          }
        }
      }
    }
    while ( v164 );
  }
    LABEL_249:
    v207 = pThisPtr;
    if ( !(pThisPtr->m_ucFlags2 & 1) )
    {
        if ( pThisPtr->m_vecRopeSegments[0].z < -50.0 )
            CRope::dtor(pThisPtr);
    }
    v207->m_ucFlags2 &= 0xFEu;
}

bool CRope::Adjust(CVector vecRopeStart, int a5, RwV3D *pOutVector)
{
    CVector
        vecLastSegment = m_vecRopeSegments[m_uiSegmentCount];
    m_vecRopeSegments[31] = vecRopeStart;
    CVector
        vecDiff = vecRopeStart - vecLastSegment;
    float
        unusedRopeLength = (long double)(31 - m_uiSegmentCount) * m_fRopeSegmentLength,
        usedRopeLength = vecDiff.Length();
    if(usedRopeLength >= unusedRopeLength)
    {
        pOutVector = vecDiff * (unusedRopeLength / usedRopeLength) + vecLastSegment;
        size_t seg = pThis->m_uiSegmentCount + 1;
        for(; seg < 32; seg++)
        {
            vecDiff.Normalize();
            vecDiff *= m_fRopeSegmentLength;
            m_vecRopeSegments[seg] = vecDiff * (seg - pThis->m_uiSegmentCount) + vecLastSegment;
        }
        return 1;
    }
    else
    {
        v14 = 6;
        do
        {
            for ( i = 31; i > pThis->m_uiSegmentCount + 1; --i )
            {
                v16 = (int)((char *)pThis + 12 * i);
                v18 = pThis->m_vecRopeSegments[i].x - *(float *)(v16 - 12);
                v17 = (RwV3D *)(v16 - 12);
                v19 = *(float *)(v16 + 4) - *(float *)(v16 - 12 + 4);
                v20 = *(float *)(v16 + 8);
                LODWORD(v73) = *(_DWORD *)(v16 - 12);
                v21 = v20 - *(float *)(v16 - 12 + 8);
                LODWORD(v22) = *(_DWORD *)(v16 - 12 + 4);
                LODWORD(v75) = *(_DWORD *)(v16 - 12 + 8);
                v74 = v22;
                if ( sqrt(v18 * v18 + v21 * v21 + v19 * v19) <= pThis->m_fRopeSegmentLength )
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
                v26 = pThis->m_fRopeSegmentLength / sqrt(v23 * v23 + v25 * v25 + v24 * v24);
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
                v32 = (RwV3D *)((char *)pThis->m_vecRopeSegments + 4 * (3 * i + 93));
                v33 = v77;
                v34 = v89 * v31;
                v32->x = v76;
                v32->y = v33;
                v78 = v34;
                v32->z = v78;
                }
            }
            v35 = pThis->m_uiSegmentCount + 1;
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
            if ( sqrt(v40 * v40 + v38 * v38 + v39 * v39) > pThis->m_fRopeSegmentLength )
            {
            v80 = v39;
            v81 = v38;
            v79 = v40;
            v41 = pThis->m_fRopeSegmentLength / sqrt(v40 * v40 + v63 * v63 + v62 * v62);
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
        if ( pThis->m_uiSegmentCount + 1 < 31 )
        {
            v44 = (int)&pThis->m_vecRopeSegments[30].z;
            while ( 1 )
            {
                v47 = *(float *)(v44 + 4) - *(float *)(v44 - 8);
                v46 = *(float *)(v44 + 8) - *(float *)(v44 - 4);
                v45 = *(float *)(v44 + 12) - *(float *)v44;
                if ( sqrt(v47 * v47 + v45 * v45 + v46 * v46) <= pThis->m_fRopeSegmentLength )
                    break;
                --v43;
                v44 -= 0xCu;
                v82 = *(float *)(v44 + 4) - *(float *)(v44 + 0x10);
                v83 = *(float *)(v44 + 8) - *(float *)(v44 + 0x14);
                v84 = *(float *)(v44 + 0xC) - *(float *)(v44 + 0x18);
                v49 = v45;
                v48 = v46;
                v50 = pThis->m_fRopeSegmentLength / sqrt(v47 * v47 + v49 * v49 + v48 * v48);
                v87 = v83 * v50;
                v88 = v84 * v50;
                v79 = v82 * v50 + *(float *)(v44 + 16);
                v80 = v87 + *(float *)(v44 + 20);
                v51 = v88 + *(float *)(v44 + 0x18);
                *(_DWORD *)(v44 + 4) = LODWORD(v79);
                *(_DWORD *)(v44 + 8) = LODWORD(v80);
                v81 = v51;
                *(_DWORD *)(v44 + 12) = LODWORD(v81);
                if ( v43 <= pThis->m_uiSegmentCount + 1 )
                    return 0;
            }
        }
        return 0;
    }
}
