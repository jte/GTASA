#include "StdInc.h"

void FxSystem_c::AttachToBone(CEntity* entity, int boneId)
{
    RpHAnimHierarchy* animHier = GetAnimHierarchyFromSkinClump((RpClump*)entity->GetRwObject());
    RwInt32 nodeIndex = RpHAnimIDGetIndex(animHier, boneId);
    RwMatrix* matrixArray = RpHAnimHierarchyGetMatrixArray(animHier);
    m_parentMatrix = &matrixArray[nodeIndex];
}

void FxSystem_c::SetOffsetPos(RwV3d* offsetPos)
{
    m_offsetMatrix.pos = *offsetPos;
    RwMatrixUpdate(&m_offsetMatrix);
}

void FxSystem_c::AddOffsetPos(RwV3d* offsetPos)
{
    RwV3dAdd(&m_offsetMatrix.pos, &m_offsetMatrix.pos, offsetPos);
    RwMatrixUpdate(&m_offsetMatrix);
}

void FxSystem_c::SetRateMult(float rateMult)
{
    m_rateMult = FloatToFixedPoint<1000>(rateMult);
}

void FxSystem_c::SetTimeMult(float timeMult)
{
    m_timeMult = FloatToFixedPoint<1000>(timeMult);
}

void FxSystem_c::SetVelAdd(RwV3d* velAdd)
{
    m_velocityAdd = *velAdd;
}

void FxSystem_c::CopyParentMatrix()
{
    RwMatrix* old = m_parentMatrix;
    m_parentMatrix = RwMatrixCreate();
    *m_parentMatrix = *old;
    m_parentMatrix->flags |= 1;
}

void FxSystem_c::GetCompositeMatrix(RwMatrixTag* compositeMatrix)
{
    if (m_parentMatrix)
    {
        RwMatrixMultiply(compositeMatrix, &m_offsetMatrix, m_parentMatrix);
    }
    else
    {
        *compositeMatrix = m_offsetMatrix;
    }
}

uint8_t FxSystem_c::GetPlayStatus()
{
    return m_playStatus;
}

bool FxSystem_c::GetBoundingSphereWld(FxSphere_c* sphere)
{
    if (!m_boundingSphere)
    {
        return false;
    }
    RwMatrixTag* composite = g_fxMan.FxRwMatrixCreate();
    GetCompositeMatrix(composite);
    RwV3dTransformPoints(&sphere->center, &m_boundingSphere->center, 1, composite);
    sphere->center = m_boundingSphere->center;
    g_fxMan.FxRwMatrixDestroy(composite);
    return true;
}

int FxSystem_c::AddParticle(RwV3d* pVecPosition, RwV3d* pVecDirection, float fBlur, FxPrtMult_c* pParticleInfo, float a6, float fBrightness, float a8, unsigned char a9)
{
	signed int v11 = (unsigned __int64)((double)rand() * 0.000030517578 * 100.0);
	if ( g_fx.GetFxQuality() != 0 || v11 >= 50 )
	{
		if ( g_fx.GetFxQuality() != 1 || v11 >= 25 )
		{
			float v14 = 1.0;
			if ( fBrightness < (double)a8 )
				v14 = 1.0 - a8 + fBrightness;
			for (size_t i = 0; i < this->m_fxSystemBP->GetNumFxEmitterBP(); i++)
			{
				FxEmitter_c* emitter = m_fxEmitters[i];
				if (emitter->IsActive())
				{
					(*(void (__stdcall **)(_DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD))(v13->vtbl + 0x14))(
					pVecPosition,
					pVecDirection,
					fBlur,
					pParticleInfo,
					LODWORD(a6),
					LODWORD(v14),
					a9);
				}
			}
		}
	}
}