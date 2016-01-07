#pragma once

#include "Maths\CVector.h"

struct LimbOrientation
{
	float yaw;
	float pitch;
};

class CPed;

class CPedIK
{
public:
	CPedIK(CPed* ped);
	/*
	* ExtractYawAndPitchWorld(RwMatrixTag*, float&, float&)
	*/ 
	void GetWorldMatrix(RwFrame* frame, RwMatrixTag* matrix);
	/* MoveLimb(LimbOrientation&, float, float, LimbMovementInfo&)
	* MoveLimb(LimbOrientation&, float, float, LimbMovementInfo&, float)
	* PitchForSlope()
	* PointGunAtPosition(CVector const&, float)
	* PointGunInDirection(float, float, bool, float)
	* RotateTorso(AnimBlendFrameData*, LimbOrientation&, bool)
	*/ 
	void RotateTorsoForArm(const CVector& vec);
protected:
private:
	CPed* m_ped;
	LimbOrientation m_torsoOrientation;
	float m_slopePitch;
	float m_slopePitchLimitMult;
	float m_slopeRoll;
	float m_bodyRoll;
	uint32_t m_flags;
	// static
	* XaxisIK
	* YaxisIK
	* ZaxisIK
	* ms_headInfo
	* ms_headRestoreInfo
	* ms_lowerArmInfo
	LimbMovementInfo ms_torsoInfo;
	* ms_upperArmInfo
};