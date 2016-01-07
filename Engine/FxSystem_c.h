#pragma once

#include "List_c.h"
#include "FxSphere_c.h"

class FxSystem_c
{
public:
	void AddOffsetPos(RwV3d* offsetPos);
	/* AddParticle(RwMatrixTag*, RwV3d*, float, FxPrtMult_c*, float, float, float, unsigned char)
	* 
	*/
	void AddParticle(RwV3d*, RwV3d*, float, FxPrtMult_c*, float, float, float, unsigned char);
    void AttachToBone(CEntity* entity, int boneId);
	void CopyParentMatrix();
	/* DoFxAudio(CVector)
	* EnablePrim(int, unsigned char)
	* Exit()
	* ForAllParticles(void (*)(Particle_c*, int, void**), void*)
	* FxSystem_c()
	* GetBoundingBox(FxBox_c*)
	* GetBoundingSphereLcl(FxSphere_c*)
	*/ 
    bool GetBoundingSphereWld(FxSphere_c* sphere);
    void GetCompositeMatrix(RwMatrixTag* compositeMatrix);
	uint8_t GetPlayStatus();
	/* Init(FxSystemBP_c*, RwMatrixTag*, RwMatrixTag*)
	* IsVisible()
	* Kill()
	* Pause()
	* Play()
	* PlayAndKill()
	* ResetBoundingSphere()
	* SetBoundingSphere(FxSphere_c*)
	* SetConstTime(unsigned char, float)
	* SetLocalParticles(unsigned char)
	* SetMatrix(RwMatrixTag*)
	* SetMustCreatePrts(unsigned char)
	*/ 
    void SetOffsetPos(RwV3d* offsetPos);
	void SetRateMult(float rateMult);
	void SetTimeMult(float timeMult);
	void SetVelAdd(RwV3d* velAdd);
	/* SetZTestEnable(unsigned char)
	* Stop()
	* Update(RwCamera*, float)
	* UpdateBoundingBoxCB(Particle_c*, int, void**)
	* ~FxSystem_c()*/
private:
    ListItem_c m_inList;
    FxSystemBP_c* m_fxSystemBP;
    RwMatrix* m_parentMatrix;
    RwMatrix m_offsetMatrix;
    uint8_t m_playStatus;
    uint8_t m_particleState;
    uint8_t m_constTimeSet;
    uint8_t pad1;
    uint32_t pad2;
    float m_distanceToCamera;
    FixedPoint m_constTime;
    FixedPoint m_rateMult;//_C=1000
    FixedPoint m_timeMult;//_C=1000
    uint8_t flags;
    uint8_t pad3;
    uint32_t pad4;
    CVector m_velocityAdd;
    FxSphere_c* m_boundingSphere;
    FxEmitter_c** m_fxEmitters;
    union
    {
    //CAEAudioFireEntity m_audioEntity;
    uint32_t m_audioEntity[34];
    };
};