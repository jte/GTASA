#include "StdInc.h"

#define rwID_VPATOMIC (0x00)
#define rwID_VPCLUMP  (0x01)
#define rwID_VPFRAME  (0x02)

#define PLUGIN_ATOMIC(object, var) \
    (RWPLUGINOFFSET(AtomicPluginDef, object, ms_atomicPluginOffset)->var)
#define PLUGIN_CLUMP(object, var) \
    (RWPLUGINOFFSET(ClumpPluginDef, object, ms_clumpPluginOffset)->var)
#define PLUGIN_FRAME(object, var) \
    (RWPLUGINOFFSET(FramePluginDef, object, ms_framePluginOffset)->var)


RwInt32 CVisibilityPlugins::ms_atomicPluginOffset = -1;;
RwInt32 CVisibilityPlugins::ms_clumpPluginOffset = -1;
RwInt32 CVisibilityPlugins::ms_framePluginOffset = -1;
static float gVehicleDistanceFromCamera;
static float gVehicleAngleFromCamera;

struct AtomicPluginDef
{
    RwInt16 id; // model info id
    RwInt16 user_value;
};

struct FramePluginDef
{
    union 
    {
        RwInt32 hierid;
        CClumpModelInfo *pClumpModelInfo;
    };
};

struct ClumpPluginDef
{
    RenderAlphaCallback callback;
    RwInt32 alpha;
};

void *CVisibilityPlugins::AtomicConstructor(void *atomic, RwInt32 offset, RwInt32 size)
{
    if(ms_atomicPluginOffset > 0)
    {
        PLUGIN_ATOMIC(atomic, id) = -1;
        PLUGIN_ATOMIC(atomic, user_value) = 0;
    }
    return atomic;
}


void *CVisibilityPlugins::AtomicDestructor(void *atomic, RwInt32 offset, RwInt32 size)
{
    return atomic;
}


void *CVisibilityPlugins::AtomicCopyConstructor(void *dstAtomic, const void *srcAtomic, RwInt32 offset, RwInt32 size)
{
    PLUGIN_ATOMIC(dstAtomic, id) = PLUGIN_ATOMIC(srcAtomic, id);
    PLUGIN_ATOMIC(dstAtomic, user_value) = PLUGIN_ATOMIC(srcAtomic, user_value);
    return dstAtomic;
}

float CVisibilityPlugins::CalculateFadingAtomicAlpha(CBaseModelInfo* modelInfo, CEntity* entity, float currentAlpha)
{
    float scale = 20.0f;
    float maxPlaneRange = modelInfo->GetColModel()->GetBoundingBox().GetRadius() + CRenderer::ms_fFarClipPlane;
    float rangeCam = TheCamera.GenerationDistMultiplier * modelInfo->GetLoadDistanceUnscaled();
    if(rangeCam < maxPlaneRange)
    {
        maxPlaneRange = rangeCam;
    }
    if(!entity->GetLodEntity())
    {
        float distScale = modelInfo->GetLoadDistanceUnscaled();
        if(distScale >= maxPlaneRange)
        {
            distScale = maxPlaneRange;
        }
        if(distScale > 150.0f)
        {
            scale = distScale / 15.0f + 10.0f;
            if(entity->GetIsBigBuilding())
            {
                maxPlaneRange *= CRenderer::ms_lowLodDistScale;
            }
        }
    }
    float alphaMult = ClampMax((maxPlaneRange + 20.0f - currentAlpha) / scale, 1.0f);
    return modelInfo->GetAlpha() * alphaMult;
}

void CVisibilityPlugins::ClearAtomicFlag(RpAtomic *pAtomic, unsigned short usFlag)
{
    PLUGIN_ATOMIC(pAtomic, user_value) &= ~usFlag;
}

static RpAtomic *ClearClumpForAllAtomicsFlagCB(RpAtomic *atomic, void *data)
{
    CVisibilityPlugins::ClearAtomicFlag(atomic, *(unsigned short*)data);
}

