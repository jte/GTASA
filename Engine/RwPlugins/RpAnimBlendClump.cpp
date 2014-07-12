#include "StdInc.h"

#define rwID_BLEND_CLUMP (0xFB)

#define PLUGIN_ANIM_BLEND_CLUMP(clump, var) \
    (RWPLUGINOFFSET(RpAnimBlendPluginDef, clump, gBlendClumpPlugin)->var)

static RwInt32 gBlendClumpPlugin = -1;

struct RpAnimBlendPluginDef
{
    CAnimBlendClumpData* animBlendClumpData;
};

CAnimBlendClumpData* GetAnimBlendClumpData(RpClump* clump)
{
    return PLUGIN_ANIM_BLEND_CLUMP(clump, animBlendClumpData);
}

static void RpAnimBlendClumpInitClump(RpClump* clump);
static void RpAnimBlendClumpInitAtomic(RpClump* clump);

void RpAnimBlendClumpInit(RpClump* clump)
{
    RpAtomic* atomic = GetFirstAtomic(clump);
    if(atomic && RpSkinGeometryGetSkin(atomic->geometry))
    {
        RpAnimBlendClumpInitClump(clump);
    }
    else
    {
        RpAnimBlendClumpInitAtomic(clump);
    }
}

static void RpAnimBlendClumpInitFrameCB(AnimBlendFrameData* frameData, void*)
{
    // Initialize frame data
    frameData->flags = 0;
}

void RpAnimBlendClumpInitClump(RpClump *clump)
{
    RpAtomic *atomic = GetFirstAtomic(clump);
    RpSkin *skin = RpSkinGeometryGetSkin(atomic->geometry);
    uint32_t boneCount = RpSkinGetNumBones(skin);
    CAnimBlendClumpData *animBlendClumpData = GetAnimBlendClumpData(clump);

    animBlendClumpData->SetNumberOfBones(boneCount);
    RpHAnimHierarchy *animHierarchy = GetAnimHierarchyFromSkinClump(clump);
    RwV3d boneTable[63];
    SkinGetBonePositionsToTable(clump, boneTable);
    for(size_t i = 0; i < boneCount; i++)
    {
        AnimBlendFrameData *pFrame = animBlendClumpData->GetFrame(i);
        //pFrame->pad2 = pAnimHierarchy->currentAnim + pAnimHierarchy->currentAnim->currentInterpKeyFrameSize * i + 0x4C; TODO
        pFrame->node = &animHierarchy->pNodeInfo[i];
        pFrame->pos = boneTable[i];
    }
    // Initialize frames
    animBlendClumpData->ForAllFrames(RpAnimBlendClumpInitFrameCB, NULL);
    // Mark root frame
    animBlendClumpData->GetFrame(0)->flags |= 8;
}

bool RpAnimBlendClumpIsInitialized(RpClump* clump)
{
    CAnimBlendClumpData *data = GetAnimBlendClumpData(clump);
    return data && data->GetAnimBlendNodeCount();
}

void RpAnimBlendClumpAddAssociation(RpClump* clump, CAnimBlendAssociation* assoc, uint16_t flags, float startTime, float blendAmount)
{
    CAnimBlendClumpData* data = GetAnimBlendClumpData(clump);
    SClumpAnimAssoc* animClumpAssoc = data->GetClumpAnimAssoc();
    if(animClumpAssoc)
    {
        animClumpAssoc->userPtr = &assoc->GetClumpAnimAssoc();
    }
    SClumpAnimAssoc& blendAssoc = assoc->GetClumpAnimAssoc();
    blendAssoc.userPtr = data;
    blendAssoc.next = data->GetClumpAnimAssoc();
    data->pFirstAnimAssoc = &assoc->GetClumpAnimAssoc();
    assoc->Start(startTime);
    blendAssoc.flags |= flags;
    blendAssoc.blendAmount = blendAmount;
}

void RpAnimBlendClumpSetBlendDeltas(RpClump* clump, uint16_t flags, float blendDelta)
{
    CAnimBlendClumpData* data = GetAnimBlendClumpData(clump);
    SClumpAnimAssoc* assoc = data->GetClumpAnimAssoc();
    while(assoc)
    {
        // Either no flags to match or flags must be explicitly set
        if(!flags || (assoc->flags & flags))
        {
            assoc->blendDelta = blendDelta;
        }
        assoc = assoc->next;
    }
}

