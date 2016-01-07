#include "StdInc.h"

ColNodeInfo CPedModelInfo::m_pColNodeInfos[12] =
{
	{0, 5, 9,	CSphere(0.05, 0.0, 0.0, 0.15)},
	{0, 3, 3,	CSphere(0.2, 0.0, 0.0, 0.2)},
	{0, 3, 3,	CSphere(0.0, 0.0, 0.0, 0.2)},
	{0, 2, 4,	CSphere(-0.1, 0.0, 0.0, 0.2)},
	{0, 0x20, 5,CSphere(0.06, 0.0, 0.0, 0.14)},
	{0, 0x16, 6,CSphere(0.06, 0.0, 0.0, 0.14)},
	{0, 0x21, 5,CSphere(0.05, 0.0, 0.0, 0.14)},
	{0, 0x17, 6,CSphere(0.05, 0.0, 0.0, 0.14)},
	{0, 0x2A, 7,CSphere(-0.1, 0.0, 0.0, 0.18)},
	{0, 0x34, 8,CSphere(-0.1, 0.0, 0.0, 0.18)},
	{0, 0x2B, 7,CSphere(-0.18, 0.0, 0.0, 0.16)},
	{0, 0x35, 8,CSphere(-0.18, 0.0, 0.0, 0.16)}
};

RwObjectNameIdAssocation CPedModelInfo::m_pPedIds[13] =
{
	{"Smid", 1, 0},
	{"Shead", 2, 0},
	{"Supperarml", 3, 0},
	{"Supperarmr", 4, 0},
	{"SLhand", 5, 0},
	{"SRhand", 6, 0},
	{"Supperlegl", 7, 0},
	{"Supperlegr", 8, 0},
	{"Sfootl", 9, 0},
	{"Sfootr", 10, 0},
	{"Slowerlegl", 11, 0},
	{"Slowerlegr", 12, 0},
	{0,0,0}
};

size_t CPedModelInfo::GetModelType()
{
	return 7;
}

void CPedModelInfo::DeleteRwObject()
{
	CClumpModelInfo::DeleteRwObject();
	if (m_hitColModel)
	{
		delete m_hitColModel;
		m_hitColModel = NULL;
	}
}

void* CPedModelInfo::SetClump(RpClump* clump)
{
	CClumpModelInfo::SetClump(clump);
	CClumpModelInfo::SetFrameIds(&m_pPedIds);
	if (!m_hitColModel)
	{
		CreateHitColModelSkinned(clump);
	}
	RpClumpForAllAtomics((RpClump*)GetRwObject(), CClumpModelInfo::SetAtomicRendererCB, CVisibilityPlugins::RenderPedCB);
	return (void*)GetAnimHierarchyFromClump(clump);
}

void CPedModelInfo::CreateHitColModelSkinned(RpClump* clump)
{
	RpHAnimHierarchy* hier = GetAnimHierarchyFromSkinClump(clump);
	CColModel* hitColModel = new CColModel;
	RwMatrixTag* frameInverseMat = CGame::GetWorkingMatrix1();
	RwMatrixTag* transformMatrix = CGame::GetWorkingMatrix2();
	hitColModel->AllocateData(12, 0, 0, 0, 0, 0);
	RwMatrixInvert(frameInverseMat, &RpClumpGetFrame(clump)->modelling);
	for (size_t i = 0; i < ELEMS_COUNT(m_pColNodeInfos); i++)
	{
		CColSphere colSphere;
		colSphere.Set(m_pColNodeInfos[i].sphere.GetRadius(), m_pColNodeInfos[i].sphere.GetCenter());
		CCollisionData* colData = hitColModel->GetColData();
		*transformMatrix = *frameInverseMat;
		RwInt32 matIndex = RpHAnimIDGetIndex(hier, m_pColNodeInfos[i].animId) << 6;
		RwMatrix* mat = (RwMatrix*)(RpHAnimHierarchyGetMatrixArray(hier) + matIndex);
		RwMatrixTransform((RwMatrix *)transformMatrix, mat, rwCOMBINEPRECONCAT);
		RwV3dTransformPoints(&colSphere.center, &colSphere.center, 1, (RwMatrix *)transformMatrix);
		colSphere.SetSurfaceTypeA(EColSurface::PED);
		colSphere.SetSurfaceTypeB(m_pColNodeInfos[i].surfaceTypeB);
		colData->SetSphere(i, colSphere);
	}
	CBoundingBox boundingBox;
	CVector maxbb(0.5f, 0.5f, 1.2f);
	CVector minbb(-0.5f,-0.5f,-1.2f);
	CBox box;
	box.Set(minbb, maxbb);
	boundingBox.SetBoundingSphere(CSphere(0, 0, 0, 1.5f));
	hitColModel->SetBoundingBox(boundingBox);
	hitColModel->SetColNum(0);
	m_hitColModel = hitColModel;
}

