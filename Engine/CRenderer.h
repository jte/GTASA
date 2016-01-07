#pragma once

class CEntity;

struct LodRenderList_t
{
    CEntity* entity;
    float distance;
}

class CRenderer
{
public:
	/* AddEntityToRenderList(CEntity*, float)
	* AddToLodDontRenderList(CEntity*, float)
	* AddToLodRenderList(CEntity*, float)
	* ConstructReflectionList()
	* ConstructRenderList()
	* GetLodDontRenderListBase()
	*/ 
    static LodRenderList_t* GetLodRenderListBase();
	/* GetObjectsInFrustum(CEntity**, float, RwMatrixTag*)*/
	static void Init();
	/* PreRender()
	*/ 
    static void ProcessLodRenderLists();
	/* RemoveVehiclePedLights(CPhysical*, bool)
	* RenderEverythingBarRoads()*/
	static void RenderFadingInEntities();
	/* RenderFadingInUnderwaterEntities()
	* RenderFirstPersonVehicle()
	
    */
    static void RenderOneNonRoad(CEntity* entity);
	//* RenderOneRoad(CEntity*)
	static void RenderRoads();
    /*
	* RequestObjectsInDirection(CVector const&, float, int)
	* RequestObjectsInFrustum(RwMatrixTag*, int)
	* ResetLodRenderLists()
	* ScanBigBuildingList(int, int)
	* ScanPtrList_RequestModels(CPtrList&)
	* ScanSectorList(int, int)
	* ScanSectorList_ListModels(int, int)
	* ScanSectorList_ListModelsVisible(int, int)
	* ScanSectorList_RequestModels(int, int)
	* ScanWorld()
	* SetupBigBuildingVisibility(CEntity*, float&)
	* SetupEntityVisibility(CEntity*, float&)
	* SetupMapEntityVisibility(CEntity*, CBaseModelInfo*, float, bool)
	* ShouldModelBeStreamed(CEntity*, CVector const&, float)
    */
	static void Shutdown();
private:
	size_t m_loadingPriority;
	bool m_pFirstPersonVehicle;
	static CEntity** ms_aInVisibleEntityPtrs;
	static CEntity** ms_aVisibleEntityPtrs;
	static CEntity** ms_aVisibleLodPtrs;
	static CEntity** ms_aVisibleSuperLodPtrs;
	static bool ms_bInTheSky;
	static bool ms_bRenderOutsideTunnels;
	static bool ms_bRenderTunnels;
	static float ms_fCameraHeading;
	static float ms_fFarClipPlane;
	static size_t ms_lodDistScale;
	static size_t ms_lowLodDistScale;
	static size_t ms_nNoOfInVisibleEntities;
	static size_t ms_nNoOfVisibleEntities;
	static size_t ms_nNoOfVisibleLods;
	static size_t ms_nNoOfVisibleSuperLods;
	static LodRenderList_t* ms_pLodDontRenderList;
	static LodRenderList_t* ms_pLodRenderList;
	static CVector ms_vecCameraPosition;
};