void RpAnimBlendClumpRemoveAssociations(RpClump *pClump, uint16_t usFlags)
{
    CAnimBlendClumpData *pAnimData = PLUGIN_BLEND_CLUMP(pClump, pBlendClumpData);
    SClumpAnimAssoc *pAssoc = pAnimData->pFirstAnimAssoc;
    while(pAssoc)
    {
        CAnimBlendAssociation *pAnimAssoc = pAssoc[-1];
        if(!usFlags || (pAnimAssoc->ClumpAnimAssoc.usFlags & usFlags))
        {
            delete pAnimAssoc;
        }
        pAssoc = pAssoc->pNext;
    }
}

CAnimBlendAssociation* RpAnimBlendClumpGetAssociation(RpClump*pClump, const char *szName)
{
    CAnimBlendClumpData *pAnimData = PLUGIN_BLEND_CLUMP(pClump, pBlendClumpData);
    SClumpAnimAssoc *pAssoc = pAnimData->pFirstAnimAssoc;
    size_t uiNameHash = CKeyGen::GetUppercaseKey(szName);
    if(pAssoc == NULL)
    {   
        return NULL;
    }
    while(pAssoc && pAssoc->pAnimHierarchy->iHashKey != uiNameHash)
    {
        pAssoc = pAssoc->pNext;
    }
    return pAssoc[-1];
}

CAnimBlendAssociation *RpAnimBlendClumpGetAssociation(RpClump *pClump, AnimationId usAnimId)
{
    CAnimBlendClumpData *pAnimData = PLUGIN_BLEND_CLUMP(pClump, pBlendClumpData);
    SClumpAnimAssoc *pAssoc = pAnimData->pFirstAnimAssoc;
    if(pAssoc == NULL)
    {
        return NULL;
    }
    while(pAssoc && pAssoc->usAnimId != usAnimId)
    {
        pAssoc = pAssoc->pNext;
    }
    return pAssoc[-1];
}

CAnimBlendAssociation *RpAnimBlendClumpGetAssociation(RpClump *pClump, bool, CAnimBlendHierarchy *pAnimHierarchy)
{
    CAnimBlendClumpData *pAnimData = PLUGIN_BLEND_CLUMP(pClump, pBlendClumpData);
    SClumpAnimAssoc *pAssoc = pAnimData->pFirstAnimAssoc;
    if(pAssoc == NULL)
    {
        return NULL;
    }
    while(pAssoc && pAssoc->pAnimHierarchy != pAnimHierarchy)
    {
        pAssoc = pAssoc->pNext;
    }
    return pAssoc[-1];
}

static void* RpAnimBlendPluginConstructor(void* clump, RwInt32 offset, RwInt32 size)
{
    PLUGIN_ANIM_BLEND_CLUMP(clump, animBlendClumpData) = NULL;
    return clump;
}

static void* RpAnimBlendPluginDestructor(void* clump, RwInt32 offset, RwInt32 size)
{
    CAnimBlendClumpData* data = GetAnimBlendClumpData(static_cast<RpClump*>(clump));
    if(data)
    {
        RpAnimBlendClumpGiveAssociations(static_cast<RpClump*>(clump), NULL);
        delete data;
        data = NULL;
    }
    return clump;
}

static void* RpAnimBlendPluginCopyConstructor(void*, const void*, RwInt32, RwInt32)
{
    return NULL;
}

