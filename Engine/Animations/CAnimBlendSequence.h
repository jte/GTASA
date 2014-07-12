#pragma once

class CAnimBlendSequence
{
public:
	CAnimBlendSequence();
	void CompressKeyframes(unsigned char* st);
	size_t GetDataSize(bool compressed);
	//* MoveMemory()
	//* Print()
	void RemoveQuaternionFlips();
	void RemoveUncompressedData(unsigned char* st);
	void SetBoneTag(uint16_t boneId);
	void SetName(char const* name);
	void SetNumFrames(size_t frameCount, bool isRoot, bool compressed, unsigned char* st);
	void Uncompress(unsigned char* st);
	~CAnimBlendSequence();

    //
    bool GetIsInitialized() const;
    bool GetIsRoot() const;
    bool GetIsCompressed() const;
    bool GetIsUsingExternalMemory() const;
    uint16_t GetNumKeyFrames() const;
    struct SKeyFrame* GetKeyFrame(size_t id, bool isRoot) const;
    uint32_t GetHash() const;
    uint16_t GetBoneId() const;
protected:
    size_t GetKeyFramesSize() const;
private:
    union
    {
        uint16_t m_boneId; // IF m_hasBoneIdSet == TRUE
        uint32_t m_hash;   // IF m_hasBoneIdSet == FALSE
    };
    uint32_t m_numFramesSet : 1;
    uint32_t m_isRoot : 1;
    uint32_t m_isCompressed : 1;
    uint32_t m_usingExternalMemory : 1;
    uint32_t m_hasBoneIdSet : 1;
    uint16_t m_numKeyFrames;
    struct SKeyFrame** m_keyFrames;
};