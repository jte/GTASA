class CStreaming
{
public:
	CLinkList *AddEntity(CEntity *pEntity);
	* AddImageToList(char const*, bool)
	* AddLodsToRequestList(CVector const&, unsigned int)
	* AddModelsToRequestList(CVector const&, unsigned int)
	* AddToLoadedVehiclesList(int)
	* AreAnimsUsedByRequestedModels(int)
	* CarIsCandateForRemoval(int)
	* ClearFlagForAll(unsigned int)
	* ClearSlots(int)
	* ConvertBufferToObject(char*, int, int)
	* DeleteAllRwObjects()
	* DeleteFarAwayRwObjects(CVector const&)
	* DeleteLeastUsedEntityRwObject(bool, unsigned int)
	* DeleteRwObjectsAfterDeath(CVector const&)
	* DeleteRwObjectsBehindCamera(int)
	* DeleteRwObjectsBehindCameraInSectorList(CPtrList&, int)
	* DeleteRwObjectsInSectorList(CPtrList&, int, int)
	* DeleteRwObjectsNotInFrustumInSectorList(CPtrList&, int)
	* DisableCopBikes(bool)
	* FindMIPedSlotForInterior(int)
	* FinishLoadingLargeFile(char*, int)
	* FlushChannels()
	* FlushRequestList()
	* ForceLayerToRead(int)
	* GetCdImageOffset(int)
	* GetDefaultAmbulanceModel()
	* GetDefaultCabDriverModel()
	* GetDefaultCopCarModel(unsigned int)
	* GetDefaultCopModel()
	* GetDefaultFireEngineModel()
	* GetDefaultFiremanModel()
	* GetDefaultMedicModel()
	* GetDiscInDrive()
	* GetGta3ImageIndex()
	* GetModelCDName(int)
	* GetNextFileOnCd(int, bool)
	* HasSpecialCharLoaded(int)
	* HasVehicleUpgradeLoaded(int)
	* IHaveUsedStreamingMemory()
	* ImGonnaUseStreamingMemory()
	* Init()
	* Init2()
	* InitImageList()
	* InstanceLoadedModels(CVector const&)
	* InstanceLoadedModelsInSectorList(CPtrList&)
	* IsCarModelNeededInCurrentZone(int)
	* IsInitialised()
	* IsObjectInCdImage(int)
	* IsVeryBusy()
	* Load()
	* LoadAllRequestedModels(bool)
	* LoadCdDirectory()
	* LoadCdDirectory(char const*, int)
	* LoadInitialPeds()
	* LoadInitialVehicles()
	* LoadInitialWeapons()
	* LoadRequestedModels()
	* LoadScene(CVector const&)
	* LoadSceneCollision(CVector const&)
	* LoadZoneVehicle(CVector const&)
	* MakeSpaceFor(int)
	* PlayerIsFlyingFast()
	* PossiblyStreamCarOutAfterCreation(int)
	* ProcessEntitiesInSectorList(CPtrList&, float, float, float, float, float, float, float, unsigned int)
	* ProcessEntitiesInSectorList(CPtrList&, unsigned int)
	* ProcessLoadingChannel(int)
	* PurgeRequestList()
	* ReInit()
	* ReadIniFile()
	* ReclassifyLoadedCars()
	* RemoveAllUnusedModels()
	* RemoveBigBuildings()
	* RemoveBuildingsNotInArea(int)
	* RemoveCarModel(int)
	* RemoveCurrentZonesModels()
	* RemoveDodgyPedsFromRandomSlots()
	* RemoveEntity(CLink<CEntity*>*)
	* RemoveInappropriatePedModels()
	* RemoveLeastUsedModel(unsigned int)
	* RemoveLoadedVehicle()
	* RemoveLoadedZoneModel()
	* RemoveModel(int)
	* RemoveModelsNotVisibleFromCullzone()
	* RemoveModelsNotVisibleFromCullzoneInSectorList(CPtrList&)
	* RemoveUnusedModelsInLoadedList()
	* RenderEntity(CLink<CEntity*>*)
	* RequestAllModels()
	* RequestBigBuildings(CVector const&)
	* RequestFile(int, int, int, int, int)
	* RequestFilesInChannel(int)
	* RequestModel(int, int);
	* RequestModelStream(int)
	* RequestPlayerSection(int, char const*, int)
	* RequestSpecialChar(int, char const*, int)
	* RequestSpecialModel(int, char const*, int)
	* RequestVehicleUpgrade(int, int)
	* RetryLoadFile(int)
	* Save()
	* SetLoadVehiclesInLoadScene(bool)
	* SetMissionDoesntRequireModel(int)
	* SetMissionDoesntRequireSpecialChar(int)
	* SetModelIsDeletable(int)
	* SetModelTxdIsDeletable(int)
	* SetSpecialCharIsDeletable(int)
	* Shutdown()
	* StartRenderEntities()
	* StreamAmbulanceAndMedic(bool)
	* StreamCopModels(int)
	* StreamFireEngineAndFireman(bool)
	* StreamOneNewCar()
	* StreamPedsForInterior(int)
	* StreamPedsIntoRandomSlots(int*)
	* StreamVehiclesAndPeds()
	* StreamVehiclesAndPeds_Always(CVector const&)
	* StreamZoneModels(CVector const&)
	* StreamZoneModels_Gangs(CVector const&)
	* Update()
	* UpdateForAnimViewer()
	* UpdateMemoryUsed()
	* WeAreTryingToPhaseVehicleOut(int)
private:
	* desiredNumVehiclesLoaded
	* m_bBoatsNeeded
	* m_bHarvesterModelsRequested
	* m_bStreamHarvesterModelsThisFrame
	* ms_NextPedToLoadFromGroup
	* ms_aInfoForModel
	* ms_bEnableRequestListPurge
	* ms_bIsInitialised
	* ms_bLoadingBigModel
	* ms_channel
	* ms_channelError
	* ms_currentZoneType
	* ms_disableStreaming
	* ms_files
	* ms_imageOffsets
	* ms_imageSize
	* ms_imageSizes
	* ms_interiorImageOffsets
	* ms_lastCullZone
	* ms_lastImageRead
	* ms_loadedGangCars
	* ms_loadedGangs
	* ms_memoryAvailable
	* ms_memoryUsed
	* ms_numModelsRequested
	* ms_numPedsLoaded
	* ms_numPriorityRequests
	* ms_oldSectorX
	* ms_oldSectorY
	* ms_pEndLoadedList
	* ms_pEndRequestedList
	* ms_pExtraObjectsDir
	* ms_pStartLoadedList
	* ms_pStartRequestedList
	* ms_pStreamingBuffer
	* ms_pedsLoaded
	CLinkList<CEntity*> ms_rwObjectInstances;
	* ms_streamingBufferSize
	* ms_vehiclesLoaded
};