void CVisibilityPlugins::ClearClumpForAllAtomicsFlag(RpClump* clump, int flag)
{
    RpClumpForAllAtomics(clump, ClearClumpForAllAtomicsFlagCB, &flag);
}

void *CVisibilityPlugins::ClumpConstructor(void *clump, RwInt32 offset, RwInt32 size)
{
    PLUGIN_CLUMP(clump, callback) = DefaultVisibilityCB;
    PLUGIN_CLUMP(clump, alpha) = 255;
}

void *CVisibilityPlugins::ClumpCopyConstructor(void *dstClump, const void *srcClump, RwInt32 offset, RwInt32 size)
{
    PLUGIN_CLUMP(dstClump, callback) = PLUGIN_CLUMP(srcClump, callback);
}

void *CVisibilityPlugins::ClumpDestructor(void *clump, RwInt32 offset, RwInt32 size)
{
    return clump;
}

void CVisibilityPlugins::DefaultVisibilityCB(void*,float)
{
}

void *CVisibilityPlugins::FrameConstructor(void *frame, RwInt32 offset, RwInt32 size)
{
    PLUGIN_FRAME(frame, hierid) = 0;
}

void *CVisibilityPlugins::FrameCopyConstructor(void *dstFrame, const void *srcFrame, RwInt32 offset, RwInt32 size)
{
    PLUGIN_FRAME(dstFrame, hierid) = PLUGIN_FRAME(srcFrame, hierid);
}

void *CVisibilityPlugins::FrameDestructor(void *frame, RwInt32 offset, RwInt32 size)
{
    return frame;
}

bool CVisibilityPlugins::FrustumSphereCB(RpClump* clump)
{
    RwFrame* frame = GetClumpFrame(clump);
    CClumpModelInfo* modelInfo = (CClumpModelInfo*)GetFrameHierarchyId(frame);
    RwSphere frustum;
    frustum = modelInfo->GetColModel()->GetBoundingBox();
    RwMatrix* ltm = RwFrameGetLTM(frame);
    RwV3dTransformPoints(&frustum.center, &frustum.center, 1, ltm);
    return RwCameraFrustumTestSphere(ms_pCamera, &frustum) != 0;
}

int CVisibilityPlugins::GetAtomicId(RpAtomic *pAtomic)
{
    return PLUGIN_ATOMIC(pAtomic, user_value);
}

uint8_t CVisibilityPlugins::GetClumpAlpha(RpClump *pClump)
{
    return PLUGIN_CLUMP(pClump, alpha);
}

CClumpModelInfo* CVisibilityPlugins::GetClumpModelInfo(RpClump* pClump)
{
    return PLUGIN_FRAME(pClump->object.parent, pClumpModelInfo);
}

float CVisibilityPlugins::GetDistanceSquaredFromCamera(RwFrame *pFrame)
{
    RwMatrix *ltm = RwFrameGetLTM(pFrame);
    CVector dpos = ltm->pos - *ms_pCameraPosn;
    return dpos.MagnitudeSquared();
}

float CVisibilityPlugins::GetDistanceSquaredFromCamera(RwV3d* vec)
{
    CVector dpos = *vec - *ms_pCameraPosn;
    return dpos.MagnitudeSquared();
}

float CVisibilityPlugins::GetDotProductWithCameraVector(RwMatrixTag *pMat1, RwMatrixTag *pMat2, unsigned int flags)
{
    RwV3d dpos = pMat1->pos - *ms_pCameraPosn;
    float dot = 0.0;
    if(flags & 0x30)
    {
        dot = DotProduct(pMat2->up, dpos);
    }
    else if(flags & 0xC)
    {
        dot = DotProduct(pMat2->right, dpos);
    }
    if(flags & 0x24)
    {
        dot = -dot;
    }
    if(!(flags & 0x8000))
    {
        float add = DotProduct(pMat2->at, dpos);
        if(flags & 0x300)
        {
            add *= 0.25f;
        }
        else
        {
            add *= 2.5f;
        }
        dot += add;
    }
    if(flags & 0x300)
    {
        float add = 0.0f;
        if(flags & 0x100)
        {
            add = -DotProduct(pMat2->up, dpos);
        }
        else if(flags & 0x200)
        {
            add = DotProduct(pMat2->up, dpos);
        }
        if(dot != 0.0f && add != 0.0f)
        {
            dot += add;
        }
    }
    return dot;
}

