#include "StdInc.h"

#define rwID_BLEND_CLUMP (0xFB)

#define PLUGIN_ANIM_BLEND_CLUMP(clump) \
    (RWPLUGINOFFSET(CAnimBlendClumpData, clump, gBlendClumpPlugin))

static RwInt32 gBlendClumpPlugin = -1;


CAnimBlendClumpData* GetAnimBlendClumpData(RpClump* clump)
{
    return PLUGIN_ANIM_BLEND_CLUMP(clump);
}

CAnimBlendAssociation* ClumpAssocToBlendAssoc(SClumpAnimAssoc* assoc)
{
    return (CAnimBlendAssociation*)(((char*)assoc) - 4);
}

static void RpAnimBlendClumpInitAtomicCB(AnimBlendFrameData* frameData, void*)
{
    frameData->flags = 0;
    frameData->pos = frameData->frame->modelling.pos;
    frameData->bone_tag = -1;
}

static RwFrame* RpAnimBlendClumpGetAtomicBoneCountCB(RwFrame* frame, void* data)
{
    size_t* numBones = (size_t*)data;
    (*numBones)++;
    RwFrameForAllChildren(frame, RpAnimBlendClumpGetAtomicBoneCountCB, data);
    return frame;
}

static RwFrame* RpAnimBlendClumpSetAtomicFrameDataCB(RwFrame* frame, void* data)
{
    AnimBlendFrameData** frames = (AnimBlendFrameData**)data;
    (*frames)->frame = frame;
    (*frames)++;
    RwFrameForAllChildren(frame, RpAnimBlendClumpSetAtomicFrameDataCB, data);
    return frame;
}

static void RpAnimBlendClumpInitAtomic(RpClump* clump)
{
    RpAnimBlendAllocateData(clump);
    size_t numBones = 0;
    RwFrameForAllChildren(RwFrameGetParent(clump), RpAnimBlendClumpGetAtomicBoneCountCB, &numBones);
    CAnimBlendClumpData* clumpData = GetAnimBlendClumpData(clump);
    clumpData->SetNumberOfBones(numBones);
    RwFrameForAllChildren(RwFrameGetParent(clump), RpAnimBlendClumpSetAtomicFrameDataCB, clumpData->GetFrames());
    clumpData->ForAllFrames(RpAnimBlendClumpInitAtomicCB, NULL);
    clumpData->GetFrame(0)->flags |= ROOT_FRAME;
}

static void RpAnimBlendClumpInitClump(RpClump* clump);

void RpAnimBlendClumpInit(RpClump* clump)
{
    RpAtomic* atomic = GetFirstAtomic(clump);
    if (atomic && RpSkinGeometryGetSkin(atomic->geometry))
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

void RpAnimBlendClumpInitClump(RpClump* clump)
{
    RpAtomic* atomic = GetFirstAtomic(clump);
    RpSkin* skin = RpSkinGeometryGetSkin(atomic->geometry);
    uint32_t boneCount = RpSkinGetNumBones(skin);
    CAnimBlendClumpData* animBlendClumpData = GetAnimBlendClumpData(clump);
    animBlendClumpData->SetNumberOfBones(boneCount);
    RpHAnimHierarchy* animHierarchy = GetAnimHierarchyFromSkinClump(clump);
    RwV3d boneTable[63];
    SkinGetBonePositionsToTable(clump, boneTable);
    for(size_t i = 0; i < boneCount; i++)
    {
        AnimBlendFrameData *pFrame = animBlendClumpData->GetFrame(i);
        pFrame->frame = (RwFrame*)rtANIMGETINTERPFRAME(animHierarchy->currentAnim, i);
        pFrame->node = &animHierarchy->pNodeInfo[i];
        pFrame->pos = boneTable[i];
    }
    // Initialize frames
    animBlendClumpData->ForAllFrames(RpAnimBlendClumpInitFrameCB, NULL);
    // Mark root frame
    animBlendClumpData->GetFrame(0)->flags |= ROOT_FRAME;
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
        animClumpAssoc->userPtr = (void*)&assoc->GetClumpAnimAssoc();
    }
    SClumpAnimAssoc& blendAssoc = assoc->GetClumpAnimAssoc();
    blendAssoc.userPtr = data;
    blendAssoc.next = data->GetClumpAnimAssoc();
    data->SetClumpAnimAssoc(&assoc->GetClumpAnimAssoc());
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
    CAnimBlendClumpData *pAnimData = GetAnimBlendClumpData(pClump);
    SClumpAnimAssoc *pAssoc = pAnimData->GetClumpAnimAssoc();
    while (pAssoc)
    {
        CAnimBlendAssociation *pAnimAssoc = ClumpAssocToBlendAssoc(pAssoc);
        if (!usFlags || (pAnimAssoc->GetClumpAnimAssoc().flags & usFlags))
        {
            delete pAnimAssoc;
        }
        pAssoc = pAssoc->next;
    }
}

