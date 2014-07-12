#pragma once

class CObjectInfo
{
public:
    float fMass; // 0
    float fTurnMass; // 4
    float fAirResistance; // 8
    float fElasticity; // 12
    float fBuoyancy; // 16
    float fUprootLimit; // 20
    float fColDamageMultiplier; // 24
    uint8_t ucColDamageEffect; // 28
    uint8_t ucSpecialColResponseCase; // 29
    uint8_t ucCameraAvoidObject; // 30
    uint8_t ucCausesExplosion; // 31
    uint8_t ucFxType; // 32
    uint8_t pad1[3]; // 33
    CVector vecFxOffset; // 36
    void* pFxSystem; // ret from CParticleData::GetDataFromName // 48
    float fSmashMultiplier; // 52
    CVector vecBreakVelocity; // 56
    float fBreakVelocityRand; // 68
    uint32_t uiGunBreakMode; // 72
    uint32_t uiSparksOnImpact; // 76
};

class CDummyObject;

class CObject : public CEntity
{
public:
	void AddToControlCodeList();
	CObject();                       
	CObject(CDummyObject*);                        
	CObject(int, bool);                          
	void CanBeDeleted();                             
	void CanBeTargetted();                 
	void CanBeUsedToTakeCoverBehind();                
	void Create(CDummyObject*);                         
	void Create(int, bool);                             
	bool CreateRwObject();                    
	void DeleteAllMissionObjects();                     
	void DeleteAllTempObjects();   
	void DeleteAllTempObjectsInArea(CVector, float);    
	void DoBurnEffect();             
	void Explode();              
	void GetLightingFromCollisionBelow();              
	void GetObjectCarriedWithRope();                   
	void GetRopeHeight();        
	void GrabObjectToCarryWithRope(CPhysical*);        
	void Init();                                       
	void Load();
	void LockDoor();
	void ObjectDamage(float, CVector*, CVector*, CEntity*, eWeaponType);   
	void ObjectFireDamage(float, CEntity*);          
	void PreRender();                             
	void ProcessControl();                        
	void ProcessControlLogic();                 
	void ProcessGarageDoorBehaviour();                  
	void ProcessSamSiteBehaviour();              
	void ProcessTrainCrossingBehaviour();             
	void RefModelInfo(int iRefModelIndex);
	void ReleaseObjectCarriedWithRope();               
	void RemoveFromControlCodeList();
	void RemoveLighting(bool);
	void Render();
	void ResetDoorAngle();                
	void Save();
	virtual void SetIsStatic(bool bLockAngle);       
	void SetMatrixForTrainCrossing(CMatrix*, float);    
	void SetObjectTargettable(unsigned char);    
	void SetRelatedDummy(CDummyObject *pDummy);
	void SetRemapTexture(RwTexture*, char const*);
	void SetRopeHeight(float);         
	void SetupLighting();
	void SpecialEntityCalcCollisionSteps(bool&, bool&);
	void SpecialEntityPreCollisionStuff(CPhysical*, bool, bool&, bool&, bool&, bool&);                                    
	void Teleport(CVector, unsigned char);
	bool TryToExplode();                 
	void TryToFreeUpTempObjects(int);                  
	void operator delete(void*);                      
	void operator new(unsigned int);                        
	void operator new(unsigned int, int);                   
	~CObject()                                        
private:
	static bool bAircraftCarrierSamSiteDisabled;
	static bool bArea51SamSiteDisabled;
	static float fDistToNearestTree;
	static size_t nBodyCastHealth;
	static size_t nNoTempObjects;
private:
    void* m_pControlCodeListLink; // 312
    uint8_t m_ucCreator; // 316
    uint8_t pad2; // 317
    uint16_t pad3; // 318

    // flags
    uint32_t b0x01 : 1; // 320
    uint32_t b0x02 : 1;
    uint32_t b0x04 : 1;
    uint32_t b0x08 : 1;
    uint32_t b0x10 : 1;
    uint32_t b0x20 : 1;
    uint32_t bExploded : 1;
    uint32_t b0x80 : 1;

    uint32_t b0x100 : 1; // 321
    uint32_t b0x200 : 1;
    uint32_t b0x400 : 1;
    uint32_t b0x800 : 1;
    uint32_t b0x1000 : 1;
    uint32_t b0x2000 : 1;
    uint32_t b0x4000 : 1;
    uint32_t bDoorLocked : 1;

    uint32_t b0x10000 : 1; // 322
    uint32_t bUpdateScale : 1;
    uint32_t b0x40000 : 1;
    uint32_t b0x80000 : 1;
    uint32_t b0x100000 : 1;
    uint32_t b0x200000 : 1;
    uint32_t b0x400000 : 1;
    uint32_t b0x800000 : 1;

    uint32_t b0x1000000 : 1; // 323
    uint32_t b0x2000000 : 1;
    uint32_t b0x4000000 : 1;
    uint32_t b0x8000000 : 1;
    uint32_t b0x10000000 : 1;
    uint32_t b0x20000000 : 1;
    uint32_t b0x40000000 : 1;
    uint32_t b0x80000000 : 1;

    uint8_t m_ucColDamageEffect; // 324
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
    CObjectInfo* m_pObjectInfo; // 352
    uint32_t pad16; // 356
    uint16_t pad17; // 360
    uint16_t m_pTexDictEntry; // 362
    uint32_t m_pRemapTexture; // 364
    CEntity* m_pRelatedDummy; // 368  CDummyObject - Might be used for other dynamic objects
    uint32_t m_burnTime; // 372
    uint32_t m_burnDamage; // 376
};