int CVisibilityPlugins::GetFrameHierarchyId(RwFrame *pFrame)
{
    return PLUGIN_FRAME(pFrame, hierid);
}

CBaseModelInfo *CVisibilityPlugins::GetModelInfo(RpAtomic* pAtomic)
{
    int id = GetModelInfoIndex(pAtomic);
    return id == -1 ? NULL : CModelInfo::GetModelInfo(id);
}

int CVisibilityPlugins::GetModelInfoIndex(RpAtomic* pAtomic)
{
    return PLUGIN_ATOMIC(pAtomic, id);
}

int CVisibilityPlugins::GetUserValue(RpAtomic *pAtomic)
{
    return PLUGIN_ATOMIC(pAtomic, user_value);
}

void CVisibilityPlugins::InitAlphaAtomicList()
{
    m_alphaList.Clear();
}

void CVisibilityPlugins::InitAlphaEntityList()
{
    m_alphaEntityList.Clear();
    m_alphaBoatAtomicList.Clear();
    m_alphaUnderwaterEntityList.Clear();
    m_alphaReallyDrawLastList.Clear();
}

bool CVisibilityPlugins::InsertAtomicIntoBoatSortedList(RpAtomic *pAtomic, float fAlpha)
{
    AlphaObjectInfo info;
    info.atomic = pAtomic;
    info.renderCB = RenderAtomic;
    info.alpha = fAlpha;
    return m_alphaBoatAtomicList.InsertSorted(info);
}

bool CVisibilityPlugins::InsertAtomicIntoReallyDrawLastList(RpAtomic *pAtomic, float fAlpha)
{
    AlphaObjectInfo info;
    info.atomic = pAtomic;
    info.renderCB = RenderAtomic;
    info.alpha = fAlpha;
    return m_alphaReallyDrawLastList.InsertSorted(info);
}

bool CVisibilityPlugins::InsertAtomicIntoSortedList(RpAtomic *pAtomic, float fAlpha)
{
    AlphaObjectInfo info;
    info.atomic = pAtomic;
    info.renderCB = RenderAtomic;
    info.alpha = fAlpha;
    return m_alphaList.InsertSorted(info);
}

bool CVisibilityPlugins::InsertEntityIntoReallyDrawLastList(CEntity *pEntity, float fAlpha)
{
    AlphaObjectInfo info;
    info.entity = pEntity;
    info.renderCB = RenderEntity;
    info.alpha = fAlpha;
    return m_alphaReallyDrawLastList.InsertSorted(info);
}

bool CVisibilityPlugins::InsertEntityIntoSortedList(CEntity *pEntity, float fAlpha)
{
    AlphaObjectInfo info;
    info.entity = pEntity;
    info.renderCB = RenderEntity;
    info.alpha = fAlpha;
    if(pEntity->GetModelIndex() == MI_GRASSHOUSE)
    {
        return m_alphaReallyDrawLastList.InsertSorted(info);
    }
    else if(pEntity->bUnderwater)
    {
        return m_alphaUnderwaterEntityList.InsertSorted(info);
    }
    else
    {
        return m_alphaEntityList.InsertSorted(info);
    }
}

bool CVisibilityPlugins::InsertEntityIntoUnderwaterList(CEntity *pEntity, float fAlpha)
{
    AlphaObjectInfo info;
    info.entity = pEntity;
    info.renderCB = RenderEntity;
    info.alpha = fAlpha;
    return m_alphaUnderwaterEntityList.InsertSorted(info);
}

bool CVisibilityPlugins::InsertObjectIntoSortedList(void *object, float alpha, RenderAlphaCallback callback)
{
    AlphaObjectInfo info;
    info.object = object;
    info.alpha = alpha;
    info.renderCB = callback;
    return m_alphaEntityList.InsertSorted(info);
}

