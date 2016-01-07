#pragma once

class CAnimBlendStaticAssociation
{
public:
	void AllocateSequenceArray(size_t count);
	CAnimBlendStaticAssociation();
	CAnimBlendStaticAssociation(RpClump* clump, CAnimBlendHierarchy* hierarchy);
	void FreeSequenceArray();
	void Init(RpClump* clump, CAnimBlendHierarchy* hierarchy);
	~CAnimBlendStaticAssociation();
private:
    uint16_t m_numBones;
    int16_t m_animId;
    int16_t m_animGroup;
    uint16_t m_flags;
    CAnimBlendSequence** m_sequences;
    CAnimBlendHierarchy* m_animHierarchy;
};