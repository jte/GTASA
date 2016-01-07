#include "StdInc.h"

CAnimBlendStaticAssociation::CAnimBlendStaticAssociation()
{
    m_animId = -1;
    m_animGroup = -1;
    m_flags = 0;
    m_sequences = NULL;
    m_animHierarchy = NULL;
}

CAnimBlendStaticAssociation::CAnimBlendStaticAssociation(RpClump* clump, CAnimBlendHierarchy* hierarchy)
{
    m_animId = -1;
    m_animGroup = -1;
    m_flags = 0;
    m_sequences = NULL;
    Init(clump, hierarchy);
}

CAnimBlendStaticAssociation::~CAnimBlendStaticAssociation()
{
    FreeSequenceArray();
}

void CAnimBlendStaticAssociation::AllocateSequenceArray(size_t count)
{
    m_sequences = (CAnimBlendSequence**)CMemoryMgr::Calloc(sizeof(CAnimBlendSequence*) * count, NULL);
}

void CAnimBlendStaticAssociation::FreeSequenceArray()
{
    if (m_sequences)
    {
        CMemoryMgr::Free(m_sequences);
    }
}

void CAnimBlendStaticAssociation::Init(RpClump* clump, CAnimBlendHierarchy* hierarchy)
{
    m_animHierarchy = hierarchy;
    if (!clump)
    {
        return;
    }
    CAnimBlendClumpData* clumpData = GetAnimBlendClumpData(clump);
    m_numBones = clumpData->GetAnimBlendNodeCount();
    AllocateSequenceArray(m_numBones);
    for (size_t i = 0; i < hierarchy->GetNumSequences(); i++)
    {
        CAnimBlendSequence* sequence = hierarchy->GetSequence(i);
        AnimBlendFrameData* frameData;
        if (sequence->GetHasBoneIdSet())
        {
            frameData = RpAnimBlendClumpFindBone(clump, sequence->GetBoneId());
        }
        else
        {
            frameData = RpAnimBlendClumpFindFrameFromHashKey(clump, sequence->GetHash());
        }
        if (frameData && sequence->GetNumKeyFrames() != 0)
        {
            m_sequences[GET_INDEX_FROM_BASE(frameData, clumpData->GetFrames())] = sequence;
        }
    }
}