bool CVisibilityPlugins::PluginAttach()
{
    ms_atomicPluginOffset = RpAtomicRegisterPlugin(sizeof(AtomicPluginDef),
                                                   MAKECHUNKID(rwVENDORID_ROCKSTAR, rwID_VPATOMIC),
                                                   AtomicConstructor,
                                                   AtomicDestructor,
                                                   AtomicCopyConstructor);
    ms_framePluginOffset = RwFrameRegisterPlugin(sizeof(FramePluginDef),
                                                 MAKECHUNKID(rwVENDORID_ROCKSTAR, rwID_VPFRAME),
                                                 FrameConstructor,
                                                 FrameDestructor,
                                                 FrameCopyConstructor);
    ms_clumpPluginOffset = RpClumpRegisterPlugin(sizeof(ClumpPluginDef),
                                                 MAKECHUNKID(rwVENDORID_ROCKSTAR, rwID_VPCLUMP),
                                                 ClumpConstructor,
                                                 ClumpDestructor,
                                                 ClumpCopyConstructor);
    return ms_atomicPluginOffset != -1 && ms_clumpPluginOffset != -1;
}

void CVisibilityPlugins::RenderAlphaAtomics()
{
    RenderOrderedList(m_alphaList);
}

void CVisibilityPlugins::RenderAtomic(void *atomic, float)
{
    AtomicDefaultRenderCallBack(atomic);
}

void CVisibilityPlugins::RenderAtomicWithAlpha(RpAtomic *pAtomic, int alpha)
{
    RpGeometry *pGeom = pAtomic->geometry;
    int savedFlags = pGeom->flags; // save flags
    char alphas[153];
    pGeom->flags |= 0x40;
    // clip alphas
    for(int i = 0; i < pGeom->matList.numMaterials; i++)
    {
        RpMaterial *pMat = pGeom->matList.materials[i];
        alphas[i] = pMat->color.alpha;
        if(pMat->color.alpha > alpha)
        {
            pMat->color.alpha = alpha;
        }
    }
    AtomicDefaultRenderCallBack(pAtomic);
    // restore alphas
    for(int i = 0; i < pGeom->matList.numMaterials; i++)
    {
        RpMaterial *pMat = pGeom->matList.materials[i];
        pMat->color.alpha = alphas[i];
    }
    pGeom->flags = savedFlags; // restore flags
}

RpAtomic* CVisibilityPlugins::RenderAtomicWithAlphaCB(RpAtomic* atomic, void* alpha)
{
    if(atomic->object.object.flags & 4)
    {
        RenderAtomicWithAlpha(atomic, *(int*)alpha);
    }
    return atomic;
}

void CVisibilityPlugins::RenderBoatAlphaAtomics()
{
    RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
    RenderOrderedList(m_alphaBoatAtomicList);
    RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
}

void CVisibilityPlugins::RenderEntity(void *entity, float alpha)
{
    CEntity *pEntity = (CEntity*)entity;
    RwObject *pRwObj = pEntity->GetRwObject();
    if(!pRwObj)
    {
        return;
    }
    CBaseModelInfo *pModelInfo = CModelInfo::GetModelInfo(pEntity->GetModelIndex());
    if(pModelInfo->flags & 4)
    {
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
    }
    if(pEntity->GetIsDrawnLast())
    {
        if(CGame::currArea || pModelInfo->flags & 8)
        {
            RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, 0);
        }
        else
        {
            RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, (void*)100);
        }
        CRenderer::RenderOneNonRoad(pEntity);
    }
    else
    {
        RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, 0);
        float fFadingAlpha = CalculateFadingAtomicAlpha(pModelInfo, pEntity, alpha);
        pEntity->SetIsBeingRendered(true);
        if(!pEntity->GetIsBackfaceCulled())
        {
            RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
        }
        pEntity->SetupLighting();
        if(pRwObj->type == RW_TYPE_ATOMIC)
        {
            RenderFadingAtomic(pModelInfo, pEntity, fFadingAlpha);
        }
        else
        {
            RenderFadingClump(pModelInfo, pEntity, fFadingAlpha);
        }
        pEntity->RemoveLighting();
        pEntity->SetIsBeingRendered(false);
        if(!pEntity->GetIsBackfaceCulled())
        {
            RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
        }
    }
    if(pModelInfo->flags & 8)
    {
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)2);
    }
}

