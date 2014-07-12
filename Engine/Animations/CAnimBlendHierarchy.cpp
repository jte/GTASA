#include "StdInc.h"

CAnimBlendHierarchy::CAnimBlendHierarchy()
{
    m_numSequences = 0;
    m_loadCompressed = false;
    bRunningCompressed = false;
    m_animBlockId = -1;
    m_totalTime = 0.0f;
    m_linkPtr = NULL;
}

CAnimBlendSequence* CAnimBlendHierarchy::FindSequence(const char* name)
{
    size_t hash = CKeyGen::GetUppercaseKey(name);
    for(size_t i = 0; i < m_numSequences; i++)
    {
        if(m_sequences[i]->GetHash() == hash)
        {
            return m_sequences[i];
        }
    }
    return NULL;
}

void CAnimBlendHierarchy::SetName(const char *name)
{
    m_hash = CKeyGen::GetUppercaseKey(name);
}

void CAnimBlendHierarchy::RemoveQuaternionFlips()
{
    for(size_t i = 0; i < m_numSequences; i++)
    {
        m_sequences[i]->RemoveQuaternionFlips();
    }
}

void CAnimBlendHierarchy::CalcTotalTime()
{
    // Zero Total Time
    m_totalTime = 0.0f;
    // Sum up times of sequences
    for(size_t i = 0; i < m_numSequences; i++)
    {
        CAnimBlendSequence* sequence = m_sequences[i];
        bool isRoot = sequence->GetIsRoot();
        float seqLastFrameTime = sequence->GetKeyFrame(sequence->GetNumKeyFrames() - 1, isRoot).time;
        if(seqLastFrameTime > m_totalTime)
        {
            m_totalTime = seqLastFrameTime;
        }
        // Sum up times of sequence's frames
        for(size_t j = 1; j < sequence->GetNumKeyFrames(); j++)
        {
            sequence->GetKeyFrame(j, isRoot)->time -= sequence->GetKeyFrame(j - 1)->time;
        }
    }
}

unsigned char* CAnimBlendHierarchy::AllocSequenceBlock(bool compressed)
{
    size_t size = 0;
    for(size_t i = 0; i < m_numSequences; i++)
    {
        size += m_sequences[i]->GetDataSize(compressed);
    }
    return size != 0 ? (unsigned char*)CMemoryMgr::Malloc(size) : NULL;
}

void CAnimBlendHierarchy::RemoveUncompressedData()
{
    unsigned char* pSequenceBlock = AllocSequenceBlock(true);
    for(size_t i = 0; i < m_numSequences; i++)
    {
        CAnimBlendSequence *sequence = m_sequences[i];
        sequence->RemoveUncompressedData(pSequenceBlock);
        pSequenceBlock += sequence->GetDataSize(true);
    }
    unsigned char* pSeqMem = GetSequenceBlock();
    if(pSeqMem)
    {
        RwFree(pSeqMem);
    }
    m_bLoadCompressed = true;
}

unsigned char* CAnimBlendHierarchy::GetSequenceBlock()
{
    return (m_sequences[0]->GetIsUsingExternalMemory()) ? (unsigned char*)m_sequences[0]->GetKeyFrame(0) : NULL;
}

void CAnimBlendHierarchy::Uncompress()
{
    unsigned char *sequenceBlock = (unsigned char*)AllocSequenceBlock(false);
    for(size_t i = 0; i < m_numSequences; i++)
    {
        CAnimBlendSequence *pSequence = m_sequences[i];
        pSequence->Uncompress(sequenceBlock);
        sequenceBlock += pSequence->GetDataSize(false);
    }
    unsigned char *pSeqMem = GetSequenceBlock();
    if(pSeqMem)
    {
        RwFree(pSeqMem);
    }
    m_bLoadCompressed = false;
    if(m_totalTime == 0.0)
    {
        RemoveQuaternionFlips();
        CalcTotalTime();
    }
}

CAnimBlendHierarchy::~CAnimBlendHierarchy()
{
    RemoveAnimSequences();
}

void CAnimBlendHierarchy::RemoveAnimSequences()
{
    CAnimManager::RemoveFromUncompressedCache(this);
    m_sequences.clear();
    m_numSequences = 0;
    m_totalTime = 0.0;
    unsigned char* pSeqMem = GetSequenceBlock();
    if(pSeqMem)
    {
        RwFree(pSeqMem);
    }
}

void CAnimBlendHierarchy::Shutdown()
{
    RemoveAnimSequences();
    m_bLoadCompressed = false;
}

uint32_t CAnimBlendHierarchy::GetHashKey() const
{
    return m_hashKey;
}

const CAnimBlendHierarchy::AnimBlendSequenceVector& CAnimBlendHierarchy::GetSequences() const
{
    return m_sequences;
}

uint16_t CAnimBlendHierarchy::GetNumSequences() const
{
    return m_numSequences;
}

uint32_t CAnimBlendHierarchy::GetAnimBlockId() const
{
    return m_animBlockId;
}

float CAnimBlendHierarchy::GetTotalTime() const
{
    return m_totalTime;
}

void CAnimBlendHierarchy::SetSequences(size_t count)
{
    m_numSequences = count;
    m_sequences.resize(count);
}