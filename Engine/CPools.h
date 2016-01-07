#pragma once

#include "CTask.h"
#include "CPool.h"

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
	static void Initialise();
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

    //
    static CPool<CTask>* GetTaskPool();
    static CPool<CEvent>* GetEventPool();
    static CPool<CPtrNodeSingleLink>* GetPtrNodeSingleLinkPool();
	static CPool<CColModel>* GetColModelPool();
private:
	static CPool<class CBuilding> *ms_pBuildingPool;
	static CPool<class CColModel> *ms_pColModelPool;
	static CPool<class CDummy> *ms_pDummyPool;
	static CPool<class CEntryInfoNode> *ms_pEntryInfoNodePool;
	static CPool<class CEvent> *ms_pEventPool;
	static CPool<class CNodeRoute> *ms_pNodeRoutePool;
	static CPool<class CObject> *ms_pObjectPool;
	static CPool<class CPatrolRoute> *ms_pPatrolRoutePool;
	static CPool<class CPedAttractor> *ms_pPedAttractorPool;
	static CPool<class CPedIntelligence> *ms_pPedIntelligencePool;
	static CPool<class CPed> *ms_pPedPool;
	static CPool<class CPointRoute> *ms_pPointRoutePool;
	static CPool<class CPtrNodeDoubleLink> *ms_pPtrNodeDoubleLinkPool;
	static CPool<class CPtrNodeSingleLink> *ms_pPtrNodeSingleLinkPool;
	static CPool<class CTaskAllocator> *ms_pTaskAllocatorPool;
	static CPool<class CTask>* ms_pTaskPool;
	static CPool<class CVehicle> *ms_pVehiclePool;
};