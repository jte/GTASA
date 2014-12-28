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
    RwInt16 userValue;
};

struct FramePluginDef
{
    union 
    {
        RwInt32 hierid;
        CClumpModelInfo* clumpModelInfo;
    };
};

struct ClumpPluginDef
{
    RenderAlphaCallback callback;
    RwInt32 alpha;
};

void* CVisibilityPlugins::AtomicConstructor(void* atomic, RwInt32 offset, RwInt32 size)
{
    if (ms_atomicPluginOffset > 0)
    {
        PLUGIN_ATOMIC(atomic, id) = -1;
        PLUGIN_ATOMIC(atomic, userValue) = 0;
    }
    return atomic;
}


void* CVisibilityPlugins::AtomicDestructor(void* atomic, RwInt32 offset, RwInt32 size)
{
    return atomic;
}


void* CVisibilityPlugins::AtomicCopyConstructor(void* dstAtomic, const void* srcAtomic, RwInt32 offset, RwInt32 size)
{
    PLUGIN_ATOMIC(dstAtomic, id) = PLUGIN_ATOMIC(srcAtomic, id);
    PLUGIN_ATOMIC(dstAtomic, userValue) = PLUGIN_ATOMIC(srcAtomic, userValue);
    return dstAtomic;
}

float CVisibilityPlugins::CalculateFadingAtomicAlpha(CBaseModelInfo* modelInfo, CEntity* entity, float currentAlpha)
{
    float scale = 20.0f;
    float maxPlaneRange = modelInfo->GetColModel()->GetBoundingBox().GetRadius() + CRenderer::ms_fFarClipPlane;
    float rangeCam = TheCamera.GenerationDistMultiplier * modelInfo->GetLoadDistanceUnscaled();
    if (rangeCam < maxPlaneRange)
    {
        maxPlaneRange = rangeCam;
    }
    if (!entity->GetLodEntity())
    {
        float distScale = modelInfo->GetLoadDistanceUnscaled();
        if (distScale >= maxPlaneRange)
        {
            distScale = maxPlaneRange;
        }
        if (distScale > 150.0f)
        {
            scale = distScale / 15.0f + 10.0f;
            if (entity->GetIsBigBuilding())
            {
                maxPlaneRange *= CRenderer::ms_lowLodDistScale;
            }
        }
    }
    float alphaMult = ClampMax((maxPlaneRange + 20.0f - currentAlpha) / scale, 1.0f);
    return modelInfo->GetAlpha() * alphaMult;
}

void CVisibilityPlugins::ClearAtomicFlag(RpAtomic* atomic, unsigned short usFlag)
{
    PLUGIN_ATOMIC(atomic, userValue) &= ~usFlag;
}

static RpAtomic* ClearClumpForAllAtomicsFlagCB(RpAtomic* atomic, void* data)
{
    CVisibilityPlugins::ClearAtomicFlag(atomic, *(unsigned short*)data);
}

void CVisibilityPlugins::ClearClumpForAllAtomicsFlag(RpClump* clump, int flag)
{
    RpClumpForAllAtomics(clump, ClearClumpForAllAtomicsFlagCB, &flag);
}

void* CVisibilityPlugins::ClumpConstructor(void* clump, RwInt32 offset, RwInt32 size)
{
    PLUGIN_CLUMP(clump, callback) = DefaultVisibilityCB;
    PLUGIN_CLUMP(clump, alpha) = 255;
}

void* CVisibilityPlugins::ClumpCopyConstructor(void* dstClump, const void* srcClump, RwInt32 offset, RwInt32 size)
{
    PLUGIN_CLUMP(dstClump, callback) = PLUGIN_CLUMP(srcClump, callback);
}

void* CVisibilityPlugins::ClumpDestructor(void* clump, RwInt32 offset, RwInt32 size)
{
    return clump;
}

void CVisibilityPlugins::DefaultVisibilityCB(void*,float)
{
}

void* CVisibilityPlugins::FrameConstructor(void* frame, RwInt32 offset, RwInt32 size)
{
    PLUGIN_FRAME(frame, hierid) = 0;
}

