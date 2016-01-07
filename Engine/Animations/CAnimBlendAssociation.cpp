#include "StdInc.h"

CAnimBlendAssociation::CAnimBlendAssociation(CAnimBlendAssociation& assoc)
{
    SetCurrentTime(assoc.m_assoc.currentTime / assoc.m_assoc.fTotalTime * m_assoc.animHierarchy->m_totalTime);
}

void CAnimBlendAssociation::Init(RpClump *pClump, CAnimBlendHierarchy *pHier)
{
    size_t uiAnimBlendNodeCount = PLUGIN_BLEND_CLUMP(pClump)->uiAnimBlendNodeCount;
    AllocateBlendNodes(uiAnimBlendNodeCount);
    for(size_t i = 0; i < uiAnimBlendNodeCount; i++)
    {   
        pAnimBlendNodeArray[i]->pAnimBlendAssociation = this;
    }
    pAnimHierarchy = pHier;
    for(size_t i = 0; i < pAnimHierarchy->usNumSequences; i++)
    {
        CAnimBlendSequence *pSequence = pAnimHierarchy[i];
        size_t bone = 0;
        if(pHier->usFlags & 0x10)
        {
            bone = RpClumpFindBoneByID(pClump, pSequence->info.iBoneID);
        }
        else
        {
            bone = RpClumpFindBoneByHash(pClump, pSequence->info.iHashKey);
        }
        if(bone && pSequence->usNumKeyFrames)
        {
            #TODO#
        }
    }
}

CAnimBlendAssociation::CAnimBlendAssociation(CAnimBlendStaticAssociation &pStaticAssoc)
{
    pAnimBlendNodeArray = NULL;
    fBlendAmount = 1.0;
    fBlendDelta = 0.0;
    fCurrentTime = 0.0;
    fSpeed = 1.0;
    fTimeStep = 0.0;
    uiCallbackType = 0;
    field_4 = 0;
    numAnimations = 0;
    Init(pStaticAssoc);
    ReferenceAnimBlock();
}

void CAnimBlendAssociation::Init(CAnimBlendStaticAssociation &pStaticAssoc)
{
    pAnimHierarchy = pStaticAssoc->pAnimHierarchy;
    uiAnimBlendNodeCount = pStaticAssoc->dwNumBones;
    usFlags = pStaticAssoc->usFlags;
    usAnimID = pStaticAssoc->usAnimID;
    usAnimGroup = pStaticAssoc->usAnimGroup;
    AllocateBlendNodes(uiAnimBlendNodeCount);
    for(size_t i = 0; i < uiAnimBlendNodeCount; i++)
    {
        pAnimBlendNodeArray[i]->pAnimBlendSequence = pStaticAssoc->pSequences[i];
        pAnimBlendNodeArray[i]->pAnimBlendAssociation = this;
    }
}

void CAnimBlendAssociation::ReferenceAnimBlock()
{
    if(!(m_usFlags & 0x100000))
    {
        CAnimManager::AddAnimBlockRef(m_pAnimHierarchy->iAnimBlockID);
        m_usFlags |= 0x4000;
    }
}

void CAnimBlendAssociation::SetCurrentTime(float fCurrentTime)
{
    m_fCurrentTime = fCurrentTime;
    if(fCurrentTime > m_pAnimHierarchy->fTotalTime)
    {
        if(m_usFlags & 0x02)
        {
            while(m_pAnimHierarchy->fTotalTime <= m_fCurrentTime)
            {
                m_fCurrentTime -= m_pAnimHierarchy->fTotalTime;
            }
        }
        else
        {
            m_fCurrentTime = m_pAnimHierarchy->fTotalTime;
        }
    }
    m_pAnimHierarchy->Uncompress();
    for(size_t i = 0; i < m_uiAnimBlendNodeCount; i++)
    {
        if(m_pAnimBlendNodeArray[i]->pAnimBlendSequence)
        {
            if(bRunningCompressed)
            {
                m_pAnimBlendNodeArray[i]->CalcDeltasCompressed();
            }
            else
            {
                m_pAnimBlendNodeArray[i]->CalcDeltas();
            }
        }
    }
}

void CAnimBlendAssociation::SetDeleteCallback(Callback pfnDeleteCallback, void *pUserData)
{
    m_uiCallbackType = DeleteCallbackType;
    m_pCallback = pfnDeleteCallback;
    m_pCallbackArgument = pUserData;
}

void CAnimBlendAssociation::SetFinishCallback(Callback pfnFinishCallback, void *pUserData)
{
    m_uiCallbackType = FinishCallbackType;
    m_pCallback = pfnDeleteCallback;
    m_pCallbackArgument = pUserData;
}

void CAnimBlendAssociation::SetBlend(float fBlendAmount, float fBlendDelta)
{
    m_fBlendAmount = fBlendAmount;
    m_fBlendDelta = fBlendDelta;
}

void CAnimBlendAssociation::SetBlendTo(float fBlendAmount, float fBlendDelta)
{
    m_fBlendDelta = (fBlendAmount - m_fBlendAmount) * fBlendDelta;
}

void CAnimBlendAssociation::Start(float fStartTime)
{
    usFlags |= 0x01;
    SetCurrentTime(fStartTime);
}

CAnimBlendNode *CAnimBlendAssociation::GetNode(int index)
{
    return &m_pAnimBlendNodeArray[index];
}

void CAnimBlendAssociation::SyncAnimation(CAnimBlendAssociation *pOtherAssoc)
{
    SetCurrentTime(pOtherAssoc->fCurrentTime / pOtherAssoc->fTotalTime * m_pAnimHierarchy->fTotalTime);
}

void CAnimBlendAssociation::AllocateAnimBlendNodeArray(int uiCount)
{
    // ceil_div(sizeof(CAnimBlendNode) * uiCount, 64) * 64
    m_pAnimBlendNodeArray = CMemoryMgr::MallocAlign(((sizeof(CAnimBlendNode) * uiCount - 1) / 64 + 1) * 64, 64);
    for(size_t i = 0; i < uiCount; i++)
    {
        m_pAnimBlendNodeArray[i].Init();
    }
}

bool CAnimBlendAssociation::UpdateBlend(float fBlendAmount)
{
    m_fBlendAmount += fBlendAmount * m_fBlendAmount;
    if(m_fBlendAmount <= 0.0 && m_fBlendDelta < 0.0)
    {
        m_fBlendAmount = 0.0;
        if(m_fBlendDelta < 0.0)
        {
            m_fBlendDelta = 0.0;
        }
        if(m_usFlags & 0x04)
        {   
            if(m_uiCallbackType == FinishCallbackType)
            {
                SetFinishCallback(NULL, 0);
            }
            else if(m_uiCallbackType = DeleteCallbackType)
            {
                SetDeleteCallback(NULL, 0);
            }
            return false;
        }
    }
    if(m_fBlendAmount > 1.0)
    {
        m_fBlendAmount = 1.0;
        if(m_fBlendDelta > 0.0)
        {
            m_fBlendDelta = 0.0;
        }
    }
    return true;
    
}
