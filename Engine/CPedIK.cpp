#include "StdInc.h"

CPedIK::CPedIK(CPed* ped)
{
	m_ped = ped;
	m_flags = 0;
	m_torsoOrientation.yaw = 0.0f;
	m_torsoOrientation.pitch = 0.0f;
	m_slopePitch = 0.0f;
	m_slopePitchLimitMult = 0.0f;
	m_slopeRoll = 0.0f;
	m_bodyRoll = 0.0f;
}

void CPedIK::GetWorldMatrix(RwFrame* frame, RwMatrixTag* matrix)
{
	*matrix = frame->modelling;
	for (RwFrame* parent = RwFrameGetParent(frame); parent != NULL; parent = RwFrameGetParent(parent))
	{
		RwMatrixTransform(matrix, &parent->modelling, rwCOMBINEPOSTCONCAT);
	}
}

void CPedIK::RotateTorsoForArm(const CVector& a2)
{
  CPedIK *v2; // edi@1
  CPed *v3; // ecx@1
  CMatrixLink *v4; // eax@1
  int v5; // esi@1
  CMatrixLink *v6; // eax@3
  int v7; // edx@3
  double v8; // st7@5
  double v9; // st7@9
  double v10; // st7@10
  float v11; // ST08_4@17
  float v12; // ST08_4@18
  float v13; // [sp+14h] [bp+Ch]@17

  v2 = this;
  v3 = this->pPed;
  v4 = v3->__parent.__parent.__parent.xyz;
  v5 = (int)&v4->matrix.matrix.pos;
  if ( !v4 )
    v5 = (int)&v3->__parent.__parent.__parent.transform;
  v6 = v3->__parent.__parent.__parent.xyz;
  v7 = (int)&v6->matrix.matrix.pos;
  if ( !v6 )
    v7 = (int)&v3->__parent.__parent.__parent.transform;
  v8 = atan2(a2->y - *(float *)(v5 + 4), -(a2->x - *(float *)v7));
  if ( v8 <= m_ped->GetCurrentRotation() + M_PI )
  {
    if ( v8 < m_ped->GetCurrentRotation() - M_PI )
      v8 = v8 + 2 * M_PI;
  }
  else
  {
    v8 = v8 - 2 * M_PI;
  }
  v9 = v8 - v3->fCurrentRotation;
  if ( v9 <= 0.78539819 )
  {
    if ( v9 >= -1.0471976 )
      return;
    v10 = v9 - -1.0471976;
    if ( v10 < -0.34906587 )
      v10 = -0.34906587;
  }
  else
  {
    v10 = v9 - 0.78539819;
    if ( v10 > 0.78539819 )
      v10 = 0.78539819;
  }
  if ( v10 != 0.0 )
  {
    if ( byte_8D2354 )
    {
      v13 = v10 * 0.5;
      v11 = v13 * 57.295776;
      RtQuatRotate(*(RtQuat **)(v3->pBodyParts.pSpecialCostume + 16), &CPedIK::XaxisIK, v11, 2);
      v10 = v13;
    }
    v12 = v10 * 57.295776;
    RtQuatRotate(v2->pPed->pBodyParts.pHead->pOrientation, &CPedIK::XaxisIK, v12, 2);
  }
}