CAnimBlendAssociation* RpAnimBlendClumpGetAssociation(RpClump *clump, const char *name)
{
    CAnimBlendClumpData *anim_data = GetAnimBlendClumpData(clump);
    SClumpAnimAssoc *assoc = anim_data->GetClumpAnimAssoc();
    size_t hash = CKeyGen::GetUppercaseKey(name);
    if (assoc == NULL)
    {   
        return NULL;
    }
    while (assoc && assoc->animHierarchy->GetHashKey() != hash)
    {
        assoc = assoc->next;
    }
    return ClumpAssocToBlendAssoc(assoc);
}

CAnimBlendAssociation *RpAnimBlendClumpGetAssociation(RpClump *clump, AnimationId anim_id)
{
    CAnimBlendClumpData *anim_data = GetAnimBlendClumpData(clump);
    SClumpAnimAssoc *assoc = anim_data->GetClumpAnimAssoc();
    if (assoc == NULL)
    {
        return NULL;
    }
    while (assoc && assoc->animId != anim_id)
    {
        assoc = assoc->next;
    }
    return ClumpAssocToBlendAssoc(assoc);
}

CAnimBlendAssociation *RpAnimBlendClumpGetAssociation(RpClump *clump, bool, CAnimBlendHierarchy *anim_hier)
{
    CAnimBlendClumpData *anim_data = GetAnimBlendClumpData(clump);
    SClumpAnimAssoc *assoc = anim_data->GetClumpAnimAssoc();
    if (assoc == NULL)
    {
        return NULL;
    }
    while (assoc && assoc->animHierarchy != anim_hier)
    {
        assoc = assoc->next;
    }
    return ClumpAssocToBlendAssoc(assoc);
}

static void *RpAnimBlendPluginConstructor(void *clump, RwInt32 offset, RwInt32 size)
{
    PLUGIN_ANIM_BLEND_CLUMP(clump, data) = NULL;
    return clump;
}

static void *RpAnimBlendPluginDestructor(void *clump, RwInt32 offset, RwInt32 size)
{
    CAnimBlendClumpData *data = GetAnimBlendClumpData(static_cast<RpClump*>(clump));
    if (data)
    {
        RpAnimBlendClumpGiveAssociations(static_cast<RpClump*>(clump));
        delete data;
        data = NULL;
    }
    return clump;
}

void RpAnimBlendClumpGiveAssociations(RpClump *clump)
{
    CAnimBlendClumpData *data = GetAnimBlendClumpData(clump);
    SClumpAnimAssoc *assoc = data->GetClumpAnimAssoc();
    if (assoc)
    {
        do
        {
            if (assoc != (SClumpAnimAssoc*)4)
            {
                CAnimBlendAssociation *blend_assoc = ClumpAssocToBlendAssoc(assoc);
                blend_assoc->~CAnimBlendAssociation(); /* do we need delete here?*/
            }
            assoc = assoc->next;
        } while (assoc);
    }
}

static void *RpAnimBlendPluginCopyConstructor(void*, const void*, RwInt32, RwInt32)
{
    return NULL;
}