void* CVisibilityPlugins::FrameCopyConstructor(void* dstFrame, const void* srcFrame, RwInt32 offset, RwInt32 size)
{
    PLUGIN_FRAME(dstFrame, hierid) = PLUGIN_FRAME(srcFrame, hierid);
}

void* CVisibilityPlugins::FrameDestructor(void* frame, RwInt32 offset, RwInt32 size)
{
    return frame;
}

bool CVisibilityPlugins::FrustumSphereCB(RpClump* clump)
{
    RwFrame* frame = GetClumpFrame(clump);
    CClumpModelInfo* modelInfo = (CClumpModelInfo*)GetFrameHierarchyId(frame);
    RwSphere frustum  = modelInfo->GetColModel()->GetBoundingBox();
    RwMatrix* ltm = RwFrameGetLTM(frame);
    RwV3dTransformPoints(&frustum.center, &frustum.center, 1, ltm);
    return RwCameraFrustumTestSphere(ms_pCamera, &frustum) != 0;
}

int CVisibilityPlugins::GetAtomicId(RpAtomic* atomic)
{
    return PLUGIN_ATOMIC(atomic, userValue);
}

uint8_t CVisibilityPlugins::GetClumpAlpha(RpClump* clump)
{
    return PLUGIN_CLUMP(clump, alpha);
}

CClumpModelInfo* CVisibilityPlugins::GetClumpModelInfo(RpClump* clump)
{
    return PLUGIN_FRAME(RwFrameGetParent(clump), clumpModelInfo);
}

float CVisibilityPlugins::GetDistanceSquaredFromCamera(RwFrame* frame)
{
    RwMatrix *ltm = RwFrameGetLTM(frame);
    CVector dpos = ltm->pos - *ms_pCameraPosn;
    return dpos.MagnitudeSquared();
}

float CVisibilityPlugins::GetDistanceSquaredFromCamera(RwV3d* vec)
{
    CVector dpos = *vec - *ms_pCameraPosn;
    return dpos.MagnitudeSquared();
}

float CVisibilityPlugins::GetDotProductWithCameraVector(RwMatrixTag* mat1, RwMatrixTag* mat2, unsigned int flags)
{
    RwV3d dpos = mat1->pos - *ms_pCameraPosn;
    float dot = 0.0;
    if (flags & 0x30)
    {
        dot = DotProduct(mat2->up, dpos);
    }
    else if (flags & 0xC)
    {
        dot = DotProduct(mat2->right, dpos);
    }
    if (flags & 0x24)
    {
        dot = -dot;
    }
    if (!(flags & 0x8000))
    {
        float add = DotProduct(mat2->at, dpos);
        if (flags & 0x300)
        {
            add *= 0.25f;
        }
        else
        {
            add *= 2.5f;
        }
        dot += add;
    }
    if (flags & 0x300)
    {
        float add = 0.0f;
        if (flags & 0x100)
        {
            add = -DotProduct(mat2->up, dpos);
        }
        else if (flags & 0x200)
        {
            add = DotProduct(mat2->up, dpos);
        }
        if (dot != 0.0f && add != 0.0f)
        {
            dot += add;
        }
    }
    return dot;
}

int CVisibilityPlugins::GetFrameHierarchyId(RwFrame* frame)
{
    return PLUGIN_FRAME(frame, hierid);
}

CBaseModelInfo* CVisibilityPlugins::GetModelInfo(RpAtomic* atomic)
{
    int id = GetModelInfoIndex(atomic);
    return id == -1 ? NULL : CModelInfo::GetModelInfo(id);
}

int CVisibilityPlugins::GetModelInfoIndex(RpAtomic* atomic)
{
    return PLUGIN_ATOMIC(atomic, id);
}

