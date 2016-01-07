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

template<typename T>
T Max(const T& a, const T& b)
{
    return a > b ? a : b;
}

template<typename T>
T Min(const T& a, const T& b)
{
    return a < b ? a : b;
}

template<typename T>
T Lerp (const T& from, float weight, const T& to)
{
    return (to - from) * weight + from;
}


template<typename T>
T Unlerp (const T& from, const T& pos, const T& to)
{
    return (pos - from) / (to - from);
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
RpAtomic *Get2DEffectAtomic(RpClump *clump);
RwObject* GetFirstObject(RwFrame* frame);
RwFrame* GetFirstChild(RwFrame* frame);
void SetLightsWithTimeOfDayColour();
bool ReadLine(int file, char* line, int size);
CVector Multiply3x3(const CMatrix& matrix, const CVector& vector);
const char* GxtCharToAscii(unsigned short* gxtString, uint8_t offset);
void AsciiToGxtChar(char const* asciiString, uint16_t* gxtString);
const char *__cdecl GetNameAndDamage(const char *a1, char *a2, int a3);