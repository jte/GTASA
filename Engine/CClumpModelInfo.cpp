#include "StdInc.h"

void CClumpModelInfo::Init()
{
    m_AnimId = -1;
}

void CClumpModelInfo::Shutdown()
{
    CBaseModelInfo::Shutdown();
}

static RpAtomic *SetAtomicHAnimHierarchyCB(RpAtomic *pAtomic, void *pData)
{
    if(pData)
    {
        RpSkinAtomicSetHAnimHierarchy(pAtomic, (RpHAnimHierarchy*)pData);
        return NULL;
    }
    else
    {
        RpSkinAtomicSetHAnimHierarchy(pAtomic, GetAnimHierarchyFromFrame((RwFrame*)pAtomic->object.object.parent));
        return pAtomic;
    }
}

RpClump *CClumpModelInfo::CreateInstance()
{
    if (GetRwObject() == NULL)
    {
        return NULL;
    }
    AddRef();
    RpClump *pCloned = RpClumpClone((RpClump*)GetRwObject());
    RpAtomic *pFirst = GetFirstAtomic(pCloned);
    if (pFirst)
    {
        if(RpSkinGeometryGetSkin(pFirst->geometry) != NULL)
        {
            if(!dwUnknownFlag10)
            {
                RpHAnimHierarchy *pHAnim = GetAnimHierarchyFromClump(pCloned);
                RpClumpForAllAtomics(pCloned, SetAtomicHAnimHierarchyCB, pHAnim);
                RtAnimAnimation *pAnim = RpAnimBlendCreateAnimationForHierarchy(pHAnim);
                RtAnimInterpolatorSetCurrentAnim(pHAnim->currentAnim, pAnim);
                pHAnim->flags = rpHANIMHIERARCHYUPDATEMODELLINGMATRICES | rpHANIMHIERARCHYUPDATELTMS;
            }
        }
    }
    if (GetIsRoad())
    {
        RpAnimBlendClumpInit(pCloned);
        CAnimBlendHierarchy *pAnim = CAnimManager::GetAnimationByID(GetHashKey(), &CAnimManager::AnimBlocks[m_AnimId]);
        if (pAnim)
        {
            CAnimManager::BlendAnimation(pCloned, pAnim, 2, 1.0f);
        }
    }
    RemoveRef();
    return pCloned;
}

RpClump *CClumpModelInfo::CreateInstance(RwMatrixTag *pMatrix)
{
    if(GetRwObject() == NULL)
    {
        return NULL;
    }
    RpClump *pInst = CreateInstance();
    ((RwFrame*)pInst->object.parent)->modelling = *pMatrix;
    return pInst;
}

void CClumpModelInfo::SetAnimFile(char const *szAnimFile)
{
    if(stricmp(szAnimFile, "null") != 0)
    {
        m_AnimFile = new char[strlen(szAnimFile) + 1];
        strcpy(m_AnimFile, szAnimFile);
    }
}

void CClumpModelInfo::ConvertAnimFileIndex()
{
    if(m_AnimId != -1)
    {
        AnimationId animId = CAnimManager::GetAnimationBlockIndex(m_AnimFile);
        delete[] m_AnimFile;
        m_AnimId = animId;
    }
}

void CClumpModelInfo::DeleteRwObject()
{
    RpClump *pClump = (RpClump*)m_rwObject;
    if (pClump)
    {
        v3 = Get2DEffectAtomic(m_rwObject);
        if ( v3 )
        {
            v4 = *(_DWORD *)(g2dEffectPluginOffset + *(_DWORD *)(v3 + 24));
            if ( v4 )
            v5 = *(_DWORD *)v4;
            else
            LOBYTE(v5) = 0;
            pThis->uc2DFxCount -= v5;
        }
        RpClumpDestroy(pClump);
        m_rwObject = 0;
        RemoveTexDictionaryRef();
        size_t animIndex = GetAnimFileIndex();
        if (animIndex != -1)
        {
            CAnimManager::RemoveAnimBlockRef(animIndex);
        }
        if ( HIBYTE(pThis->flags) & 8 )
            LOBYTE(v1) = CBaseModelInfo__DeleteCollisionModel(pThis);
    }
}