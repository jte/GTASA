#pragma once

class CRope
{
public:
	//* CreateHookObjectForRope()
	//* DoControlsApply()
	//* PickUpObject(CEntity*)
	//* ReleasePickedUpObject()
	//* Remove()
	void Render();
	//* Update()
	//* UpdateWeightInRope(CVector, float, CVector*)
protected:
private:
    CVector m_ropeSegments[32];
    CVector m_ropeSegmentsReleased[32];

00000300 m_pRopeAttachToEntity dd ?              ; offset
00000304 m_pad4          dd ?
00000308 m_fMass         dd ?
0000030C m_fRopeSegmentLength dd ?
00000310 m_pRopeHolder   dd ?                    ; offset
00000314 m_pRopeAttacherObject dd ?              ; offset
00000318 m_pAttachedEntity dd ?                  ; offset
0000031C m_uiRopeSegmentLength dd ?
00000320 m_uiHoldEntityExpireTime dd ?
00000324 m_uiSegmentCount db ?
00000325 m_ucRopeType    db ?
00000326 m_ucFlags1      db ?
00000327 m_ucFlags2      db ?
};