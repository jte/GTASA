#pragma once
/*
00000000 CImgFile        struc ; (sizeof=0x10)   ; XREF: .data:_playerImgr
00000000 headers         dd ?                    ; offset
00000004 size            dd ?
00000008 count           dd ?
0000000C dynamicAllocated db ?
0000000D __pad           db 3 dup(?)
00000010 CImgFile        ends
*/
struct CImgDescriptor
{
	char name[40];
	bool isNotPlayerImg;
	uint8_t pad[3];
	uint32_t streamHandle;
};

class CStreaming
{
public:
	static CLinkList<CEntity*>* AddEntity(CEntity *pEntity);
	static uint32_t AddImageToList(char const* name, bool notPlayerImage);
	//* AddLodsToRequestList(CVector const&, unsigned int)
	//* AddModelsToRequestList(CVector const&, unsigned int)
	//* AddToLoadedVehiclesList(int)
	//* AreAnimsUsedByRequestedModels(int)
	//* CarIsCandateForRemoval(int)
	//* ClearFlagForAll(unsigned int)
	//* ClearSlots(int)
	//* ConvertBufferToObject(char*, int, int)
	//* DeleteAllRwObjects()
	//* DeleteFarAwayRwObjects(CVector const&)
	//* DeleteLeastUsedEntityRwObject(bool, unsigned int)
	//* DeleteRwObjectsAfterDeath(CVector const&)
	//* DeleteRwObjectsBehindCamera(int)
	//* DeleteRwObjectsBehindCameraInSectorList(CPtrList&, int)
	//* DeleteRwObjectsInSectorList(CPtrList&, int, int)
	//* DeleteRwObjectsNotInFrustumInSectorList(CPtrList&, int)
	static void DisableCopBikes(bool disable);
	//* FindMIPedSlotForInterior(int)
	static bool FinishLoadingLargeFile(char *a3, int index);
	//* FlushChannels()
	//* FlushRequestList()
	//* ForceLayerToRead(int)
	//* GetCdImageOffset(int)
	//* GetDefaultAmbulanceModel()
	//* GetDefaultCabDriverModel()
	//* GetDefaultCopCarModel(unsigned int)
	//* GetDefaultCopModel()
	//* GetDefaultFireEngineModel()
	//* GetDefaultFiremanModel()
	//* GetDefaultMedicModel()
	//* GetDiscInDrive()
	//* GetGta3ImageIndex()
	//* GetModelCDName(int)
	//* GetNextFileOnCd(int, bool)
	static bool HasSpecialCharLoaded(int id);
	//* HasVehicleUpgradeLoaded(int)
	//* IHaveUsedStreamingMemory()
	//* ImGonnaUseStreamingMemory()
	//* Init()
	//* Init2()
	static void InitImageList();
	//* InstanceLoadedModels(CVector const&)
	//* InstanceLoadedModelsInSectorList(CPtrList&)
	//* IsCarModelNeededInCurrentZone(int)
	//* IsInitialised()
	//* IsObjectInCdImage(int)
	//* IsVeryBusy()
	//* Load()
	//* LoadAllRequestedModels(bool)
	//* LoadCdDirectory()
	//* LoadCdDirectory(char const*, int)
	//* LoadInitialPeds()
	//* LoadInitialVehicles()
	//void LoadInitialWeapons();
	//* LoadRequestedModels()
	//* LoadScene(CVector const&)
	//* LoadSceneCollision(CVector const&)
	//* LoadZoneVehicle(CVector const&)
	static void MakeSpaceFor(int size);
	//* PlayerIsFlyingFast()
	//* PossiblyStreamCarOutAfterCreation(int)
	//* ProcessEntitiesInSectorList(CPtrList&, float, float, float, float, float, float, float, unsigned int)
	//* ProcessEntitiesInSectorList(CPtrList&, unsigned int)
	//* ProcessLoadingChannel(int)
	//* PurgeRequestList()
	//* ReInit()
	//* ReadIniFile()
	//* ReclassifyLoadedCars()
	//* RemoveAllUnusedModels()
	static void RemoveBigBuildings();
	//* RemoveBuildingsNotInArea(int)
	//* RemoveCarModel(int)
	//* RemoveCurrentZonesModels()
	//* RemoveDodgyPedsFromRandomSlots()
	//* RemoveEntity(CLink<CEntity*>*)
	//* RemoveInappropriatePedModels()
	//* RemoveLeastUsedModel(unsigned int)
	//* RemoveLoadedVehicle()
	//* RemoveLoadedZoneModel()
	//* RemoveModel(int)
	//* RemoveModelsNotVisibleFromCullzone()
	//* RemoveModelsNotVisibleFromCullzoneInSectorList(CPtrList&)
	//* RemoveUnusedModelsInLoadedList()
	//* RenderEntity(CLink<CEntity*>*)
	//* RequestAllModels()
	//* RequestBigBuildings(CVector const&)
	//* RequestFile(int, int, int, int, int)
	//* RequestFilesInChannel(int)
    static void RequestModel(uint32_t modelId, uint32_t flags);
	//* RequestModelStream(int)
	//* RequestPlayerSection(int, char const*, int)
	//* RequestSpecialChar(int, char const*, int)
	//* RequestSpecialModel(int, char const*, int)
	//* RequestVehicleUpgrade(int, int)
	//* RetryLoadFile(int)
	//* Save()
	static void SetLoadVehiclesInLoadScene(bool loadVehiclesInLoadScene);
	static void SetMissionDoesntRequireModel(int id);
	//* SetMissionDoesntRequireSpecialChar(int)
	static void SetModelIsDeletable(int id);
	static void SetModelTxdIsDeletable(int id);
	//* SetSpecialCharIsDeletable(int)
	//* Shutdown()
	//* StartRenderEntities()
	//* StreamAmbulanceAndMedic(bool)
	//* StreamCopModels(int)
	//* StreamFireEngineAndFireman(bool)
	//* StreamOneNewCar()
	//* StreamPedsForInterior(int)
	//* StreamPedsIntoRandomSlots(int*)
	//* StreamVehiclesAndPeds()
	//* StreamVehiclesAndPeds_Always(CVector const&)
	//* StreamZoneModels(CVector const&)
	//* StreamZoneModels_Gangs(CVector const&)
	//* Update()
	//* UpdateForAnimViewer()
	//* UpdateMemoryUsed()
	//* WeAreTryingToPhaseVehicleOut(int)
private:
	//* desiredNumVehiclesLoaded
	//* m_bBoatsNeeded
	//* m_bHarvesterModelsRequested
	//* m_bStreamHarvesterModelsThisFrame
	//* ms_NextPedToLoadFromGroup
	static CStreamingInfo ms_aInfoForModel[26316];
	//* ms_bEnableRequestListPurge
	//* ms_bIsInitialised
	//* ms_bLoadingBigModel
	//* ms_channel
	//* ms_channelError
	//* ms_currentZoneType
	//* ms_disableStreaming
	static CImgDescriptor ms_files[8];
	//* ms_imageOffsets
	//* ms_imageSize
	//* ms_imageSizes
	//* ms_interiorImageOffsets
	//* ms_lastCullZone
	//* ms_lastImageRead
	//* ms_loadedGangCars
	//* ms_loadedGangs
	static size_t ms_memoryAvailable;
	static size_t ms_memoryUsed;
	static size_t ms_numModelsRequested;
	//* ms_numPedsLoaded
	static size_t ms_numPriorityRequests;
	//* ms_oldSectorX
	//* ms_oldSectorY
	//* ms_pEndLoadedList
	//* ms_pEndRequestedList
	//* ms_pExtraObjectsDir
	static CStreamingInfo* ms_pStartLoadedList;
	//* ms_pStartRequestedList;
	//* ms_pStreamingBuffer
	//* ms_pedsLoaded
	static CLinkList<CEntity*> ms_rwObjectInstances;
	//* ms_streamingBufferSize
	//* ms_vehiclesLoaded
};