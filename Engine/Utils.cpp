#include "StdInc.h"

static RpAtomic *GetAnimHierarchyFromSkinClumpCB(RpAtomic *pAtomic, void *pHier)
{
    *(RpHAnimHierarchy**)pHier = RpSkinAtomicGetHAnimHierarchy(pAtomic);
    return NULL;
}

RpHAnimHierarchy *GetAnimHierarchyFromSkinClump(RpClump *pClump)
{
    RpHAnimHierarchy *pHier = NULL;

    RpClumpForAllAtomics(pClump, GetAnimHierarchyFromSkinClumpCB, &pHier);
    return pHier;
}

static RpAtomic *GetFirstAtomicCB(RpAtomic *pAtomic, void *pFirst)
{
    *(RpAtomic**)pFirst = pAtomic;
    return NULL;
}

RpAtomic *GetFirstAtomic(RpClump *pClump)
{
    RpAtomic *pFirst;
    RpClumpForAllAtomics(pClump, GetFirstAtomicCB, &pFirst);
    return pFirst;
}

RpHAnimHierarchy *GetAnimHierarchyFromClump(RpClump *pClump)
{
    return GetAnimHierarchyFromFrame((RwFrame*)pClump->object.parent);
}

static RwFrame *GetAnimHierarchyFromFrameCB(RwFrame *pFrame, void *pHier)
{
    RpHAnimHierarchy *pLocalHier = RpHAnimFrameGetHierarchy(pFrame);
    if(pLocalHier)
    {
        *(RpHAnimHierarchy**)pHier = pLocalHier;
        return NULL;
    }
    RwFrameForAllChildren(pFrame, GetAnimHierarchyFromFrameCB, pHier);
    return pFrame;
}

RpHAnimHierarchy *GetAnimHierarchyFromFrame(RwFrame *pFrame)
{
    RpHAnimHierarchy *pHier = RpHAnimFrameGetHierarchy(pFrame);
    if(pHier)
    {
        return pHier;
    }
    else
    {
        RwFrameForAllChildren(pFrame, GetAnimHierarchyFromFrameCB, &pHier);
    }
    if(!pHier)
    {
        RwFrameForAllChildren(pFrame, GetAnimHierarchyFromFrameCB, &pHier);
    }
    return pHier;
}

RpLight *g_AmbientWorldLight;
RwRGBAReal AmbientLightColourForFrame;

void SetAmbientColours()
{
    RpLightSetColor(g_AmbientWorldLight, &AmbientLightColourForFrame);
}

RpLight *g_DirectWorldLight;

void DeActivateDirectional()
{
    g_DirectWorldLight->object.object.flags = 0;
}

void SetLightsForNightVision()
{
    RwRGBAReal night;
    night.red = 0.0f;
    night.green = 1.0f;
    night.blue = 0.0f;
    night.alpha = 1.0f;
    RpLightSetColor(g_AmbientWorldLight, &night);
    RpLightSetColor(g_DirectWorldLight, &night);
}

void SetLightsForInfraredVisionDefaultObjects()
{
    RwRGBAReal blue;
    blue.red = 0.0f;
    blue.green = 0.0f;
    blue.blue = 1.0f;
    blue.alpha = 1.0f;
    RpLightSetColor(g_AmbientWorldLight, &blue);
    RpLightSetColor(g_DirectWorldLight, &blue);
}

void SkinGetBonePositionsToTable(RpClump *pClump, RwV3d *pTable)
{
    if(!pTable)
    {
        return;
    }
    // recast as it should have been
    RpAtomic *pFirst = GetFirstAtomic(pClump);
    RpSkin *pSkin = RpSkinGeometryGetSkin(pFirst->geometry);
    RpHAnimHierarchy *pHAnimHier = GetAnimHierarchyFromSkinClump(pClump);
    pTable[0].x = 0.0f;
    pTable[0].y = 0.0f;
    pTable[0].z = 0.0f;
    const RwMatrix *skinToBone = RpSkinGetSkinToBoneMatrices(pSkin);
    size_t j = 0;
    size_t decision = 0;
    size_t stack[64];
    size_t *sp = stack;
    for(size_t i = 1; i < RpSkinGetNumBones(pSkin); i++)
    {
        const RwMatrix& bone = skinToBone[i];
        RwMatrix boneInverted;
        RwMatrixInvert(&boneInverted, &bone);
        const RwMatrix& transformBone = skinToBone[decision];
        RwV3dTransformPoints(&pTable[i], &boneInverted.pos, 1, &transformBone);
        size_t node_flags = pHAnimHier->pNodeInfo[i].flags;
        if(node_flags & rpHANIMHIERARCHYNOMATRICES)
        { // This hierarchy has no local matrices
            sp++;           // push_stack operation
            *sp = decision;
        }
        if(node_flags & rpHANIMHIERARCHYSUBHIERARCHY)
        { // This hierarchy is a sub-hierarchy
            decision = *sp; // pop_stack operation
            sp--;
        }
        else
        { 
            // empty stack
            decision = i;
        }
    }
}

const char *ConvertBoneTag2BoneName(size_t bone_tag)
{
    switch(bone_tag)
    {
        case 0: return "Root";
        case 1: return "Pelvis";
        case 2: return "Spine";
        case 3: return "Spine1";
        case 4: return "Neck";
        case 5: return "Head";
        case 6: return "L Brow";
        case 7: return "R Brow";
        case 8: return "Jaw";
        case 21: return "Bip01 R Clavicle";
        case 22: return "R UpperArm";
        case 23: return "R Forearm";
        case 24: return "R Hand";
        case 25: return "R Fingers";
        case 26: return "R Finger01";
        case 31: return "Bip01 L Clavicle";
        case 32: return "L UpperArm";
        case 33: return "L Forearm";
        case 34: return "L Hand";
        case 35: return "L Fingers";
        case 36: return "L Finger01";
        case 41: return "L Thigh";
        case 42: return "L Calf";
        case 43: return "L Foot";
        case 44: return "L Toe";
        case 51: return "R Thigh";
        case 52: return "R Calf";
        case 53: return "R Foot";
        case 54: return "R Toe";
        case 201: return "Belly";
        case 301: return "R Breast";
        case 302: return "L Breast";
        default: return NULL;
    }
}