RwBool RpAnimBlendPluginAttach()
{
    gBlendClumpPlugin = RpClumpRegisterPlugin(sizeof(CAnimBlendClumpData*), 
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

CAnimBlendClumpData* RpAnimBlendAllocateData(RpClump *clump)
{
    PLUGIN_ANIM_BLEND_CLUMP(clump, data) = new CAnimBlendClumpData;
    return GetAnimBlendClumpData(clump);
}

CAnimBlendAssociation* RpAnimBlendClumpExtractAssociations(RpClump *clump)
{
    CAnimBlendClumpData *data = GetAnimBlendClumpData(clump);
    SClumpAnimAssoc *clump_assoc = data->GetClumpAnimAssoc();
    CAnimBlendAssociation *assoc = ClumpAssocToBlendAssoc(clump_assoc);
    clump_assoc->userPtr = NULL;
    data->SetClumpAnimAssoc(NULL);
    return assoc;
}

typedef void (*AnimBlendFrameDataCallback)(AnimBlendFrameData*, void*);

static void RpAnimBlendClumpFillFrameCB(AnimBlendFrameData *frame_data, void *data)
{
    AnimBlendFrameData **ppFrames = (AnimBlendFrameData**)data;
    ppFrames[CVisibilityPlugins::GetFrameHierarchyId(frame_data->frame)] = frame_data;
}

static void RpAnimBlendClumpFillClump(RpClump *clump, AnimBlendFrameData **frames)
{
    CAnimBlendClumpData *frame_data = GetAnimBlendClumpData(clump);
    RpHAnimHierarchy *hanim_hier = GetAnimHierarchyFromSkinClump(clump);
    for(size_t i = 1; i < 19; i++)
    {
        size_t bone_tag = ConvertPedNode2BoneTag(i);
        frames[i] = frame_data->GetFrame(RpHAnimIDGetIndex(hanim_hier, bone_tag));
    }
}

void RpAnimBlendClumpFillFrameArray(RpClump *clump, AnimBlendFrameData **frames)
{
    CAnimBlendClumpData *data = GetAnimBlendClumpData(clump);
    RpAtomic *first = GetFirstAtomic(clump);
    if(first && RpSkinGeometryGetSkin(first->geometry))
    {
        RpAnimBlendClumpFillClump(clump, frames);
    }
    else
    {
        data->ForAllFrames(RpAnimBlendClumpFillFrameCB, frames);
    }
}

static AnimBlendFrameData* RpAnimBlendClumpFrame;

void RpAnimBlendClumpFindBoneCB(AnimBlendFrameData *frame_data, void *data)
{
    int node_id = *(int*)data;
    if(frame_data->node->nodeID == node_id)
    {
        RpAnimBlendClumpFrame = frame_data;
    }
}

AnimBlendFrameData *RpAnimBlendClumpFindBone(RpClump *clump, unsigned int node_id)
{
    CAnimBlendClumpData *data = GetAnimBlendClumpData(clump);

    RpAnimBlendClumpFrame = NULL;
    data->ForAllFrames(RpAnimBlendClumpFindBoneCB, &node_id);
    return RpAnimBlendClumpFrame;
}

static void RpAnimBlendClumpFindClumpFrameCB(AnimBlendFrameData *frame_data, void *data)
{
    const char *name = (const char*)data;
    const char *frame_node_name = ConvertBoneTag2BoneName(frame_data->bone_tag);
    if (frame_node_name)
    {
        if (!stricmp(name, frame_node_name))
        {
            RpAnimBlendClumpFrame = frame_data;
        }
    }
}

static void RpAnimBlendClumpFindAtomicFrameCB(AnimBlendFrameData *frame_data, void *data)
{
    const char *name = (const char*)data;
    const char *node_name = GetFrameNodeName(frame_data->frame);
    if (!stricmp(name, node_name))
    {
        RpAnimBlendClumpFrame = frame_data;
    }
}

AnimBlendFrameData* RpAnimBlendClumpFindFrame(RpClump *clump, char const *name)
{
    CAnimBlendClumpData *frame_data = GetAnimBlendClumpData(clump);
    RpAtomic *first = GetFirstAtomic(clump);

    RpAnimBlendClumpFrame = 0;
    if(first && RpSkinGeometryGetSkin(first->geometry))
    {
        frame_data->ForAllFrames(RpAnimBlendClumpFindClumpFrameCB, (void*)name);
    }
    else
    {
        frame_data->ForAllFrames(RpAnimBlendClumpFindAtomicFrameCB, (void*)name);
    }
    return RpAnimBlendClumpFrame;
}

static void RpAnimBlendClumpFindClumpByHashCB(AnimBlendFrameData *frame_data, void *data)
{
    uint32_t hash = *(uint32_t*)data;
    const char *bone_name = ConvertBoneTag2BoneName(frame_data->bone_tag);
    if (bone_name)
    {
        uint32_t bone_hash = CKeyGen::GetUppercaseKey(bone_name);
        if (bone_hash == hash)
        {
            RpAnimBlendClumpFrame = frame_data;
        }
    }
}

static void RpAnimBlendClumpFindAtomicByHashCB(AnimBlendFrameData *frame_data, void *data)
{
    const char *node_name = GetFrameNodeName(frame_data->frame);
    uint32_t hash = *(uint32_t*)data;
    uint32_t node_hash = CKeyGen::GetUppercaseKey(node_name);
    if (hash == node_hash)
    {
        RpAnimBlendClumpFrame = frame_data;
    }
}

AnimBlendFrameData* RpAnimBlendClumpFindFrameFromHashKey(RpClump *clump, unsigned int hash)
{
    CAnimBlendClumpData *frame_data = GetAnimBlendClumpData(clump);
    RpAtomic *first = GetFirstAtomic(clump);

    RpAnimBlendClumpFrame = 0;
    if (first && RpSkinGeometryGetSkin(first->geometry))
    {
        frame_data->ForAllFrames(RpAnimBlendClumpFindClumpByHashCB, &hash);
    }
    else
    {
        frame_data->ForAllFrames(RpAnimBlendClumpFindAtomicByHashCB, &hash);
    }
    return RpAnimBlendClumpFrame;
}

CAnimBlendAssociation* RpAnimBlendClumpGetFirstAssociation(RpClump *clump)
{
    CAnimBlendClumpData *frame_data = GetAnimBlendClumpData(clump);

    if (RpAnimBlendClumpIsInitialized(clump) && frame_data->GetClumpAnimAssoc())
    {
        return ClumpAssocToBlendAssoc(frame_data->GetClumpAnimAssoc());
    }
    return NULL;
}

CAnimBlendAssociation* RpAnimBlendClumpGetFirstAssociation(RpClump *clump, unsigned int flags)
{
    CAnimBlendClumpData *frame_data = GetAnimBlendClumpData(clump);
    if (!frame_data)
    {
        return NULL;
    }
    SClumpAnimAssoc *assoc = frame_data->GetClumpAnimAssoc();
    do
    {
        if(assoc->flags & flags)
        {
            return ClumpAssocToBlendAssoc(assoc);
        }
        assoc = assoc->next;
    } while (assoc);
    return NULL;
}

CAnimBlendAssociation* RpAnimBlendClumpGetMainAssociation(RpClump *clump, CAnimBlendAssociation **assocs, float *blend)
{
    float min_blend = 0.0f; // cheeck init values
    float max_blend = 0.0f;
    CAnimBlendAssociation *blend_assoc;
    SClumpAnimAssoc *clump_assoc = GetAnimBlendClumpData(clump)->GetClumpAnimAssoc();
    do 
    {
        if (!(clump_assoc->flags & 0x10))
        {
            if (clump_assoc->blendAmount <= min_blend)
            {
                if (clump_assoc->blendAmount > max_blend)
                {
                    blend_assoc = ClumpAssocToBlendAssoc(clump_assoc);
                    max_blend = clump_assoc->blendAmount;
                }
            }
            else
            {
                max_blend = min_blend;
                min_blend = clump_assoc->blendAmount;
                blend_assoc = ClumpAssocToBlendAssoc(clump_assoc);
            }
        }
        clump_assoc = clump_assoc->next;
    } while (clump_assoc);
    if (assocs)
    {
        assocs = &blend_assoc;
    }
    if (blend)
    {
        *blend = max_blend;
    }
    return blend_assoc;
}

CAnimBlendAssociation *RpAnimBlendClumpGetMainAssociation_N(RpClump *clump, int n)
{
    SClumpAnimAssoc *assoc = GetAnimBlendClumpData(clump)->GetClumpAnimAssoc();
    if (!assoc)
    {
        return NULL;
    }
    for (size_t i = 0; assoc; assoc = assoc->next, i++)
    {
        if ((assoc->flags & 0x10) && i == n)
        {
            return ClumpAssocToBlendAssoc(assoc);
        }
    }
    return NULL;
}

CAnimBlendAssociation *RpAnimBlendClumpGetMainPartialAssociation(RpClump *clump)
{
    SClumpAnimAssoc *assoc = GetAnimBlendClumpData(clump)->GetClumpAnimAssoc();
    CAnimBlendAssociation *blend_assoc = NULL;
    float min_blend = 0.0f;
    
    for (; assoc; assoc = assoc->next)
    {
        if ((assoc->flags & 0x10) && min_blend < assoc->blendAmount)
        {
            blend_assoc = ClumpAssocToBlendAssoc(assoc);
            min_blend = assoc->blendAmount;
        }
    }
    return blend_assoc;
}

CAnimBlendAssociation *RpAnimBlendClumpGetMainPartialAssociation_N(RpClump *clump, int n)
{
    SClumpAnimAssoc *assoc = GetAnimBlendClumpData(clump)->GetClumpAnimAssoc();
    if (!assoc)
    {
        return NULL;
    }
    for (size_t i = 0; assoc; assoc = assoc->next, i++)
    {
        if (!(assoc->flags & 0x10) && i == n)
        {
            return ClumpAssocToBlendAssoc(assoc);
        }
    }
    return NULL;
}

size_t RpAnimBlendClumpGetNumAssociations(RpClump *clump)
{
    SClumpAnimAssoc *assoc = GetAnimBlendClumpData(clump)->GetClumpAnimAssoc();
    size_t count;
    for (count = 0; assoc; assoc = assoc->next)
    {
        count++;
    }
    return count;
}

size_t RpAnimBlendClumpGetNumNonPartialAssociations(RpClump *clump)
{
    SClumpAnimAssoc *assoc = GetAnimBlendClumpData(clump)->GetClumpAnimAssoc();
    size_t count;
    for (count = 0; assoc; assoc = assoc->next)
    {
        if (!(assoc->flags & 0x10))
        {
            count++;
        }
    }
    return count;
}

size_t RpAnimBlendClumpGetNumPartialAssociations(RpClump *clump)
{
    SClumpAnimAssoc *assoc = GetAnimBlendClumpData(clump)->GetClumpAnimAssoc();
    size_t count;
    for (count = 0; assoc; assoc = assoc->next)
    {
        if (assoc->flags & 0x10)
        {
            count++;
        }
    }
    return count;
}