void CVisibilityPlugins::RenderFadingAtomic(CBaseModelInfo *pModelInfo, RpAtomic *pAtomic, int bias)
{
    SetupRenderFadingAtomic(pModelInfo, ?);
    RenderAtomicWithAlpha(pAtomic, bias);
    ResetRenderFadingAtomic(pModelInfo);
}

void CVisibilityPlugins::RenderFadingClump(CBaseModelInfo *pModelInfo, RpClump *pClump, int bias)
{
    SetupRenderFadingAtomic(pModelInfo, ?);
    RpClumpForAllAtomics(pClump, RenderAtomicWithAlphaCB, &bias);
    ResetRenderFadingAtomic(pModelInfo);
}

RpAtomic* CVisibilityPlugins::RenderFadingClumpCB(RpAtomic *pAtomic)
{
    uint8_t alpha = GetClumpAlpha(pAtomic->clump);
    if(alpha == 255)
    {
        AtomicDefaultRenderCallBack(pAtomic);
    }
    else
    {
        RenderAtomicWithAlpha(pAtomic, alpha);
    }
    return pAtomic;
}

void CVisibilityPlugins::RenderFadingEntities()
{
    RenderOrderedList(m_alphaEntityList);
    RenderBoatAlphaAtomics();
}

void CVisibilityPlugins::RenderFadingUnderwaterEntities()
{
    RenderOrderedList(m_alphaUnderwaterEntityList);
}

RpAtomic* CVisibilityPlugins::RenderHeliRotorAlphaCB(RpAtomic* pAtomic)
{
    if(gVehicleDistanceFromCamera < ms_bigVehicleLod0Dist)
    {
        RwMatrix *atomic_ltm = RwFrameGetLTM(GetAtomicFrame(pAtomic));
        RwMatrix *clump_ltm = RwFrameGetLTM(GetClumpFrame(pAtomic->clump));
        RwV3d dpos = atomic_ltm->pos - *ms_pCameraPosn;
        float dot = DotProduct(dpos, clump_ltm->at);
        float alpha = dot * 20.0f + gVehicleDistanceFromCamera;
        if(!InsertAtomicIntoSortedList(pAtomic, alpha))
        {
            AtomicDefaultRenderCallBack(pAtomic);
        }
    }
    return pAtomic;
}

RpAtomic* CVisibilityPlugins::RenderHeliTailRotorAlphaCB(RpAtomic *pAtomic)
{
    if(gVehicleDistanceFromCamera < ms_vehicleLod0Dist)
    {
        RwMatrix *atomic_ltm = RwFrameGetLTM(GetAtomicFrame(pAtomic));
        RwMatrix *clump_ltm = RwFrameGetLTM(GetClumpFrame(pAtomic->clump));
        RwV3d dpos = atomic_ltm->pos - *ms_pCameraPosn;
        float dot = -DotProduct(dpos, clump_ltm->right) - DotProduct(dpos, clump_ltm->up);
        float alpha = dot + gVehicleDistance;
        if(!InsertAtomicIntoSortedList(pAtomic, alpha))
        {
            AtomicDefaultRenderCallBack(pAtomic);
        }
    }
    return pAtomic;
}

void CVisibilityPlugins::RenderOrderedList(CLinkList<AlphaObjectInfo>& list)
{
    //TODO
}

void CVisibilityPlugins::RenderReallyDrawLastObjects()
{
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)1);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)1);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)1);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)1);
    RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
    SetAmbientColours();
    DeActivateDirectional();
    RenderOrderedList(m_alphaReallyDrawLastList);
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, 0);
}

void CVisibilityPlugins::SetAtomicRenderCallback(RpAtomic* atomic, AtomicRenderCallback callback)
{
    atomic->renderCallBack = callback ? callback : AtomicDefaultRenderCallBack;
}

