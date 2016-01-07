#pragma once

class CObjectInfo
{
public:
    float mass; // 0
    float turnMass; // 4
    float airResistance; // 8
    float elasticity; // 12
    float buoyancy; // 16
    float uprootLimit; // 20
    float colDamageMultiplier; // 24
    uint8_t colDamageEffect; // 28
    uint8_t specialColResponseCase; // 29
    uint8_t cameraAvoidObject; // 30
    uint8_t causesExplosion; // 31
    uint8_t fxType; // 32
    uint8_t pad1[3]; // 33
    CVector fxOffset; // 36
    void* fxSystem; // ret from CParticleData::GetDataFromName // 48
    float smashMultiplier; // 52
    CVector breakVelocity; // 56
    float breakVelocityRand; // 68
    uint32_t gunBreakMode; // 72
    uint32_t sparksOnImpact; // 76
};

class CDummyObject;

class CObject : public CPhysical
{
public:
    /*
	* AddToControlCodeList()
	*/ 
    CObject();
    //CObject(CDummyObject*);
	CObject(int modelIndex, bool create);
	/* CanBeDeleted()
	* CanBeTargetted()
	* CanBeUsedToTakeCoverBehind()
	* Create(CDummyObject*)
	* Create(int, bool)
	*/ 
    void CreateRwObject();
	/* DeleteAllMissionObjects()
	* DeleteAllTempObjects()
	* DeleteAllTempObjectsInArea(CVector, float)
	* DoBurnEffect()
	*/ 
    void Explode();
	/* GetLightingFromCollisionBelow()
	* GetObjectCarriedWithRope()
	* GetRopeHeight()
	* GrabObjectToCarryWithRope(CPhysical*)
	* Init()
	* Load()
	* LockDoor()
	* ObjectDamage(float, CVector*, CVector*, CEntity*, eWeaponType)
	* ObjectFireDamage(float, CEntity*)
	* PreRender()
	* ProcessControl()
	* ProcessControlLogic()
	* ProcessGarageDoorBehaviour()
	* ProcessSamSiteBehaviour()
	* ProcessTrainCrossingBehaviour()
	* RefModelInfo(int)
	* ReleaseObjectCarriedWithRope()
	* RemoveFromControlCodeList()
	* RemoveLighting(bool)
	* Render()
	* ResetDoorAngle()
	* Save()
	* SetIsStatic(bool)
	* SetMatrixForTrainCrossing(CMatrix*, float)
	* SetObjectTargettable(unsigned char)
	*/ 
    void SetRelatedDummy(CDummyObject* relatedDummy);
	/* SetRemapTexture(RwTexture*, char const*)
	* SetRopeHeight(float)
	* SetupLighting()
	* SpecialEntityCalcCollisionSteps(bool&, bool&)
	* SpecialEntityPreCollisionStuff(CPhysical*, bool, bool&, bool&, bool&, bool&)
	* Teleport(CVector, unsigned char)
	*/ 
    bool TryToExplode();
	/* TryToFreeUpTempObjects(int)
	* operator delete(void*)
	* operator new(unsigned int)
	* operator new(unsigned int, int)
	* ~CObject()
    */
private:
	static bool bAircraftCarrierSamSiteDisabled;
	static bool bArea51SamSiteDisabled;
	static float fDistToNearestTree;
	static size_t nBodyCastHealth;
	static size_t nNoTempObjects;
private:
    void* m_pControlCodeListLink; // 312
    uint8_t m_type; // 316
    uint8_t pad2; // 317
    uint16_t pad3; // 318

    // flags
    uint32_t b0x01 : 1; // 320
    uint32_t b0x02 : 1;
    uint32_t b0x04 : 1;
    uint32_t b0x08 : 1;
    uint32_t m_glassBroken : 1;
    uint32_t b0x20 : 1;
    uint32_t m_exploded : 1;
    uint32_t b0x80 : 1;

    uint32_t m_lampPost : 1; // 321
    uint32_t m_targetable : 1;
    uint32_t m_broken : 1;
    uint32_t m_trainCrossEnabled : 1;
    uint32_t m_photographed : 1;
    uint32_t m_liftable : 1;
    uint32_t b0x4000 : 1;
    uint32_t m_door : 1;

    uint32_t m_hasNoModel : 1; // 322
    uint32_t m_scaled : 1;
    uint32_t m_canBeAttachedToMagnet : 1;
    uint32_t b0x80000 : 1;
    uint32_t b0x100000 : 1;
    uint32_t b0x200000 : 1;
    uint32_t m_fadingIn : 1;
    uint32_t m_affectedByColBrightness : 1;

    uint32_t b0x1000000 : 1; // 323
    uint32_t m_doNotRender : 1;
    uint32_t b0x4000000 : 1;
    uint32_t b0x8000000 : 1;
    uint32_t b0x10000000 : 1;
    uint32_t b0x20000000 : 1;
    uint32_t b0x40000000 : 1;
    uint32_t b0x80000000 : 1;

    uint8_t m_colDamageEffect; // 324
    uint8_t pad4; // 325
    uint8_t pad5; // 326
    uint8_t pad6; // 327
    uint8_t m_ucBreakWeaponType; // 328
    uint8_t m_bHitByLOSCheck; // 329
    // model index of a model
    // that is referencing us
    uint16_t m_nRefModelIndex; // 330
    uint8_t m_colour1; // 332
    uint8_t m_colour2; // 333
    uint8_t m_colour3; // 334
    uint8_t m_colour4; // 335
    uint32_t m_uiObjectRemovalTime; // 336
    float m_fHealth; // 340
    uint32_t m_fLockedRotationAngle; // 344
    float m_fScale; // 348
    CObjectInfo* m_objectInfo; // 352
    uint32_t pad16; // 356
    uint16_t pad17; // 360
    uint16_t m_pTexDictEntry; // 362
    uint32_t m_pRemapTexture; // 364
    CDummyObject* m_relatedDummy; // 368  CDummyObject - Might be used for other dynamic objects
    uint32_t m_burnTime; // 372
    uint32_t m_burnDamage; // 376
};