int CVisibilityPlugins::GetUserValue(RpAtomic* atomic)
{
    return PLUGIN_ATOMIC(atomic, userValue);
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

bool CVisibilityPlugins::InsertAtomicIntoBoatSortedList(RpAtomic* atomic, float alpha)
{
    AlphaObjectInfo info;
    info.atomic = atomic;
    info.renderCB = RenderAtomic;
    info.alpha = alpha;
    return m_alphaBoatAtomicList.InsertSorted(info);
}

bool CVisibilityPlugins::InsertAtomicIntoReallyDrawLastList(RpAtomic* atomic, float alpha)
{
    AlphaObjectInfo info;
    info.atomic = atomic;
    info.renderCB = RenderAtomic;
    info.alpha = alpha;
    return m_alphaReallyDrawLastList.InsertSorted(info);
}

bool CVisibilityPlugins::InsertAtomicIntoSortedList(RpAtomic* atomic, float alpha)
{
    AlphaObjectInfo info;
    info.atomic = atomic;
    info.renderCB = RenderAtomic;
    info.alpha = alpha;
    return m_alphaList.InsertSorted(info);
}

bool CVisibilityPlugins::InsertEntityIntoReallyDrawLastList(CEntity* entity, float alpha)
{
    AlphaObjectInfo info;
    info.entity = entity;
    info.renderCB = RenderEntity;
    info.alpha = alpha;
    return m_alphaReallyDrawLastList.InsertSorted(info);
}

bool CVisibilityPlugins::InsertEntityIntoSortedList(CEntity* entity, float alpha)
{
    AlphaObjectInfo info;
    info.entity = entity;
    info.renderCB = RenderEntity;
    info.alpha = alpha;
    if(entity->GetModelIndex() == MI_GRASSHOUSE)
    {
        return m_alphaReallyDrawLastList.InsertSorted(info);
    }
    else if (entity->bUnderwater)
    {
        return InsertEntityIntoUnderwaterList(entity, alpha);
    }
    else
    {
        return m_alphaEntityList.InsertSorted(info);
    }
}

bool CVisibilityPlugins::InsertEntityIntoUnderwaterList(CEntity* entity, float alpha)
{
    AlphaObjectInfo info;
    info.entity = entity;
    info.renderCB = RenderEntity;
    info.alpha = alpha;
    return m_alphaUnderwaterEntityList.InsertSorted(info);
}

bool CVisibilityPlugins::InsertObjectIntoSortedList(void* object, float alpha, RenderAlphaCallback callback)
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
    AtomicDefaultRenderCallBack((RpAtomic*)atomic);
}

void CVisibilityPlugins::RenderAtomicWithAlpha(RpAtomic* atomic, int alpha)
{
    RpGeometry *geom = atomic->geometry;
    int savedFlags = geom->flags; // save flags
    char alphas[153];
    geom->flags |= rpGEOMETRYMODULATEMATERIALCOLOR;
    // clip alphas
    for(int i = 0; i < geom->matList.numMaterials; i++)
    {
        RpMaterial* material = geom->matList.materials[i];
        alphas[i] = material->color.alpha;
        if (material->color.alpha > alpha)
        {
            material->color.alpha = alpha;
        }
    }
    AtomicDefaultRenderCallBack(atomic);
    // restore alphas
    for(int i = 0; i < geom->matList.numMaterials; i++)
    {
        RpMaterial* material = geom->matList.materials[i];
        material->color.alpha = alphas[i];
    }
    geom->flags = savedFlags; // restore flags
}

