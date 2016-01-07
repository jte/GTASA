#include "StdInc.h"

CAnimBlendSequence::CAnimBlendSequence()
{
    m_numFramesSet = 0;
    m_isRoot = false;
    m_isCompressed = false;
    m_usingExternalMemory = false;
    m_hasBoneIdSet = false;
    m_numKeyFrames = 0;
    m_keyFrames = NULL;
    m_hash = -1;
}

CAnimBlendSequence::~CAnimBlendSequence()
{
    // If we allocated memory, clean it
    if(!m_usingExternalMemory && m_keyFrames)
    {
        CMemoryMgr::Free(m_keyFrames);
    }
}

void CAnimBlendSequence::SetName(const char* name)
{
    m_hash = CKeyGen::GetUppercaseKey(name);
}

void CAnimBlendSequence::SetBoneTag(uint16_t boneId)
{
    if(boneId != -1)
    {
        m_hasBoneIdSet = true;
        m_boneId = boneId;
    }
}

size_t CAnimBlendSequence::GetDataSize(bool compressed)
{
    // compressed version is 2x smaller
    size_t compressionDivide = compressed ? 2 : 1;
    return GetKeyFramesSize() / compressionDivide;
}

void CAnimBlendSequence::SetNumFrames(size_t frameCount, bool isRoot, bool compressed, unsigned char* st)
{
    m_keyFrames = (SKeyFrame**)((st) ? st : CMemoryMgr::Malloc(GetDataSize(compressed) * frameCount));
    m_isRoot = isRoot;
    m_numFramesSet = true;
    m_numKeyFrames = frameCount;
    m_isCompressed = compressed;
}

void CAnimBlendSequence::Uncompress(unsigned char* st)
{
    assert(m_isCompressed);

    if(m_numKeyFrames == 0)
    {
        return;
    }
    // S(Compressed)RootKeyFrame & S(Compressed)ChildKeyFrame inherits from S(Compressed)KeyFrame
    // so we don't care yet if it's really root key frame
    SRootKeyFrame** uncompressedFrames = (SRootKeyFrame**)(st ? st : CMemoryMgr::Malloc(GetKeyFramesSize()));
    SCompressedRootKeyFrame** compressedFrames = (SCompressedRootKeyFrame**)m_keyFrames;
    for(size_t i = 0; i < m_numKeyFrames; i++)
    {
        // Decompress rotation
        DecompressQuaternion(uncompressedFrames[i]->rotation, compressedFrames[i]->rotation);
        // Decompress time
        uncompressedFrames[i]->time = FixedPointToFloat<60>(compressedFrames[i]->time);
        // Decompress translation iif root sequence
        if(m_isRoot)
        {
            DecompressVector<1024>(uncompressedFrames[i]->translation, compressedFrames[i]->translation);
        }
    }
    // Free up old compressed frames
    if(!m_usingExternalMemory)
    {
        CMemoryMgr::Free(compressedFrames);
    }
    // Assign new uncompressed frames
    m_keyFrames = (SKeyFrame**)uncompressedFrames;
    m_usingExternalMemory = st != NULL;
    m_isCompressed = false;
}

size_t CAnimBlendSequence::GetKeyFramesSize() const
{
    return (m_isRoot ? sizeof(SRootKeyFrame) : sizeof(SChildKeyFrame)) * m_numKeyFrames;
}

void CAnimBlendSequence::CompressKeyframes(unsigned char* st)
{
    assert(!m_isCompressed);

    if(m_numKeyFrames == 0)
    {
        return;
    }

    SCompressedRootKeyFrame** compressedFrames = (SCompressedRootKeyFrame**)(st ? st : CMemoryMgr::Malloc(GetKeyFramesSize()));
    SRootKeyFrame** uncompressedFrames = (SRootKeyFrame**)m_keyFrames;
    for(size_t i = 0; i < m_numKeyFrames; i++)
    {
        CompressQuaternion(uncompressedFrames[i]->rotation, compressedFrames[i]->rotation);
        compressedFrames[i]->time = FloatToFixedPoint<60>(m_keyFrames[i]->time) + 0.5f;
        if(m_isRoot)
        {
            CompressVector<1024>(uncompressedFrames[i]->translation, compressedFrames[i]->translation);
        }
    }
    if(!m_usingExternalMemory)
    {
        CMemoryMgr::Free(m_keyFrames);
    }
    m_keyFrames = (SKeyFrame**)compressedFrames;
    m_usingExternalMemory = st != NULL;
    m_isCompressed = true;
}

void CAnimBlendSequence::RemoveUncompressedData(unsigned char* st)
{
    if(m_numKeyFrames != 0)
    {
        CompressKeyframes(st);
    }
}

void CAnimBlendSequence::RemoveQuaternionFlips()
{
    if(m_numKeyFrames >= 2)
    {
        for(size_t i = 0; i < m_numKeyFrames; i++)
        {
            SKeyFrame* keyFrame = m_keyFrames[i];
            keyFrame->rotation = -keyFrame->rotation;
        }
    }
}

bool CAnimBlendSequence::GetHasBoneIdSet() const
{
    return m_hasBoneIdSet;
}