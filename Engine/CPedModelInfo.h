#pragma once

#include "CBaseModelInfo.h"

struct ColNodeInfo
{
	uint32_t pad1;
	uint32_t animId;
	EColSurface surfaceTypeB;
	CSphere sphere;
};

class CPedModelInfo : public CClumpModelInfo
{
public:
	// virtual methods
	virtual ~CPedModelInfo();
	virtual size_t GetModelType();
	virtual void DeleteRwObject();
	virtual void* SetClump(RpClump* clump);
	//
	
	void AddXtraAtomics(RpClump*); // not implemented in GTASA
	CColModel* AnimatePedColModelSkinned(RpClump* clump);//NEEDS WORK
	/* AnimatePedColModelSkinnedWorld(RpClump*)
	*/ 
	void CreateHitColModelSkinned(RpClump* clump);
	void IncrementVoice();
	void SetFaceTexture(RwTexture*); // not implemented in GTASA
	/* ~CPedModelInfo()
	*/
private:
	uint32_t pad1;
	uint32_t AssocGroupId;
	uint32_t pedType;
	uint32_t pedStats;
	uint16_t canDriveCars;
	uint16_t pedFlags;
	CColModel* m_hitColModel;
	uint8_t FirstRadioStation;
	uint8_t SecondRadioStation;
	uint16_t Race;
	uint16_t AudioPedType;
	int16_t m_firstVoice;
	int16_t m_lastVoice;
	int16_t m_nextVoice;
	// static members
	static ColNodeInfo m_pColNodeInfos[12];
	static RwObjectNameIdAssocation m_pPedIds[13];
};