RwBool RpAnimBlendPluginAttach()
{
    gBlendClumpPlugin = RpClumpRegisterPlugin(sizeof(RpAnimBlendPluginDef), 
                                              MAKECHUNKID(rwVENDORID_ROCKSTAR, rwID_BLEND_CLUMP),
                                              RpAnimBlendPluginConstructor,
                                              RpAnimBlendPluginDestructor,
                                              RpAnimBlendPluginCopyConstructor);
    if(gBlendClumpPlugin == -1)
    {
        return false;
    }
    RtAnimInterpolatorInfo rtInfo;
    rtInfo.animKeyFrameSize = 0x1C;
    rtInfo.interpKeyFrameSize = 0x24;
    rtInfo.keyFrameAddCB = RpHAnimKeyFrameAdd;
    rtInfo.keyFrameApplyCB = RpHAnimKeyFrameApply;
    rtInfo.keyFrameBlendCB = RpHAnimKeyFrameBlend;
    rtInfo.keyFrameInterpolateCB = RpHAnimKeyFrameInterpolate;
    rtInfo.keyFrameMulRecipCB = RpHAnimKeyFrameMulRecip;
    rtInfo.keyFrameStreamGetSizeCB = RpHAnimKeyFrameStreamGetSize;
    rtInfo.keyFrameStreamReadCB = RpHAnimKeyFrameStreamRead;
    rtInfo.keyFrameStreamWriteCB = RpHAnimKeyFrameStreamWrite;
    rtInfo.typeID = 0;
    RtAnimRegisterInterpolationScheme(&rtInfo);
    return true;
}

CAnimBlendClumpData* RpAnimBlendAllocateData(RpClump* clump)
{
    PLUGIN_ANIM_BLEND_CLUMP(clump, animBlendClumpData) = new CAnimBlendClumpData;
    return GetAnimBlendClumpData(clump);
}

CAnimBlendAssociation* RpAnimBlendClumpExtractAssociations(RpClump *pClump)
{
    CAnimBlendClumpData *pData = PLUGIN_BLEND_CLUMP(pClump, pBlendClumpData);
    CAnimBlendAssociation *pAssoc = pData->pFirstAnimAssoc[-1];
    pData->pFirstAnimAssoc->pUserPtr = NULL;
    pData->pFirstAnimAssoc = NULL;
    return pAssoc;
}

typedef void (*AnimBlendFrameDataCallback)(AnimBlendFrameData*, void*);

static void RpAnimBlendClumpFillFrameCB(AnimBlendFrameData *pFrameData, void *pData)
{
    AnimBlendFrameData **ppFrames = (AnimBlendFrameData**)pData;
    ppFrames[CVisibilityPlugins::GetFrameHierarchyId(pFrameData->pFrame)] = pFrameData;
}

static void RpAnimBlendClumpFillClump(RpClump *pClump, AnimBlendFrameData **ppFrames)
{
    CAnimBlendClumpData *pFrameData = PLUGIN_BLEND_CLUMP(pClump, pBlendClumpData);
    RpHAnimHierarchy *pHAnimHier = GetAnimHierarchyFromSkinClump(pClump);
    for(size_t i = 1; i < 19 /*or 20*/; i++)
    {
        size_t boneTag = ConvertPedNode2BoneTag(i);
        ppFrames[i] = pFrameData->GetFrame(RpHAnimIDGetIndex(pHAnimHier, boneTag));
    }
}

void RpAnimBlendClumpFillFrameArray(RpClump *pClump, AnimBlendFrameData **ppFrames)
{
    CAnimBlendClumpData *pData = PLUGIN_BLEND_CLUMP(pClump, pBlendClumpData);
    RpAtomic *pFirst = GetFirstAtomic(pClump);
    if(pFirst && RpSkinGeometryGetSkin(pFirst->geometry))
    {
        RpAnimBlendClumpFillClump(pClump, ppFrames);
    }
    else
    {
        pData->ForAllFrames(RpAnimBlendClumpFillFrameCB, ppFrames);
    }
}

static AnimBlendFrameData* RpAnimBlendClumpFrame;

void RpAnimBlendClumpFindBoneCB(AnimBlendFrameData *pFrameData, void *pData)
{
    int node_id = *(int*)pData;
    if(pFrameData->node_id == node_id)
    {
        RpAnimBlendClumpFrame = pFrameData;
    }
}

AnimBlendFrameData *RpAnimBlendClumpFindBone(RpClump *pClump, unsigned int node_id)
{
    CAnimBlendClumpData *pData = PLUGIN_BLEND_CLUMP(pClump, pBlendClumpData);

    RpAnimBlendClumpFrame = 0;
    pData->ForAllFrames(RpAnimBlendClumpFindBoneCB, &node_id);
    return RpAnimBlendClumpFrame;
}