size_t ConvertPedNode2BoneTag(size_t pedNode)
{
    switch(pedNode)
    {
        case 1: return 3;
        case 2: return 5;
        case 3: return 32;
        case 4: return 22;
        case 5: return 34;
        case 6: return 24;
        case 7: return 41;
        case 8: return 51;
        case 9: return 43;
        case 10:return 53;
        case 12:return 42;
        case 11:return 52;
        case 13:return 33;
        case 14:return 23;
        case 15:return 31;
        case 16:return 21;
        case 17:return 4;
        case 18:return 8;
        default:return -1;
    }
}

#define rwID_NODENAME (0xFE)

#define PLUGIN_NODE_NAME(frame, var) \
    (RWPLUGINOFFSET(NodeNamePluginDef, frame, gNodeNamePlugin)->var)

struct NodeNamePluginDef
{
    char name[24];
};

RwInt32 gNodeNamePlugin = -1;

void SetFrameNodeName(RwFrame *frame, char const *name)
{
    if(gNodeNamePlugin > 0)
    {
        strncpy(PLUGIN_NODE_NAME(frame, name), name, sizeof(PLUGIN_NODE_NAME(frame, name))-1);
        PLUGIN_NODE_NAME(frame, name)[sizeof(PLUGIN_NODE_NAME(frame, name))-1] = '\0';
    }
}

const char *GetFrameNodeName(RwFrame *frame)
{
    return gNodeNamePlugin > 0 ? PLUGIN_NODE_NAME(frame, name) : NULL;
}

void *NodeNamePluginConstructor(void *frame, RwInt32 offset, RwInt32 size)
{
    PLUGIN_NODE_NAME(frame, name)[0] = '\0';
    return frame;
}

void *NodeNamePluginDestructor(void *frame, RwInt32 offset, RwInt32 size)
{
    return frame;
}

void *NodeNamePluginCopyConstructor(void *dstFrame, const void *srcFrame, RwInt32 offset, RwInt32 size)
{
    strncpy(PLUGIN_NODE_NAME(dstFrame, name), PLUGIN_NODE_NAME(srcFrame, name), sizeof(PLUGIN_NODE_NAME(dstFrame, name)));
    return dstFrame;
}

static RwInt32 NodeNamePluginGetSize(const void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
    const char* name = PLUGIN_NODE_NAME(object, name);
    return name ? strlen(name) : 0;
}

static RwStream* NodeNamePluginRead(RwStream *stream, RwInt32 binaryLength, void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
    RwStreamRead(stream, PLUGIN_NODE_NAME(object, name), binaryLength);
    PLUGIN_NODE_NAME(object, name)[binaryLength] = '\0';
    return stream;
}

static RwStream* NodeNamePluginWrite(RwStream *stream, RwInt32 binaryLength, const void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
    RwStreamWrite(stream, PLUGIN_NODE_NAME(object, name), binaryLength);
    return stream;
}

RwBool NodeNamePluginAttach()
{
    gNodeNamePlugin = RwFrameRegisterPlugin(sizeof(NodeNamePluginDef),
                                            MAKECHUNKID(rwVENDORID_ROCKSTAR, rwID_NODENAME),
                                            NodeNamePluginConstructor,
                                            NodeNamePluginDestructor,
                                            NodeNamePluginCopyConstructor);

    RwFrameRegisterPluginStream(MAKECHUNKID(rwVENDORID_ROCKSTAR, rwID_NODENAME),
                                NodeNamePluginRead,
                                NodeNamePluginWrite,
                                NodeNamePluginGetSize);
    return gNodeNamePlugin != -1;
}

void GetSlerpParams(const CQuaternion& first, const CQuaternion& second, float& theta0, float& theta1)
{
    const float dot = ClampMax(DotProduct(first, second), 1.0f);
    theta0 = acos(dot);
    theta1 = theta0 == 0.0f ? 0.0f : (1.0f / sin(theta0));
}

RwFrame* GetAtomicFrame(RpAtomic* atomic)
{
    return reinterpret_cast<RwFrame*>(atomic->object.object.parent);
}

RwFrame* GetClumpFrame(RpClump* clump)
{
    return reinterpret_cast<RwFrame*>(clump->object.parent);
}

const float DotProduct(const RwV3d& first, const RwV3d& second)
{
    return first.x * second.x + first.y * second.y + first.z * second.z;
}

RpAtomic *Get2DEffectAtomicCallback(RpAtomic *atomic, void *data)
{
    DWORD* v3 = *(DWORD*)atomic->geometry + g2dEffectPluginOffset)
  v3 = *(_DWORD *)(g2dEffectPluginOffset + *(_DWORD *)(atomic + 24));
  if ( v3 )
  {
    if ( *(_DWORD *)v3 )
    {
      *(_DWORD *)data = atomic;
      result = 0;
    }
  }
  return atomic;
}

RpAtomic *Get2DEffectAtomic(RpClump *clump)
{
    RpAtomic *atomic = NULL;
    RpClumpForAllAtomics(clump, Get2DEffectAtomicCallback, &atomic);
    return atomic;
}