#pragma once

class CWorld
{
public:
    void Add(CEntity *pEntity);
	void CallOffChaseForArea(float, float, float, float);
	void CallOffChaseForAreaSectorListPeds(CPtrList&, float, float, float, float, float, float, float, float);
	void CallOffChaseForAreaSectorListVehicles(CPtrList&, float, float, float, float, float, float, float, float);
	bool CameraToIgnoreThisObject(CEntity *pEntity);
	void CastShadow(float, float, float, float);
	void CastShadowSectorList(CPtrList&, float, float, float, float);
	void CheckBlockListIntegrity();
	void ClearCarsFromArea(float, float, float, float, float, float);
	void ClearExcitingStuffFromArea(CVector const&, float, unsigned char);
	void ClearForRestart();
	void ClearPedsFromArea(float, float, float, float, float, float);
	void ClearScanCodes();
	void DoZoneTestForChaser(CPhysical*);
	void ExtinguishAllCarFiresInArea(CVector, float);
	void FindGroundZFor3DCoord(float, float, float, bool*, CEntity**);
	void FindGroundZForCoord(float, float);
	void FindLodOfTypeInRange(unsigned int, CVector const&, float, bool, short*, short, CEntity**);
	void FindLowestZForCoord(float, float);
	void FindMissionEntitiesIntersectingCube(CVector const&, CVector const&, short*, short, CEntity**, bool, bool, bool);
	void FindMissionEntitiesIntersectingCubeSectorList(CPtrList&, CVector const&, CVector const&, short*, short, CEntity**, bool, bool, bool);
	void FindNearestObjectOfType(int, CVector const&, float, bool, bool, bool, bool, bool, bool);
	void FindNearestObjectOfTypeSectorList(int, CPtrList&, CVector const&, float, bool, CEntity**, float*);
	void FindObjectsInRange(CVector const &vecCenter, float fRadius, bool bIgnoreZ, short *usFoundCount, short usMaxCount, CEntity **pFoundObjectsList, bool bIncludeBuildings, bool bIncludeVehicles, bool bIncludePeds, bool bIncludeObjects, bool bIncludeDummies);
	void FindObjectsInRangeSectorList(CPtrList &pNode, CVector const &vecCenter, float fRadius, bool bIgnoreZ, short *usFoundCount, short usMaxCount, CEntity **pFoundObjectsList);
	void FindObjectsIntersectingAngledCollisionBox(CBox const&, CMatrix const&, CVector const&, float, float, float, float, short*, short, CEntity**, bool, bool, bool, bool, bool);
	void FindObjectsIntersectingAngledCollisionBoxSectorList(CPtrList&, CBox const&, CMatrix const&, CVector const&, short*, short, CEntity**);
	void FindObjectsIntersectingCube(CVector const&, CVector const&, short*, short, CEntity**, bool, bool, bool, bool, bool);
	void FindObjectsIntersectingCubeSectorList(CPtrList&, CVector const&, CVector const&, short*, short, CEntity**);
	void FindObjectsKindaColliding(CVector const&, float, bool, short*, short, CEntity**, bool, bool, bool, bool, bool);
	void FindObjectsKindaCollidingSectorList(CPtrList&, CVector const&, float, bool, short*, short, CEntity**);
	void FindObjectsOfTypeInRange(unsigned int, CVector const&, float, bool, short*, short, CEntity**, bool, bool, bool, bool, bool);
	void FindObjectsOfTypeInRangeSectorList(unsigned int, CPtrList&, CVector const&, float, bool, short*, short, CEntity**);
	void FindPlayerSlotWithPedPointer(void*);
	void FindPlayerSlotWithRemoteVehiclePointer(void*);
	void FindPlayerSlotWithVehiclePointer(CEntity*);
	void FindRoofZFor3DCoord(float, float, float, bool*);
	void FindUnsuspectingTargetCar(CVector, CVector);
	void FindUnsuspectingTargetPed(CVector, CVector);
	void GetIsLineOfSightClear(CVector const&, CVector const&, bool, bool, bool, bool, bool, bool, bool);
	void GetIsLineOfSightSectorClear(CSector&, CRepeatSector&, CColLine const&, bool, bool, bool, bool, bool, bool, bool);
	void GetIsLineOfSightSectorListClear(CPtrList&, CColLine const&, bool, bool);
	void HandleCollisionZoneChange(eLevelName, eLevelName);
	void Initialise();
	void PrintCarChanges();
	void Process();
	void ProcessAttachedEntities();
	void ProcessForAnimViewer();
	void ProcessLineOfSight(CVector const&, CVector const&, CColPoint&, CEntity*&, bool, bool, bool, bool, bool, bool, bool, bool);
	void ProcessLineOfSightSector(CSector&, CRepeatSector&, CColLine const&, CColPoint&, float&, CEntity*&, bool, bool, bool, bool, bool, bool, bool, bool);
	void ProcessLineOfSightSectorList(CPtrList&, CColLine const&, CColPoint&, float&, CEntity*&, bool, bool, bool);
	void ProcessPedsAfterPreRender();
	void ProcessVerticalLine(CVector const&, float, CColPoint&, CEntity*&, bool, bool, bool, bool, bool, bool, CStoredCollPoly*);
	void ProcessVerticalLineSector(CSector&, CRepeatSector&, CColLine const&, CColPoint&, CEntity*&, bool, bool, bool, bool, bool, bool, CStoredCollPoly*);
	void ProcessVerticalLineSectorList(CPtrList&, CColLine const&, CColPoint&, float&, CEntity*&, bool, CStoredCollPoly*);
	void ProcessVerticalLineSectorList_FillGlobeColPoints(CPtrList&, CColLine const&, CEntity*&, bool, CStoredCollPoly*);
	void ProcessVerticalLineSector_FillGlobeColPoints(CSector&, CRepeatSector&, CColLine const&, CEntity*&, bool, bool, bool, bool, bool, bool, CStoredCollPoly*);
	void ProcessVerticalLine_FillGlobeColPoints(CVector const&, float, CEntity*&, bool, bool, bool, bool, bool, bool, CStoredCollPoly*);
	void Remove(CEntity *pEntity);
	void RemoveEntityInsteadOfProcessingIt(CEntity*);
	void RemoveFallenCars();
	void RemoveFallenPeds();
	void RemoveReferencesToDeletedObject(CEntity*);
	void RemoveStaticObjects();
	void RepositionCertainDynamicObjects();
	void RepositionOneObject(CEntity*);
	void ResetLineTestOptions();
	void SetAllCarsCanBeDamaged(bool);
	void SetCarsOnFire(float, float, float, float, CEntity*);
	void SetPedsChoking(float, float, float, float, CEntity*);
	void SetPedsOnFire(float, float, float, float, CEntity*);
	void SetWorldOnFire(float, float, float, float, CEntity*);
	void ShutDown();
	void SprayPaintWorld(CVector&, CVector&, float, bool);
	void StopAllLawEnforcersInTheirTracks();
	void TestForBuildingsOnTopOfEachOther();
	void TestForBuildingsOnTopOfEachOther(CPtrList&);
	void TestForUnusedModels();
	void TestForUnusedModels(CPtrList&, int*);
	void TestSphereAgainstSectorList(CPtrList&, CVector, float, CEntity*, bool);
	void TestSphereAgainstWorld(CVector, float, CEntity*, bool, bool, bool, bool, bool, bool);
	void TriggerExplosion(CVector const&, float, float, CEntity*, CEntity*, bool, float);
	void TriggerExplosionSectorList(CPtrList&, CVector const&, float, float, CEntity*, CEntity*, bool, float);
	void UseDetonator(CEntity*);
private:
	static bool PlayerInFocus;
	//static CPlayerPed Players[2];
	static float SnookerTableMax;
	static float SnookerTableMin;
	static bool bDoingCarCollisions;
	static bool bForceProcessControl;
	static bool bIncludeBikers;
	static bool bIncludeCarTyres;
	static bool bIncludeDeadPeds;
	static bool bNoMoreCollisionTorque;
	static bool bProcessCutsceneOnly;
	static bool bSecondShift;
	static float fWeaponSpreadRate;
	//* m_aTempColPts
	//* ms_aLodPtrLists
	//* ms_aRepeatSectors
	//* ms_aSectors
	//* ms_iProcessLineNumCrossings
	//* ms_listMovingEntityPtrs
	//* ms_listObjectsWithControlCode
	size_t ms_nCurrentScanCode;
	static CEntity *pIgnoreEntity;
};