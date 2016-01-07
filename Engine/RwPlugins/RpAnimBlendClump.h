#pragma once

enum CLUMP_ANIM_ASSOC_FLAGS
{
    ANIMATION_STARTED = 0x01,
    ROOT_FRAME = 0x08,
    IS_PARTIAL_ASSOC = 0x10
};

class CAnimBlendAssociation;
class CAnimBlendHierarchy;
class CAnimBlendClumpData;
struct AnimBlendFrameData;

CAnimBlendClumpData* GetAnimBlendClumpData(RpClump* clump);
void RpAnimBlendClumpInit(RpClump *pClump);
bool RpAnimBlendClumpIsInitialized(RpClump *pClump);
void RpAnimBlendClumpAddAssociation(RpClump *pClump, CAnimBlendAssociation *pAssoc, unsigned int usFlags, float fStartTime, float fBlendAmount);
void RpAnimBlendClumpSetBlendDeltas(RpClump *pClump, unsigned int usFlags, float fBlendDelta);
void RpAnimBlendClumpRemoveAssociations(RpClump *pClump, unsigned int usFlags);
CAnimBlendAssociation *RpAnimBlendClumpGetAssociation(RpClump *pClump, const char *szName);
CAnimBlendAssociation *RpAnimBlendClumpGetAssociation(RpClump *pClump, AnimationId usAnimId);
CAnimBlendAssociation *RpAnimBlendClumpGetAssociation(RpClump *pClump, bool, CAnimBlendHierarchy *pAnimHierarchy);
RwBool RpAnimBlendPluginAttach();
CAnimBlendClumpData *RpAnimBlendAllocateData(RpClump *pClump);
CAnimBlendAssociation* RpAnimBlendClumpExtractAssociations(RpClump *pClump);
void RpAnimBlendClumpFillFrameArray(RpClump *pClump, AnimBlendFrameData **ppFrames);
AnimBlendFrameData* RpAnimBlendClumpFindBone(RpClump *pClump, unsigned int node_id);
AnimBlendFrameData* RpAnimBlendClumpFindFrame(RpClump *pClump, char const *name);
AnimBlendFrameData* RpAnimBlendClumpFindFrameFromHashKey(RpClump *pClump, unsigned int hash);
CAnimBlendAssociation* RpAnimBlendClumpGetFirstAssociation(RpClump *pClump);
CAnimBlendAssociation* RpAnimBlendClumpGetFirstAssociation(RpClump *pClump, unsigned int flags);
CAnimBlendAssociation* RpAnimBlendClumpGetMainAssociation(RpClump *pClump, CAnimBlendAssociation **pAssocs, float *fBlend);
CAnimBlendAssociation* RpAnimBlendClumpGetMainAssociation_N(RpClump *pClump, int n);
CAnimBlendAssociation* RpAnimBlendClumpGetMainPartialAssociation(RpClump *clump);
CAnimBlendAssociation* RpAnimBlendClumpGetMainPartialAssociation_N(RpClump *clump, int n);
size_t RpAnimBlendClumpGetNumAssociations(RpClump *pClump);
size_t RpAnimBlendClumpGetNumNonPartialAssociations(RpClump *pClump);
size_t RpAnimBlendClumpGetNumPartialAssociations(RpClump *pClump);
void RpAnimBlendClumpGiveAssociations(RpClump *clump);
/*	* RpAnimBlendClumpGiveAssociations(RpClump*, CAnimBlendAssociation*)
	* RpAnimBlendClumpPauseAllAnimations(RpClump*)
	* RpAnimBlendClumpRemoveAllAssociations(RpClump*)
	* RpAnimBlendClumpUnPauseAllAnimations(RpClump*)
	* RpAnimBlendClumpUpdateAnimations(RpClump*, float, bool)
	* RpAnimBlendCreateAnimationForHierarchy(RpHAnimHierarchy*)
	* RpAnimBlendFrameGetName(RwFrame*)
	* RpAnimBlendFrameSetName(RwFrame*, char*)
	* RpAnimBlendGetNextAssociation(CAnimBlendAssociation*)
	* RpAnimBlendGetNextAssociation(CAnimBlendAssociation*, unsigned int)
	* RpAnimBlendKeyFrameInterpolate(void*, void*, void*, float, void*)*/