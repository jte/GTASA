#pragma once

class CAnimBlendStaticAssociation
{
public:
	void AllocateSequenceArray(int iCount);
	CAnimBlendStaticAssociation();
	CAnimBlendStaticAssociation(RpClump*, CAnimBlendHierarchy*);
	void FreeSequenceArray();
	void Init(RpClump*, CAnimBlendHierarchy*);
	~CAnimBlendStaticAssociation();
private:
    uint16_t dwNumBones;
    uint16_t usAnimId;
    uint16_t usAnimGroup;
    uint16_t usFlags;
    CAnimBlendSequence **ppSequences;
    CAnimBlendHierarchy *pAnimHierarchy;
};