void CVisibilityPlugins::SetupVehicleVariables(RpClump* clump)
{
    if(clump->object.type == RW_TYPE_CLUMP)
    {
        RwFrame* frame = GetClumpFrame(clump);
        const RwMatrix* ltm = RwFrameGetLTM(frame);
        const CVector dpos = ltm->pos - *ms_pCameraPosn;
        gVehicleDistanceFromCamera = dpos.MagnitudeSquared();
        const CVector dang = *ms_pCameraPosn - frame->modelling.pos; //
        gVehicleAngleFromCamera = atan2(dang.Magnitude2D(), dang.z);
    }
}

RpAtomic* CVisibilityPlugins::RenderVehicleHiDetailCB(RpAtomic* atomic)
{
    if(gVehicleDistanceFromCamera < ms_vehicleLod0Dist)
    {
        if(gVehicleDistanceFromCamera >= ms_vehicleLod0RenderMultiPassDist)
        {
            SetAtomicFlag(atomic, 0x2000);
        }
        else
        {
            ClearAtomicFlag(atomic, 0x2000);
        }
        float dot = 0.0f;
        if(CanRenderAtomic(atomic, dot))
        {
            AtomicDefaultRenderCallBack(atomic);
        }
    }
    return atomic;
}

RpAtomic* CVisibilityPlugins::RenderVehicleReallyLowDetailCB(RpAtomic* atomic)
{
    if(gVehicleDistanceFromCamera >= ms_vehicleLod0Dist)
    {
        SetAtomicFlag(atomic, 0x2000);
        if(GetClumpAlpha(atomic->clump) == 255)
        {
            AtomicDefaultRenderCallBack(atomic);
        }
        else
        {
            RenderAtomicWithAlpha(atomic, GetClumpAlpha(atomic->clump));
        }
    }
    return atomic;
}

RpAtomic* CVisibilityPlugins::RenderVehicleReallyLowDetailCB_BigVehicle(RpAtomic* atomic)
{
    if(gVehicleDistanceFromCamera >= ms_bigVehicleLod0Dist)
    {
        AtomicDefaultRenderCallBack(atomic);
    }
    return atomic;
}

RpAtomic* CVisibilityPlugins::RenderTrainHiDetailAlphaCB(RpAtomic* atomic)
{
    if(gVehicleDistanceFromCamera < ms_bigVehicleLod0Dist)
    {
        if(gVehicleDistanceFromCamera >= ms_vehicleLod0RenderMultiPassDist)
        {
            SetAtomicFlag(atomic, 0x2000);
        }
        else
        {
            ClearAtomicFlag(atomic, 0x2000);
        }
        float dot = 0.0f;
        if(CanRenderAtomic(atomic, dot))
        {
            float alpha = gVehicleDistanceFromCamera;
            if(!(GetAtomicId(atomic) & 0x40))
            {
                alpha += dot;
            }
            if(!InsertAtomicIntoSortedList(atomic, alpha))
            {
                AtomicDefaultRenderCallBack(atomic);
            }
        }
    }
    return atomic;
}

bool CVisibilityPlugins::CanRenderAtomic(RpAtomic* atomic, float& dot)
{
    RwMatrixTag* atomicLTM = RwFrameGetLTM(GetAtomicFrame(atomic));
    RwMatrixTag* clumpLTM = RwFrameGetLTM(GetClumpFrame(atomic->clump));
    dot = GetDotProductWithCameraVector(atomicLTM, clumpLTM, GetAtomicId(atomic));
    if(gVehicleDistanceFromCamera <= ms_cullCompsDist)
    {
        return true;
    }
    else if(GetAtomicId(atomic) & 0x400)
    {
        return true;
    }
    else if(gVehicleAngleFromCamera >= 0.2f)
    {
        return true;
    }
    else if(dot <= 0.0f)
    {
        return true;
    }
    else if((GetAtomicId(atomic) & 0x80) && (gVehicleDistanceFromCamera / 10.0f) >= dot * dot)
    {
        return true;
    }
    return false;
}