static void RpAnimBlendClumpFindClumpFrameCB(AnimBlendFrameData *pFrameData, void *pData)
{
    const char *name = (const char*)pData;
    const char *frame_node_name = ConvertBoneTag2BoneName(pFrameData->bone_tag);
    if(frame_node_name)
    {
        if(!stricmp(name, frame_node_name))
        {
            RpAnimBlendClumpFrame = pFrameData;
        }
    }
}

static void RpAnimBlendClumpFindAtomicFrameCB(AnimBlendFrameData *frame_data, void *data)
{
    const char *name = (const char*)data;
    const char *node_name = GetFrameNodeName(frame_data->pFrame);
    if(!stricmp(name, node_name))
    {
        RpAnimBlendClumpFrame = frame_data;
    }
}

AnimBlendFrameData* RpAnimBlendClumpFindFrame(RpClump *pClump, char const *name)
{
    CAnimBlendClumpData *pFrameData = PLUGIN_BLEND_CLUMP(pClump, pBlendClumpData);
    RpAtomic *pFirst = GetFirstAtomic(pClump);

    RpAnimBlendClumpFrame = 0;
    if(pFirst && RpSkinGeometryGetSkin(pFirst->geometry))
    {
        pFrameData->ForAllFrames(RpAnimBlendClumpFindClumpFrameCB, (void*)name);
    }
    else
    {
        pFrameData->ForAllFrames(RpAnimBlendClumpFindAtomicFrameCB, (void*)name);
    }
    return RpAnimBlendClumpFrame;
}

static void RpAnimBlendClumpFindClumpByHashCB(AnimBlendFrameData *frame_data, void *data)
{
    uint32_t hash = *(uint32_t*)data;
    const char *bone_name = ConvertBoneTag2BoneName(frame_data->bone_tag);
    if(bone_name)
    {
        uint32_t bone_hash = CKeyGen::GetUppercaseKey(bone_name);
        if(bone_hash == hash)
        {
            RpAnimBlendClumpFrame = frame_data;
        }
    }
}

static void RpAnimBlendClumpFindAtomicByHashCB(AnimBlendFrameData *frame_data, void *data)
{
    const char *node_name = GetFrameNodeName(frame_data->pFrame);
    uint32_t hash = *(uint32_t*)data;
    uint32_t node_hash = CKeyGen::GetUppercaseKey(node_name);
    if(hash == node_hash)
    {
        RpAnimBlendClumpFrame = frame_data;
    }
}

AnimBlendFrameData* RpAnimBlendClumpFindFrameFromHashKey(RpClump *pClump, unsigned int hash)
{
    CAnimBlendClumpData *pFrameData = PLUGIN_BLEND_CLUMP(pClump, pBlendClumpData);
    RpAtomic *pFirst = GetFirstAtomic(pClump);

    RpAnimBlendClumpFrame = 0;
    if(pFirst && RpSkinGeometryGetSkin(pFirst->geometry))
    {
        pFrameData->ForAllFrames(RpAnimBlendClumpFindClumpByHashCB, &hash);
    }
    else
    {
        pFrameData->ForAllFrames(RpAnimBlendClumpFindAtomicByHashCB, &hash);
    }
    return RpAnimBlendClumpFrame;
}

CAnimBlendAssociation* RpAnimBlendClumpGetFirstAssociation(RpClump *pClump)
{
    CAnimBlendClumpData *pFrameData = PLUGIN_BLEND_CLUMP(pClump, pBlendClumpData);

    if(RpAnimBlendClumpIsInitialized(pClump) && pFrameData->pFirstAnimAssoc)
    {
        return pFrameData->pFirstAnimAssoc[-1];
    }
    return NULL;
}

CAnimBlendAssociation* RpAnimBlendClumpGetFirstAssociation(RpClump *pClump, unsigned int flags)
{
    CAnimBlendClumpData *pFrameData = PLUGIN_BLEND_CLUMP(pClump, pBlendClumpData);
    if(!pFrameData)
    {
        return NULL;
    }
    SClumpAnimAssoc *pAssoc = pFrameData->pFirstAnimAssoc;
    do
    {
        if(pAssoc->usFlags & flags)
        {
            return pAssoc[-1];
        }
        pAssoc = pAssoc->pNext;
    } while(pAssoc);
    return NULL;
}

