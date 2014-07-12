#pragma once

#include "../CLinkList.h"

class CAnimBlendSequence;
class CAnimBlendHierarchy;

class CAnimBlendHierarchy
{
    typedef std::vector<CAnimBlendSequence*> AnimBlendSequenceVector;
public:
	unsigned char* AllocSequenceBlock(bool bCompressed);
	CAnimBlendHierarchy();
	void CalcTotalTime();
	//* CalcTotalTimeCompressed()
	//* CompressKeyframes()
	CAnimBlendSequence* FindSequence(const char* name);
	unsigned char *GetSequenceBlock();
	//* MoveMemory()
	//* Print()
	void RemoveAnimSequences();
	void RemoveQuaternionFlips();
	void RemoveUncompressedData();
	void SetName(char const *name);
	void Shutdown();
	void Uncompress();
	~CAnimBlendHierarchy();

    //
    uint32_t GetHashKey() const;
    const AnimBlendSequenceVector& GetSequences() const;
    uint16_t GetNumSequences() const;
    uint32_t GetAnimBlockId() const;
    float GetTotalTime() const;
    void SetLoadCompressed(bool loadCompressed);
    bool GetLoadCompressed() const;
    void SetSequences(size_t count);
    CAnimBlendSequence* GetSequence(size_t id) const;
private:
    uint32_t m_hash;
    AnimBlendSequenceVector m_sequences;
    uint16_t m_numSequences;
    bool m_loadUncompressed;
    bool m_unknown;
    uint32_t m_animBlockId;
    float m_totalTime;
    CLinkList<CAnimBlendHierarchy*>* m_linkPtr;
};