void CPedModelInfo::IncrementVoice()
{
	if (m_firstVoice < 0 || m_lastVoice < 0)
	{
		m_nextVoice = -1;
	}
	else
	{
		m_nextVoice++;
		if (m_nextVoice > m_lastVoice || m_nextVoice < m_firstVoice)
		{
			m_nextVoice = m_firstVoice;
		}
	}
}

CColModel* CPedModelInfo::AnimatePedColModelSkinned(RpClump* clump)
{
  v2 = this;
  v3 = this->HitColModel;
  v21 = this;
	if (!m_hitColModel)
	{
		CreateHitColModelSkinned(clump);
		return m_hitColModel;
	}
    v20 = v3->pColData;
    v5 = GetAnimHierarchyFromSkinClump(c);
    v6 = CGame::m_pWorkingMatrix1;
    v7 = CGame::m_pWorkingMatrix2;
    v8 = &c->object.parent->modelling;
    v19 = v5;
    LODWORD(b.x) = 0;
    LODWORD(b.y) = 0;
    LODWORD(b.z) = 0;
    v18 = CGame::m_pWorkingMatrix1;
    unk_7F2070(CGame::m_pWorkingMatrix1, v8);
    ca = 0;
    v9 = (char *)&CPedModelInfo::m_pColNodeInfos[0].sphere.vecPosition.fY;
    do
    {
      LODWORD(v10) = *((_DWORD *)v9 - 1);
      LODWORD(v11) = *((_DWORD *)v9 + 1);
      LODWORD(center.y) = *(_DWORD *)v9;
      center.x = v10;
      center.z = v11;
      memcpy(v7, v6, 0x40u);
      v12 = RpHAnimIDGetIndex(v19, *((_DWORD *)v9 - 3)) << 6;
      RpHAnimHierarchyGetMatrixArray(v19);
      RwMatrixTransform((RwMatrix *)v7, (RwMatrix *)(v13 + v12), 1);
      v7EDD90(&center, &center, 1, (RwMatrix *)v7);
      v6 = v18;
      v14 = (int)((char *)v20->m_pSpheres + ca);
      *(_DWORD *)v14 = (_DWORD)center.x;
      *(_DWORD *)(v14 + 4) = LODWORD(center.y);
      v9 += 28;
      *(_DWORD *)(v14 + 8) = LODWORD(center.z);
      ca += 20;
    }
    while ( (signed int)v9 < (signed int)&componentListCar );

    center.y = b.y;
    LODWORD(center.x) = (_DWORD)b.x;
    center.z = b.z;
    memcpy(v7, v18, 0x40u);
    v15 = RpHAnimIDGetIndex(v19, 3);
    RpHAnimHierarchyGetMatrixArray(v19);
    RwMatrixTransform((RwMatrix *)v7, (RwMatrix *)((v15 << 6) + v16), 1);
    v7EDD90(&center, &center, 1, (RwMatrix *)v7);
    CSphere::Set((RwSphere *)&v21->HitColModel->boundingBox.vecBoundOffset, 1.5, &center);
    
	b.x = center.x + 1.2;
    b.y = center.y + 1.2;
    b.z = center.z + 1.2;
    a.x = center.x - 1.2;
    a.y = center.y - 1.2;
    a.z = center.z - 1.2;
	v17 = (CBox *)v21->HitColModel;
    CBox::Set(v17, &a, &b);
	return m_hitColModel;
}