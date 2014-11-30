#pragma once

template<typename T>
T Clamp(const T& min, const T& a, const T& max)
{
    return a < min ? min : a > max ? max : a;
}

template<typename T>
T ClampMax(const T& a, const T& max)
{
    return a > max ? max : a;
}

template<typename T>
T ClampMin(const T& a, const T& min)
{
    return a < min ? min : a;
}

template<typename T>
void Exchange(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

RpHAnimHierarchy *GetAnimHierarchyFromSkinClump(RpClump *pClump);
RpAtomic *GetFirstAtomic(RpClump *pClump);
RpHAnimHierarchy *GetAnimHierarchyFromClump(RpClump *pClump);
RpHAnimHierarchy *GetAnimHierarchyFromFrame(RwFrame *pFrame);
void SetAmbientColours();
void DeActivateDirectional();
void SetLightsForNightVision();
void SetLightsForInfraredVisionDefaultObjects();
void SkinGetBonePositionsToTable(RpClump *pClump, RwV3d *pTable);
const char *ConvertBoneTag2BoneName(size_t bone_tag);
size_t ConvertPedNode2BoneTag(size_t pedNode);
void SetFrameNodeName(RwFrame *frame, char const *name);
const char *GetFrameNodeName(RwFrame *frame);
RwBool NodeNamePluginAttach();
void GetSlerpParams(const CQuaternion& first, const CQuaternion& second, float& theta0, float& theta1);
RwFrame* GetAtomicFrame(RpAtomic* atomic);
RwFrame* GetClumpFrame(RpClump* clump);
const float DotProduct(const RwV3d& first, const RwV3d& second);
RpAtomic *Get2DEffectAtomic(RpClump *clump);