RpAtomic* CVisibilityPlugins::RenderAtomicWithAlphaCB(RpAtomic* atomic, void* alpha)
{
    if (rwObjectTestFlags(&atomic->object.object, 4))
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

void CVisibilityPlugins::RenderEntity(void* ventity, float alpha)
{
    CEntity* entity = (CEntity*)ventity;
    RwObject* rwObject = entity->GetRwObject();
    if(!rwObject)
    {
        return;
    }
    CBaseModelInfo* modelInfo = CModelInfo::GetModelInfo(entity->GetModelIndex());
    if (modelInfo->IsLod())
    {
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
    }
    if (entity->GetIsDrawnLast())
    {
        if (CGame::currArea || modelInfo->GetDontCastShadowsOn())
        {
            RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, 0);
        }
        else
        {
            RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, (void*)100);
        }
        CRenderer::RenderOneNonRoad(entity);
    }
    else
    {
        RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, 0);
        float fadingAlpha = CalculateFadingAtomicAlpha(modelInfo, entity, alpha);
        entity->SetIsBeingRendered(true);
        if(!entity->GetIsBackfaceCulled())
        {
            RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
        }
        entity->SetupLighting();
        if (rwObject->type == RW_TYPE_ATOMIC)
        {
            RenderFadingAtomic(modelInfo, (RpAtomic*)rwObject, fadingAlpha);
        }
        else
        {
            RenderFadingClump(modelInfo, (RpClump*)rwObject, fadingAlpha);
        }
        entity->RemoveLighting();
        entity->SetIsBeingRendered(false);
        if (!entity->GetIsBackfaceCulled())
        {
            RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
        }
    }
    if (modelInfo->IsLod())
    {
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)1);
    }
}

void CVisibilityPlugins::RenderFadingAtomic(CBaseModelInfo* modelInfo, RpAtomic* atomic, int bias)
{
    SetupRenderFadingAtomic(modelInfo);
    RenderAtomicWithAlpha(atomic, bias);
    ResetRenderFadingAtomic(modelInfo);
}

void CVisibilityPlugins::RenderFadingClump(CBaseModelInfo* modelInfo, RpClump* clump, int bias)
{
    SetupRenderFadingAtomic(modelInfo);
    RpClumpForAllAtomics(clump, RenderAtomicWithAlphaCB, &bias);
    ResetRenderFadingAtomic(modelInfo);
}