bool CVisibilityPlugins::CanRenderAtomicBigVeh(RpAtomic* atomic, float& dot)
{
   RwMatrixTag* atomicLTM = RwFrameGetLTM(GetAtomicFrame(atomic));
    RwMatrixTag* clumpLTM = RwFrameGetLTM(GetClumpFrame(atomic->clump));
    dot = GetDotProductWithCameraVector(atomicLTM, clumpLTM, GetAtomicId(atomic));
    if(gVehicleDistanceFromCamera <= ms_cullBigCompsDist)
    {
        return true;
    }
    else if(GetAtomicId(atomic) & 0x400)
    {
        return true;
    }
    else if(gVehicleAngleFromCamera >= 0.2f)
    {
        return true;
    }
    else if(dot <= 0.0f)
    {
        return true;
    }
    else if((GetAtomicId(atomic) & 0x80) && (gVehicleDistanceFromCamera / 10.0f) >= dot * dot)
    {
        return true;
    }
    return false;
}

RpAtomic* CVisibilityPlugins::RenderTrainHiDetailCB(RpAtomic* atomic)
{
    if(gVehicleDistanceFromCamera < ms_bigVehicleLod0Dist)
    {
        if(gVehicleDistanceFromCamera >= ms_vehicleLod0RenderMultiPassDist)
        {
            SetAtomicFlag(atomic, 0x2000);
        }
        else
        {
            ClearAtomicFlag(atomic, 0x2000);
        }
        float dot = 0.0f;
        if(CanRenderAtomic(atomic, dot))
        {
            AtomicDefaultRenderCallBack(atomic);
        }
    }
    return atomic;
}

RpAtomic* CVisibilityPlugins::RenderVehicleHiDetailAlphaCB(RpAtomic* atomic)
{
    if(gVehicleDistanceFromCamera < ms_vehicleLod0Dist)
    {
        if(gVehicleDistanceFromCamera >= ms_vehicleLod0RenderMultiPassDist)
        {
            SetAtomicFlag(atomic, 0x2000);
        }
        else
        {
            ClearAtomicFlag(atomic, 0x2000);
        }
        float dot = 0.0f;
        if(CanRenderAtomic(atomic, dot))
        {
            float alpha = gVehicleDistanceFromCamera;
            if(GetAtomicId(atomic) & 0x40)
            {
                alpha -= 1/10000;
            }
            else
            {
                alpha += dot;
            }
            if(!InsertAtomicIntoSortedList(atomic, alpha))
            {
                AtomicDefaultRenderCallBack(atomic);
            }
        }
    }
    return atomic;
}

RpAtomic* CVisibilityPlugins::RenderVehicleHiDetailAlphaCB_BigVehicle(RpAtomic* atomic)
{
    if(gVehicleDistanceFromCamera < ms_bigVehicleLod0Dist)
    {
        if(gVehicleDistanceFromCamera >= ms_vehicleLod0RenderMultiPassDist)
        {
            SetAtomicFlag(atomic, 0x2000);
        }
        else
        {
            ClearAtomicFlag(atomic, 0x2000);
        }
        float dot = 0.0f;
        if(CanRenderAtomicBigVeh(atomic, dot))
        {
            float alpha = gVehicleDistanceFromCamera;
            if(GetAtomicId(atomic) & 0x40)
            {
                alpha -= 1/10000;
            }
            else
            {
                alpha += dot;
            }
            if(!InsertAtomicIntoSortedList(atomic, alpha))
            {
                AtomicDefaultRenderCallBack(atomic);
            }
        }
    }
}

RpAtomic* CVisibilityPlugins::RenderPedCB(RpAtomic* atomic)
{
    if(GetDistanceSquaredFromCamera(GetAtomicFrame(atomic)) < ms_pedLodDist)
    {
        if(GetClumpAlpha(atomic->clump) == 255)
        {
            AtomicDefaultRenderCallBack(atomic);
        }
        else
        {
            RenderAtomicWithAlpha(atomic, GetClumpAlpha(atomic->clump));
        }
    }
    return atomic;
}