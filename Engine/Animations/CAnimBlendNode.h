#pragma once

class CAnimBlendNode
{
public:
	void CalcDeltas();
	//* CalcDeltasCompressed()                                #
	//* FindKeyFrame(float)                                   #
	void GetCurrentTranslation(CVector& currentTranslation, float mult);
	//* GetCurrentTranslationCompressed(CVector&, float)      #
	void GetEndTranslation(CVector& vecEndTranslation, float mult);
	//* GetEndTranslationCompressed(CVector&, float)          #
	void Init();
	//* NextKeyFrameCompressed()                              #
	//* NextKeyFrameNoCalc()
	//* SetupKeyFrameCompressed()                             #
	//* Update(CVector&, CQuaternion&, float)                 #
	//* UpdateCompressed(CVector&, CQuaternion&, float)       #
	//* UpdateTime()
private:
    float m_theta0; // angle between two quaternions
    float m_theta1; // 1 / sin(m_theta0), used in slerp calculation
    uint16_t m_endKeyFrameId;
    uint16_t m_startKeyFrameId;
    float m_timeDelta;
    CAnimBlendSequence* m_sequence;
    CAnimBlendAssociation* m_assoc;
};