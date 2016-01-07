#include "StdInc.h"

void CPools::Initialise()
{
    ms_pPtrNodeSingleLinkPool = new CPool<CPtrNodeSingleLink>[70000];
    ms_pPtrNodeDoubleLinkPool = new CPool<CPtrNodeDoubleLink>[3200];
    ms_pEntryInfoNodePool = new CPool<CEntryInfoNode>[500];
    ms_pPedPool = new CPool<CPed>[140];
    ms_pVehiclePool = new CPool<CVehicle>[110];
    ms_pBuildingPool = new CPool<CBuilding>[13000];
    ms_pObjectPool = new CPool<CObject>[350];
    ms_pDummyPool = new CPool<CDummy>[2500];
    ms_pColModelPool = new CPool<CColModel>[10150];
    ms_pTaskPool = new CPool<CTask>[500];
    ms_pEventPool = new CPool<CEvent>[200];
    ms_pPointRoutePool = new CPool<CPointRoute>[64];
    ms_pPatrolRoutePool = new CPool<CPatrolRoute>[32];
    ms_pNodeRoutePool = new CPool<CNodeRoute>[64];
    ms_pTaskAllocatorPool = new CPool<CTaskAllocator>[16];
    ms_pPedIntelligencePool = new CPool<CPedIntelligence>[140];
    ms_pPedAttractorPool = new CPool<CPedAttractor>[64];
}

CPool<CTask> CPools::GetTaskPool()
{
    return ms_pTaskPool;
}