#pragma once

class CPtrNodeDoubleLink;

CPed* FindPlayerPed(int number);

class CWorld
{
public:
    /*
	* Add(CEntity*)
	* CallOffChaseForArea(float, float, float, float)
	* CallOffChaseForAreaSectorListPeds(CPtrList&, float, float, float, float, float, float, float, float)
	* CallOffChaseForAreaSectorListVehicles(CPtrList&, float, float, float, float, float, float, float, float)
	* CameraToIgnoreThisObject(CEntity*)
	* CastShadow(float, float, float, float)
	* CastShadowSectorList(CPtrList&, float, float, float, float)
	* CheckBlockListIntegrity()
	* ClearCarsFromArea(float, float, float, float, float, float)
	* ClearExcitingStuffFromArea(CVector const&, float, unsigned char)
	* ClearForRestart()
	* ClearPedsFromArea(float, float, float, float, float, float)
	* 
	* ClearScanCodes();
	* DoZoneTestForChaser(CPhysical*)
	* ExtinguishAllCarFiresInArea(CVector, float)
	* FindGroundZFor3DCoord(float, float, float, bool*, CEntity**)
	* FindGroundZForCoord(float, float)
	* FindLodOfTypeInRange(unsigned int, CVector const&, float, bool, short*, short, CEntity**)
	* FindLowestZForCoord(float, float)
	* FindMissionEntitiesIntersectingCube(CVector const&, CVector const&, short*, short, CEntity**, bool, bool, bool)
	* FindMissionEntitiesIntersectingCubeSectorList(CPtrList&, CVector const&, CVector const&, short*, short, CEntity**, bool, bool, bool)
	* FindNearestObjectOfType(int, CVector const&, float, bool, bool, bool, bool, bool, bool)
	* FindNearestObjectOfTypeSectorList(int, CPtrList&, CVector const&, float, bool, CEntity**, float*)
	*/
	static void FindObjectsInRange(const CVector& vecCenter, float fRadius, bool bIgnoreZ, short *usFoundCount, short usMaxCount, CEntity** pFoundObjectsList, bool bIncludeBuildings, bool bIncludeVehicles, bool bIncludePeds, bool bIncludeObjects, bool bIncludeDummies);
	/* FindObjectsInRangeSectorList(CPtrList&, CVector const&, float, bool, short*, short, CEntity**)
	* FindObjectsIntersectingAngledCollisionBox(CBox const&, CMatrix const&, CVector const&, float, float, float, float, short*, short, CEntity**, bool, bool, bool, bool, bool)
	* FindObjectsIntersectingAngledCollisionBoxSectorList(CPtrList&, CBox const&, CMatrix const&, CVector const&, short*, short, CEntity**)
	* FindObjectsIntersectingCube(CVector const&, CVector const&, short*, short, CEntity**, bool, bool, bool, bool, bool)
	* FindObjectsIntersectingCubeSectorList(CPtrList&, CVector const&, CVector const&, short*, short, CEntity**)
	* FindObjectsKindaColliding(CVector const&, float, bool, short*, short, CEntity**, bool, bool, bool, bool, bool)
	* FindObjectsKindaCollidingSectorList(CPtrList&, CVector const&, float, bool, short*, short, CEntity**)
	* FindObjectsOfTypeInRange(unsigned int, CVector const&, float, bool, short*, short, CEntity**, bool, bool, bool, bool, bool)
	* FindObjectsOfTypeInRangeSectorList(unsigned int, CPtrList&, CVector const&, float, bool, short*, short, CEntity**)
	* FindPlayerSlotWithPedPointer(void*)
	* FindPlayerSlotWithRemoteVehiclePointer(void*)
	* FindPlayerSlotWithVehiclePointer(CEntity*)
	* FindRoofZFor3DCoord(float, float, float, bool*)
	* FindUnsuspectingTargetCar(CVector, CVector)
	* FindUnsuspectingTargetPed(CVector, CVector)
	* GetIsLineOfSightClear(CVector const&, CVector const&, bool, bool, bool, bool, bool, bool, bool)
	* GetIsLineOfSightSectorClear(CSector&, CRepeatSector&, CColLine const&, bool, bool, bool, bool, bool, bool, bool)
	* GetIsLineOfSightSectorListClear(CPtrList&, CColLine const&, bool, bool)
	* HandleCollisionZoneChange(eLevelName, eLevelName)
	* Initialise()
	* PrintCarChanges()
	* Process()
	* ProcessAttachedEntities()
	* ProcessForAnimViewer()
	* ProcessLineOfSight(CVector const&, CVector const&, CColPoint&, CEntity*&, bool, bool, bool, bool, bool, bool, bool, bool)
	* ProcessLineOfSightSector(CSector&, CRepeatSector&, CColLine const&, CColPoint&, float&, CEntity*&, bool, bool, bool, bool, bool, bool, bool, bool)
	* ProcessLineOfSightSectorList(CPtrList&, CColLine const&, CColPoint&, float&, CEntity*&, bool, bool, bool)
	* ProcessPedsAfterPreRender()
	* ProcessVerticalLine(CVector const&, float, CColPoint&, CEntity*&, bool, bool, bool, bool, bool, bool, CStoredCollPoly*)
	* ProcessVerticalLineSector(CSector&, CRepeatSector&, CColLine const&, CColPoint&, CEntity*&, bool, bool, bool, bool, bool, bool, CStoredCollPoly*)
	* ProcessVerticalLineSectorList(CPtrList&, CColLine const&, CColPoint&, float&, CEntity*&, bool, CStoredCollPoly*)
	* ProcessVerticalLineSectorList_FillGlobeColPoints(CPtrList&, CColLine const&, CEntity*&, bool, CStoredCollPoly*)
	* ProcessVerticalLineSector_FillGlobeColPoints(CSector&, CRepeatSector&, CColLine const&, CEntity*&, bool, bool, bool, bool, bool, bool, CStoredCollPoly*)
	* ProcessVerticalLine_FillGlobeColPoints(CVector const&, float, CEntity*&, bool, bool, bool, bool, bool, bool, CStoredCollPoly*)
	* Remove(CEntity*)
	* RemoveEntityInsteadOfProcessingIt(CEntity*)
	* RemoveFallenCars()
	* RemoveFallenPeds()
	* RemoveReferencesToDeletedObject(CEntity*)
	* RemoveStaticObjects()
	* RepositionCertainDynamicObjects()
	* RepositionOneObject(CEntity*)
	* ResetLineTestOptions()
	* SetAllCarsCanBeDamaged(bool)
	* SetCarsOnFire(float, float, float, float, CEntity*)
	* SetPedsChoking(float, float, float, float, CEntity*)
	* SetPedsOnFire(float, float, float, float, CEntity*)
	* SetWorldOnFire(float, float, float, float, CEntity*)
	* ShutDown()
	* SprayPaintWorld(CVector&, CVector&, float, bool)
	* StopAllLawEnforcersInTheirTracks()
	* TestForBuildingsOnTopOfEachOther()
	* TestForBuildingsOnTopOfEachOther(CPtrList&)
	* TestForUnusedModels()
	* TestForUnusedModels(CPtrList&, int*)
	* TestSphereAgainstSectorList(CPtrList&, CVector, float, CEntity*, bool)
	* TestSphereAgainstWorld(CVector, float, CEntity*, bool, bool, bool, bool, bool, bool)
	* TriggerExplosion(CVector const&, float, float, CEntity*, CEntity*, bool, float)
	* TriggerExplosionSectorList(CPtrList&, CVector const&, float, float, CEntity*, CEntity*, bool, float)
	* UseDetonator(CEntity*)
    */
//private:
	static bool PlayerInFocus;
	static CPlayerPed Players[2];
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
	static CPtrNodeSingleLink/*CSector*/ ms_aSectors[14400];
	static CRepeatSector ms_aRepeatSectors[256];
	static CPtrNodeSingleLink ms_aLodPtrLists[450];

	//* m_aTempColPts
	//* ms_iProcessLineNumCrossings
	static CPtrNodeDoubleLink* ms_listMovingEntityPtrs;
	//* ms_listObjectsWithControlCode
	static size_t ms_nCurrentScanCode;
	static CEntity *pIgnoreEntity;
};