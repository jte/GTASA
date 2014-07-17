#pragma once

class CPedIK
{
public:
	* CPedIK(CPed*)
	* ExtractYawAndPitchWorld(RwMatrixTag*, float&, float&)
	* GetWorldMatrix(RwFrame*, RwMatrixTag*)
	* MoveLimb(LimbOrientation&, float, float, LimbMovementInfo&)
	* MoveLimb(LimbOrientation&, float, float, LimbMovementInfo&, float)
	* PitchForSlope()
	* PointGunAtPosition(CVector const&, float)
	* PointGunInDirection(float, float, bool, float)
	* RotateTorso(AnimBlendFrameData*, LimbOrientation&, bool)
	* RotateTorsoForArm(CVector const&)
protected:
private:
	* XaxisIK
	* YaxisIK
	* ZaxisIK
	* ms_headInfo
	* ms_headRestoreInfo
	* ms_lowerArmInfo
	LimbMovementInfo ms_torsoInfo;
	* ms_upperArmInfo
};