RpAtomic* CVisibilityPlugins::RenderFadingClumpCB(RpAtomic *pAtomic)
{
    uint8_t alpha = GetClumpAlpha(pAtomic->clump);
    if (alpha == 255)
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

RpAtomic* CVisibilityPlugins::RenderHeliRotorAlphaCB(RpAtomic* atomic)
{
    if (gVehicleDistanceFromCamera < ms_bigVehicleLod0Dist)
    {
        RwMatrix* atomic_ltm = RwFrameGetLTM(GetAtomicFrame(atomic));
        RwMatrix* clump_ltm = RwFrameGetLTM(GetClumpFrame(atomic->clump));
        RwV3d dpos = atomic_ltm->pos - *ms_pCameraPosn;
        float dot = DotProduct(dpos, clump_ltm->at);
        float alpha = dot * 20.0f + gVehicleDistanceFromCamera;
        if (!InsertAtomicIntoSortedList(atomic, alpha))
        {
            AtomicDefaultRenderCallBack(atomic);
        }
    }
    return atomic;
}

RpAtomic* CVisibilityPlugins::RenderHeliTailRotorAlphaCB(RpAtomic* atomic)
{
    if (gVehicleDistanceFromCamera < ms_vehicleLod0Dist)
    {
        RwMatrix* atomic_ltm = RwFrameGetLTM(GetAtomicFrame(atomic));
        RwMatrix* clump_ltm = RwFrameGetLTM(GetClumpFrame(atomic->clump));
        RwV3d dpos = atomic_ltm->pos - *ms_pCameraPosn;
        float dot = -DotProduct(dpos, clump_ltm->right) - DotProduct(dpos, clump_ltm->up);
        float alpha = dot + gVehicleDistanceFromCamera;
        if (!InsertAtomicIntoSortedList(atomic, alpha))
        {
            AtomicDefaultRenderCallBack(atomic);
        }
    }
    return atomic;
}

void CVisibilityPlugins::SetRenderWareCamera(RwCamera* camera)
{
#define SQUARED(x) ((x) * (x))
    ms_pCamera = camera;
    ms_pCameraPosn = &RwFrameGetParent(&camera->object.object)->modelling.pos;
    ms_cullCompsDist = SQUARED(TheCamera.GenerationDistMulitpler * 20.0);
    ms_cullBigCompsDist = SQUARED(TheCamera.GenerationDistMulitpler * 50.0);
    ms_vehicleLod0RenderMultiPassDist = 2 * SQUARED(TheCamera.m_fAlphaSpeedAtStartInter * 45.0);
    ms_vehicleLod0Dist = 2 * SQUARED(TheCamera.m_fAlphaSpeedAtStartInter * 70.0);
    ms_vehicleLod1Dist = 2 * SQUARED(TheCamera.m_ffAlphaSpeedAtStartInter * 150.0);
    ms_bigVehicleLod0Dist = ms_vehicleLod1Dist;
    ms_pedLodDist = 2 * SQUARED(TheCamera.GenerationDistMulitpler * 60.0);
    ms_pedFadeDist = 2 * SQUARED(TheCamera.GenerationDistMulitpler * 70.0);
#undef SQUARED
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

void CVisibilityPlugins::SetAtomicRenderCallback(RpAtomic* atomic, RpAtomicCallBackRender callback)
{
    atomic->renderCallBack = callback ? callback : AtomicDefaultRenderCallBack;
}

void CVisibilityPlugins::SetupVehicleVariables(RpClump* clump)
{
    if (clump->object.type == RW_TYPE_CLUMP)
    {
        RwFrame* frame = GetClumpFrame(clump);
        gVehicleDistanceFromCamera = GetDistanceSquaredFromCamera(frame);
        const CVector dang = *ms_pCameraPosn - frame->modelling.pos; //
        gVehicleAngleFromCamera = atan2(dang.Magnitude2D(), dang.z);
    }
}

RpAtomic* CVisibilityPlugins::RenderVehicleHiDetailCB(RpAtomic* atomic)
{
    if (gVehicleDistanceFromCamera < ms_vehicleLod0Dist)
    {
        if (gVehicleDistanceFromCamera >= ms_vehicleLod0RenderMultiPassDist)
        {
            SetAtomicFlag(atomic, 0x2000);
        }
        else
        {
            ClearAtomicFlag(atomic, 0x2000);
        }
        float dot = 0.0f;
        if (CanRenderAtomic(atomic, dot))
        {
            AtomicDefaultRenderCallBack(atomic);
        }
    }
    return atomic;
}

RpAtomic* CVisibilityPlugins::RenderVehicleReallyLowDetailCB(RpAtomic* atomic)
{
    if (gVehicleDistanceFromCamera >= ms_vehicleLod0Dist)
    {
        SetAtomicFlag(atomic, 0x2000);
        if (GetClumpAlpha(atomic->clump) == 255)
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
    if (gVehicleDistanceFromCamera >= ms_bigVehicleLod0Dist)
    {
        AtomicDefaultRenderCallBack(atomic);
    }
    return atomic;
}

RpAtomic* CVisibilityPlugins::RenderTrainHiDetailAlphaCB(RpAtomic* atomic)
{
    if (gVehicleDistanceFromCamera < ms_bigVehicleLod0Dist)
    {
        if (gVehicleDistanceFromCamera >= ms_vehicleLod0RenderMultiPassDist)
        {
            SetAtomicFlag(atomic, 0x2000);
        }
        else
        {
            ClearAtomicFlag(atomic, 0x2000);
        }
        float dot = 0.0f;
        if (CanRenderAtomic(atomic, dot))
        {
            float alpha = gVehicleDistanceFromCamera;
            if (!(GetAtomicId(atomic) & 0x40))
            {
                alpha += dot;
            }
            if (!InsertAtomicIntoSortedList(atomic, alpha))
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
    if (gVehicleDistanceFromCamera <= ms_cullCompsDist)
    {
        return true;
    }
    else if (GetAtomicId(atomic) & 0x400)
    {
        return true;
    }
    else if (gVehicleAngleFromCamera >= 0.2f)
    {
        return true;
    }
    else if (dot <= 0.0f)
    {
        return true;
    }
    else if ((GetAtomicId(atomic) & 0x80) && (gVehicleDistanceFromCamera / 10.0f) >= dot * dot)
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
    if (gVehicleDistanceFromCamera <= ms_cullBigCompsDist)
    {
        return true;
    }
    else if (GetAtomicId(atomic) & 0x400)
    {
        return true;
    }
    else if (gVehicleAngleFromCamera >= 0.2f)
    {
        return true;
    }
    else if (dot <= 0.0f)
    {
        return true;
    }
    else if ((GetAtomicId(atomic) & 0x80) && (gVehicleDistanceFromCamera / 10.0f) >= dot * dot)
    {
        return true;
    }
    return false;
}

RpAtomic* CVisibilityPlugins::RenderTrainHiDetailCB(RpAtomic* atomic)
{
    if (gVehicleDistanceFromCamera < ms_bigVehicleLod0Dist)
    {
        if (gVehicleDistanceFromCamera >= ms_vehicleLod0RenderMultiPassDist)
        {
            SetAtomicFlag(atomic, 0x2000);
        }
        else
        {
            ClearAtomicFlag(atomic, 0x2000);
        }
        float dot = 0.0f;
        if (CanRenderAtomic(atomic, dot))
        {
            AtomicDefaultRenderCallBack(atomic);
        }
    }
    return atomic;
}

RpAtomic* CVisibilityPlugins::RenderVehicleHiDetailAlphaCB(RpAtomic* atomic)
{
    if (gVehicleDistanceFromCamera < ms_vehicleLod0Dist)
    {
        if (gVehicleDistanceFromCamera >= ms_vehicleLod0RenderMultiPassDist)
        {
            SetAtomicFlag(atomic, 0x2000);
        }
        else
        {
            ClearAtomicFlag(atomic, 0x2000);
        }
        float dot = 0.0f;
        if (CanRenderAtomic(atomic, dot))
        {
            float alpha = gVehicleDistanceFromCamera;
            if (GetAtomicId(atomic) & 0x40)
            {
                alpha -= 1/10000;
            }
            else
            {
                alpha += dot;
            }
            if (!InsertAtomicIntoSortedList(atomic, alpha))
            {
                AtomicDefaultRenderCallBack(atomic);
            }
        }
    }
    return atomic;
}

RpAtomic* CVisibilityPlugins::RenderVehicleHiDetailAlphaCB_BigVehicle(RpAtomic* atomic)
{
    if (gVehicleDistanceFromCamera < ms_bigVehicleLod0Dist)
    {
        if (gVehicleDistanceFromCamera >= ms_vehicleLod0RenderMultiPassDist)
        {
            SetAtomicFlag(atomic, 0x2000);
        }
        else
        {
            ClearAtomicFlag(atomic, 0x2000);
        }
        float dot = 0.0f;
        if (CanRenderAtomicBigVeh(atomic, dot))
        {
            float alpha = gVehicleDistanceFromCamera;
            if (GetAtomicId(atomic) & 0x40)
            {
                alpha -= 1/10000;
            }
            else
            {
                alpha += dot;
            }
            if (!InsertAtomicIntoSortedList(atomic, alpha))
            {
                AtomicDefaultRenderCallBack(atomic);
            }
        }
    }
}

RpAtomic* CVisibilityPlugins::RenderPedCB(RpAtomic* atomic)
{
    if (GetDistanceSquaredFromCamera(GetAtomicFrame(atomic)) < ms_pedLodDist)
    {
        if (GetClumpAlpha(atomic->clump) == 255)
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

void CVisibilityPlugins::VehicleVisibilityCB_BigVehicle(void* a1, float)
{
    FrustumSphereCB((RpClump *)a1);
}

void CVisibilityPlugins::VehicleVisibilityCB(RpClump* clump)
{
    RwFrame* frame = RwFrameGetParent(clump);
    if (GetDistanceSquaredFromCamera(frame) < ms_vehicleLod1Dist)
    {
        FrustumSphereCB(clump);
    }/*
  int v1; // r5@1
  int result; // r0@2

  __asm
  {
    VLDR            S15, [R3]
    VMOV            S14, R0
                        a, b
    VCMPE.F32       CVisibilityPlugins::GetDistanceSquaredFromCamera(), CVisibilityPlugins::ms_vehicleLod1Dist
    VMRS            APSR_nzcv, FPSCR
    a >= b || 
  }
  if ( (unsigned __int8)(_NF ^ _VF) | _ZF )
    result = CVisibilityPlugins::FrustumSphereCB(v1);
  else
    result = 0;
  return result;*/
}


void CVisibilityPlugins::SetClumpModelInfo(RpClump* clump, CClumpModelInfo* info)
{
    PLUGIN_FRAME(GetClumpFrame(clump), clumpModelInfo) = info;
    if (info->GetModelType() == 6)
    {
        CVehicleModelInfo* vehicle = reinterpret_cast<CVehicleModelInfo*>(info);
        if (vehicle->VehicleType == 6 || vehicle->VehicleType == 8)
        {
            PLUGIN_CLUMP(clump, callback) = VehicleVisibilityCB_BigVehicle;
        }
        else
        {
            PLUGIN_CLUMP(clump, callback) = VehicleVisibilityCB;
        }
    }
}

void CVisibilityPlugins::Initialise()
{
    m_alphaList.Init(20);
    m_alphaBoatAtomicList.Init(20);
    m_alphaEntityList.Init(200);
    m_alphaUnderwaterEntityList.Init(100);
    m_alphaReallyDrawLastList.Init(50);
    ms_weaponPedsForPC.Init(60);
}

void CVisibilityPlugins::RenderWeaponPedsForPC()
{
}

bool CVisibilityPlugins::IsAtomicVisible(RpAtomic* atomic)
{
    if (atomic->interpolator.flags & rpINTERPOLATORDIRTYSPHERE)
    {
        _rpAtomicResyncInterpolatedSphere(atomic);
    }
    RwSphere sphere = atomic->boundingSphere;
    RwV3dTransformPoints(&sphere.center, &sphere.center, 1, &GetAtomicFrame(atomic)->modelling);
    return RwCameraFrustumTestSphere(ms_pCamera, &sphere) != 0;
}

bool CVisibilityPlugins::IsClumpVisible(RpClump* clump)
{
    return PLUGIN_CLUMP(clump, callback);
}

void CVisibilityPlugins::ResetRenderFadingAtomic(CBaseModelInfo* info)
{
    if (info->IsLod())
    {
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
    }
}

void CVisibilityPlugins::SetupRenderFadingAtomic(CBaseModelInfo* info)
{
    if (info->IsLod())
    {
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDONE);
    }
}

RpAtomic* CVisibilityPlugins::RenderVehicleHiDetailAlphaCB_Boat(RpAtomic* atomic)
{
    if (gVehicleDistanceFromCamera < ms_vehicleLod0Dist )
    {
        if (gVehicleDistanceFromCamera >= ms_vehicleLod0RenderMultiPassDist)
        {
            PLUGIN_ATOMIC(atomic, userValue) |= 0x2000;
        }
        else
        {
            PLUGIN_ATOMIC(atomic, userValue) &= ~0x2000;
        }
        if ((!PLUGIN_ATOMIC(atomic, userValue) & 0x40))
        {
            AtomicDefaultRenderCallBack(atomic);
        }
        else if (!InsertAtomicIntoSortedList(atomic, gVehicleDistanceFromCamera))
        {
            AtomicDefaultRenderCallBack(atomic);
        }
    }
    return atomic;
}

RpAtomic* CVisibilityPlugins::RenderVehicleHiDetailCB_Boat(RpAtomic* atomic)
{
    if (gVehicleDistanceFromCamera < ms_vehicleLod0Dist)
    {
        if (gVehicleDistanceFromCamera >= ms_vehicleLod0RenderMultiPassDist)
        {
            PLUGIN_ATOMIC(atomic, userValue) |= 0x2000;
        }
        else
        {
            PLUGIN_ATOMIC(atomic, userValue) &= 0x2000;
        }
        AtomicDefaultRenderCallBack(atomic);
    }
    return atomic;
}

RpAtomic* CVisibilityPlugins::RenderVehicleLoDetailCB_Boat(RpAtomic* atomic)
{
    if (gVehicleDistanceFromCamera >= ms_vehicleLod0Dist)
    {
        PLUGIN_ATOMIC(atomic, userValue) |= 0x2000;
        if (PLUGIN_CLUMP(atomic->clump, alpha) == 255)
        {
            AtomicDefaultRenderCallBack((RpAtomic *)atomic);
        }
        else
        {
            RenderAtomicWithAlpha(atomic, PLUGIN_CLUMP(atomic->clump, alpha));
        }
    }
    return atomic;
}

RpAtomic* CVisibilityPlugins::RenderVehicleHiDetailCB_BigVehicle(RpAtomic* atomic)
{
    if (gVehicleDistanceFromCamera < ms_bigVehicleLod0Dist)
    {
        if (gVehicleDistanceFromCamera >= ms_vehicleLod0RenderMultiPassDist)
        {
            PLUGIN_ATOMIC(atomic, userValue) |= 0x2000;
        }
        else
        {
            PLUGIN_ATOMIC(atomic, userValue) &= ~0x2000;
        }
        if (gVehicleDistanceFromCamera <= ms_cullBigCompsDist)
        {
            AtomicDefaultRenderCallBack(atomic);
        }
        else if (PLUGIN_ATOMIC(atomic, userValue) & 0x400)
        {
            AtomicDefaultRenderCallBack(atomic);
        }
        else if (gVehicleAngleFromCamera >= 0.2f)
        {
            AtomicDefaultRenderCallBack(atomic);
        }
        RwMatrix* atomicLTM = RwFrameGetLTM(GetAtomicFrame(atomic));
        RwMatrix* clumpLTM = RwFrameGetLTM(GetClumpFrame(atomic->clump));
        if (GetDotProductWithCameraVector(atomicLTM, clumpLTM, GetAtomicId(atomic)) <= 0.0)
        {
            AtomicDefaultRenderCallBack(atomic);
        }
    }
    return atomic;
}

void CVisibilityPlugins::SetClumpAlpha(RpClump* clump, int alpha)
{
    PLUGIN_CLUMP(clump, alpha) = alpha;
}

static RpAtomic* SetAtomicFlagCB(RpAtomic* atomic, void* data)
{
    CVisibilityPlugins::SetAtomicFlag(atomic, *(unsigned short*)data);
    return atomic;
}

void CVisibilityPlugins::SetClumpForAllAtomicsFlag(RpClump* clump, int flags)
{
    RpClumpForAllAtomics(clump, SetAtomicFlagCB, &flags);
}

void CVisibilityPlugins::SetAtomicId(RpAtomic* atomic, int id)
{
    PLUGIN_ATOMIC(atomic, userValue) = id;
}

void CVisibilityPlugins::SetAtomicFlag(RpAtomic* atomic, unsigned short flags)
{
    PLUGIN_ATOMIC(atomic, userValue) |= flags;
}

 void CVisibilityPlugins::SetUserValue(RpAtomic* atomic, unsigned short value)
 {
     PLUGIN_ATOMIC(atomic, userValue) = value;
 }

 void CVisibilityPlugins::SetFrameHierarchyId(RwFrame* frame, int id)
 {
     PLUGIN_FRAME(frame, hierid) = id;
 }

 void CVisibilityPlugins::SetModelInfoIndex(RpAtomic* atomic, int id)
 {
     PLUGIN_ATOMIC(atomic, id) = id;
 }

void CVisibilityPlugins::Shutdown()
{
    m_alphaList.Shutdown();
    m_alphaBoatAtomicList.Shutdown();
    m_alphaEntityList.Shutdown();
    m_alphaReallyDrawLastList.Shutdown();
    ms_weaponPedsForPC.Shutdown();
}

RpAtomic* CVisibilityPlugins::RenderWheelAtomicCB(RpAtomic* atomic)
{
    AtomicDefaultRenderCallBack(atomic);
    return atomic;
}