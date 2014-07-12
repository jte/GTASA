#pragma once

class CPools
{
public:
	//* CheckBuildingAtomics()
	//* CheckPoolsEmpty()
	//* GetObject(int)
	//* GetObjectRef(CObject*)
	//* GetPed(int)
	//* GetPedRef(CPed*)
	//* GetVehicle(int)
	//* GetVehicleRef(CVehicle*)
	void Initialise();
	//* Load()
	//* LoadObjectPool()
	//* LoadPedPool()
	//* LoadPedTasks()
	//* LoadVehiclePool()
	//* MakeSureSlotInObjectPoolIsEmpty(int)
	//* Save()
	//* SaveObjectPool()
	//* SavePedPool()
	//* SavePedTasks()
	//* SaveVehiclePool()
	//* ShutDown()
private:
	CPool<class CBuilding> *ms_pBuildingPool;
	CPool<class CColModel> *ms_pColModelPool;
	CPool<class CDummy> *ms_pDummyPool;
	CPool<class CEntryInfoNode> *ms_pEntryInfoNodePool;
	CPool<class CEvent> *ms_pEventPool;
	CPool<class CNodeRoute> *ms_pNodeRoutePool;
	CPool<class CObject> *ms_pObjectPool;
	CPool<class CPatrolRoute> *ms_pPatrolRoutePool;
	CPool<class CPedAttractor> *ms_pPedAttractorPool;
	CPool<class CPedIntelligence> *ms_pPedIntelligencePool;
	CPool<class CPed> *ms_pPedPool;
	CPool<class CPointRoute> *ms_pPointRoutePool;
	CPool<class CPtrNodeDoubleLink> *ms_pPtrNodeDoubleLinkPool;
	CPool<class CPtrNodeSingleLink> *ms_pPtrNodeSingleLinkPool;
	CPool<class CTaskAllocator> *ms_pTaskAllocatorPool;
	CPool<class CTask> *ms_pTaskPool;
	CPool<class CVehicle> *ms_pVehiclePool;
};