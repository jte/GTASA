#pragma once

class CEntity;
class CBaseModelInfo;

typedef void (*RenderAlphaCallback)(void*, float);
struct AlphaObjectInfo
{
    union 
    {
        RpAtomic *atomic;
        CEntity *entity;
        void *object;
    };
    RenderAlphaCallback renderCB;
    float alpha;
};

template<>
class CLinkList<AlphaObjectInfo>
{
public:
    bool InsertSorted(const AlphaObjectInfo& info);
    // common functionality
    void Init(int);
    void Clear();
    void Shutdown();
    void Insert(const AlphaObjectInfo*);
};

class CVisibilityPlugins
{
    typedef RpAtomic* (*AtomicRenderCallback)(RpAtomic*);
public:
	static void *AtomicConstructor(void *atomic, RwInt32 offset, RwInt32 size);
	static void *AtomicCopyConstructor(void *dstAtomic, const void *srcAtomic, RwInt32 offset, RwInt32 size);
	static void *AtomicDestructor(void *atomic, RwInt32 offset, RwInt32 size);
	static float CalculateFadingAtomicAlpha(CBaseModelInfo *pModelInfo, CEntity *pEntity, float fBias);
	static void ClearAtomicFlag(RpAtomic *pAtomic, unsigned short usFlag);
	static void ClearClumpForAllAtomicsFlag(RpClump *pClump, int flag);
	static void *ClumpConstructor(void *clump, RwInt32 offset, RwInt32 size);
	static void *ClumpCopyConstructor(void *dstClump, const void *srcClump, RwInt32 offset, RwInt32 size);
	static void *ClumpDestructor(void *clump, RwInt32 offset, RwInt32 size);
	static void DefaultVisibilityCB(void*, float);
	static void *FrameConstructor(void *frame, RwInt32 offset, RwInt32 size);
	static void *FrameCopyConstructor(void *dstFrame, const void *srcFrame, RwInt32 offset, RwInt32 size);
	static void *FrameDestructor(void *frame, RwInt32 offset, RwInt32 size);
	static bool FrustumSphereCB(RpClump *pClump);
	static int GetAtomicId(RpAtomic *pAtomic);
	static uint8_t GetClumpAlpha(RpClump* pClump);
	static CClumpModelInfo* GetClumpModelInfo(RpClump* pClump);
	static float GetDistanceSquaredFromCamera(RwFrame *pFrame);
	static float GetDistanceSquaredFromCamera(RwV3d *pVec);
	static float GetDotProductWithCameraVector(RwMatrixTag *pMat1, RwMatrixTag *pMat2, unsigned int flags);
	static int GetFrameHierarchyId(RwFrame *pFrame);
	static CBaseModelInfo *GetModelInfo(RpAtomic* pAtomic);
	static int GetModelInfoIndex(RpAtomic* pAtomic);
	static int GetUserValue(RpAtomic *pAtomic);
	static void InitAlphaAtomicList();
	static void InitAlphaEntityList();
	//* Initialise();
	static bool InsertAtomicIntoBoatSortedList(RpAtomic *pAtomic, float fAlpha);
	static bool InsertAtomicIntoReallyDrawLastList(RpAtomic *pAtomic, float fAlpha);
	static bool InsertAtomicIntoSortedList(RpAtomic *pAtomic, float fAlpha);
	static bool InsertEntityIntoReallyDrawLastList(CEntity *pEntity, float fAlpha);
	static bool InsertEntityIntoSortedList(CEntity *pEntity, float fAlpha);
	static bool InsertEntityIntoUnderwaterList(CEntity *pEntity, float fAlpha);
	static bool InsertObjectIntoSortedList(void *object, float alpha, RenderAlphaCallback callback);
	//* IsAtomicVisible(RpAtomic *pAtomic);
	//* IsClumpVisible(RpClump*)
	static bool PluginAttach();
	static void RenderAlphaAtomics();
	static void RenderAtomic(void *atomic, float);
	static void RenderAtomicWithAlpha(RpAtomic *pAtomic, int alpha);
	static RpAtomic* RenderAtomicWithAlphaCB(RpAtomic *pAtomic, void *object);
	static void RenderBoatAlphaAtomics();
	static void RenderEntity(void *entity, float alpha);
	static void RenderFadingAtomic(CBaseModelInfo *pModelInfo, RpAtomic *pAtomic, int bias);
	static void RenderFadingClump(CBaseModelInfo *pModelInfo, RpClump *pClump, int bias);
	static RpAtomic* RenderFadingClumpCB(RpAtomic *pAtomic);
	static void RenderFadingEntities();
	static void RenderFadingUnderwaterEntities();
	static RpAtomic* RenderHeliRotorAlphaCB(RpAtomic* pAtomic);
	static RpAtomic* RenderHeliTailRotorAlphaCB(RpAtomic *pAtomic);
	//* RenderObjNormalAtomic(RpAtomic*);
	static void RenderOrderedList(CLinkList<AlphaObjectInfo>& list);
	//* RenderPedCB(RpAtomic*)
	//* RenderPlayerCB(RpAtomic*)
	static void RenderReallyDrawLastObjects();
	RpAtomic* RenderTrainHiDetailAlphaCB(RpAtomic* atomic);
	RpAtomic* RenderTrainHiDetailCB(RpAtomic* atomic);
	//* RenderVehicleHiDetailAlphaCB(RpAtomic*)
	//* RenderVehicleHiDetailAlphaCB_BigVehicle(RpAtomic*)
	//* RenderVehicleHiDetailAlphaCB_Boat(RpAtomic*)
	RpAtomic* RenderVehicleHiDetailCB(RpAtomic* atomic);
	//* RenderVehicleHiDetailCB_BigVehicle(RpAtomic*)
	//* RenderVehicleHiDetailCB_Boat(RpAtomic*)
	//* RenderVehicleLoDetailCB_Boat(RpAtomic*)
	RpAtomic* RenderVehicleReallyLowDetailCB(RpAtomic* atomic);
	RpAtomic* RenderVehicleReallyLowDetailCB_BigVehicle(RpAtomic* atomic);
	//* RenderWeaponCB(RpAtomic*)
	//* RenderWeaponPedsForPC()
	//* RenderWheelAtomicCB(RpAtomic*)
	//* ResetRenderFadingAtomic(CBaseModelInfo*)
	//* SetAtomicFlag(RpAtomic*, unsigned short)
	//* SetAtomicId(RpAtomic*, int)
	void SetAtomicRenderCallback(RpAtomic* atomic, AtomicRenderCallback callback);
	//* SetClumpAlpha(RpClump*, int)
	//* SetClumpForAllAtomicsFlag(RpClump*, int)
	//* SetClumpModelInfo(RpClump*, CClumpModelInfo*)
	//* SetFrameHierarchyId(RwFrame*, int)
	//* SetModelInfoIndex(RpAtomic*, int)
	//* SetRenderWareCamera(RwCamera*)
	//* SetUserValue(RpAtomic*, unsigned short)
	//* SetupRenderFadingAtomic(CBaseModelInfo*, int)
	void SetupVehicleVariables(RpClump* clump);
	//* Shutdown()
	//* VehicleVisibilityCB(RpClump*)
	//* VehicleVisibilityCB_BigVehicle(RpClump*)

    // 
    static bool CanRenderAtomic(RpAtomic* atomic, float& dot);
protected:
private:
	static CLinkList<AlphaObjectInfo> m_alphaBoatAtomicList;
	static CLinkList<AlphaObjectInfo> m_alphaEntityList;
	static CLinkList<AlphaObjectInfo> m_alphaList;
	static CLinkList<AlphaObjectInfo> m_alphaReallyDrawLastList;
	static CLinkList<AlphaObjectInfo> m_alphaUnderwaterEntityList;
	static RwInt32 ms_atomicPluginOffset;
	static size_t ms_bigVehicleLod0Dist;
	static RwInt32 ms_clumpPluginOffset;
	static size_t ms_cullBigCompsDist;
	static size_t ms_cullCompsDist;
	//* ms_defaultRenderer
	static RwInt32 ms_framePluginOffset;
	static RwCamera* ms_pCamera;
	static CVector* ms_pCameraPosn;
	static size_t ms_pedFadeDist;
	static size_t ms_pedLodDist;
	static size_t ms_vehicleLod0Dist;
	static size_t ms_vehicleLod0RenderMultiPassDist;
	static size_t ms_vehicleLod1Dist;
	//* ms_weaponPedsForPC

};