CAnimBlendAssociation* RpAnimBlendClumpGetMainAssociation(RpClump *pClump, CAnimBlendAssociation **pAssocs, float *fBlend)
{
    float fMinBlend;
    float fMaxBlend = 0.0;
    CAnimBlendAssociation *pBlendAssoc;
    SClumpAnimAssoc *pAssoc = PLUGIN_BLEND_CLUMP(pClump, pBlendClumpData)->pFirstAnimAssoc;
    do 
    {
        if(!(pAssoc->usFlags & 0x10))
        {
            if(pAssoc->fBlendAmount <= fMinBlend)
            {
                if(fMaxBlend < pAssoc->fBlendAmount)
                {
                    pBlendAssoc = pAssoc[-1];
                    fMaxBlend = pAssoc->fBlendAmount;
                }
            }
            else
            {
                fMaxBlend = fMinBlend;
                fMinBlend = pAssoc->fBlendAmount;
                pBlendAssoc = pAssoc[-1];
            }
        }
        pAssoc = pAssoc->pNext;
    } while(pAssoc);
    if(pAssocs)
    {
        pAssocs = &pBlendAssoc;
    }
    if(fBlend)
    {
        *fBlend = fMaxBlend;
    }
    return pBlendAssoc;
}

CAnimBlendAssociation* RpAnimBlendClumpGetMainAssociation_N(RpClump *pClump, int n)
{
    SClumpAnimAssoc *pAssoc = PLUGIN_BLEND_CLUMP(pClump, pBlendClumpData)->pFirstAnimAssoc;
    if(!pAssoc)
    {
        return NULL;
    }
    int i = 0;
    do 
    {
        if((pAssoc->usFlags & 0x10) && i == n)
        {
            return pAssoc[-1];
        }
        i++;
        pAssoc = pAssoc->pNext;
    } while(pAssoc);
    return NULL;
}

CAnimBlendAssociation* RpAnimBlendClumpGetMainPartialAssociation(RpClump *clump)
{
    SClumpAnimAssoc *assoc = PLUGIN_BLEND_CLUMP(clump, pBlendClumpData)->pFirstAnimAssoc;
    CAnimBlendAssociation *blend_assoc = NULL;
    float min_blend = 0.0f;
    
    do 
    {
        if((assoc->usFlags & 0x10) && min_blend < assoc->fBlendAmount)
        {
            blend_assoc = pAssoc[-1];
            min_blend = assoc->fBlendAmount;
        }
        assoc = assoc->pNext;
    } while(assoc);
    return blend_assoc;
}

CAnimBlendAssociation* RpAnimBlendClumpGetMainPartialAssociation_N(RpClump *clump, int n)
{
    SClumpAnimAssoc *assoc = PLUGIN_BLEND_CLUMP(clump, pBlendClumpData)->pFirstAnimAssoc;
    if(!assoc)
    {
        return NULL;
    }
    int i = 0;
    do
    {
        if(!(assoc->usFlags & 0x10) && i == n)
        {
            return assoc[-1];
        }
        assoc = assoc->pNext;
    } while(assoc);
    return NULL;
}

size_t RpAnimBlendClumpGetNumAssociations(RpClump *pClump)
{
    SClumpAnimAssoc *assoc = PLUGIN_BLEND_CLUMP(pClump, pBlendClumpData)->pFirstAnimAssoc;
    size_t count = 0;
    do
    {
        count++;
        assoc = assoc->pNext;
    } while(assoc);
    return count;
}

size_t RpAnimBlendClumpGetNumNonPartialAssociations(RpClump *pClump)
{
    SClumpAnimAssoc *assoc = PLUGIN_BLEND_CLUMP(pClump, pBlendClumpData)->pFirstAnimAssoc;
    size_t count = 0;
    do
    {
        if(!(assoc->usFlags & 0x10))
        {
            count++;
        }
        assoc = assoc->pNext;
    } while(assoc);
    return count;
}

size_t RpAnimBlendClumpGetNumPartialAssociations(RpClump *pClump)
{
    SClumpAnimAssoc *assoc = PLUGIN_BLEND_CLUMP(pClump, pBlendClumpData)->pFirstAnimAssoc;
    size_t count = 0;
    do
    {
        if(assoc->usFlags & 0x10)
        {
            count++;
        }
        assoc = assoc->pNext;
    } while(assoc);
    return count;
}