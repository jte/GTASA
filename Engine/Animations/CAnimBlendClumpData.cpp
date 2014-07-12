#include "StdInc.h"

CAnimBlendClumpData::CAnimBlendClumpData()
{
    m_clumpAnimAssoc = NULL;
    pad1 = 0;
    m_animBlendNodeCount = 0;
    pad2 = 0;
    m_frames = NULL;
}

void CAnimBlendClumpData::ForAllFrames(AnimBlendFrameDataCallback callback, void* data)
{
    for(size_t i = 0; i < m_animBlendNodeCount; i++)
    {
        callback(m_frames[i], data);
    }
}

void CAnimBlendClumpData::SetNumberOfBones(int count)
{
    m_animBlendNodeCount = count;
}

void CAnimBlendClumpData::ForAllFramesInSPR(AnimBlendFrameDataCallback, void*, unsigned int)
{
}

void CAnimBlendClumpData::LoadFramesIntoSPR()
{
}

AnimBlendFrameData** CAnimBlendClumpData::GetFrames() const
{
    return m_frames;
}

AnimBlendFrameData* CAnimBlendClumpData::GetFrame(size_t index) const
{ 
    return m_frames[index]; 
}

size_t CAnimBlendClumpData::GetAnimBlendNodeCount() const
{ 
    return m_animBlendNodeCount; 
}

SClumpAnimAssoc* CAnimBlendClumpData::GetClumpAnimAssoc() const
{
    return m_clumpAnimAssoc;
}