#pragma once

class CAnimBlendHierarchy;

struct AnimBlendFrameData
{// sizeof(AnimBlendFrameData) = 0x18
    int flags;
    RwV3d pos;
    RwFrame *frame;
    union
    {
        RpHAnimNodeInfo *node;
        size_t bone_tag;
    };
};

struct SClumpAnimAssoc
{ // sizeof(SClumpAnimAssoc) = 0x2C
    SClumpAnimAssoc* next;
    void* userPtr;
    uint16_t animBlendNodeCount;
    uint16_t animGroup;
    CAnimBlendNode** animBlendNodeArray;
    CAnimBlendHierarchy* animHierarchy;
    float blendAmount;
    float blendDelta;
    float currentTime;
    float speed;
    float timeStep;
    uint16_t animId;
    uint16_t flags;
};

class CAnimBlendClumpData
{
    typedef void (*AnimBlendFrameDataCallback)(AnimBlendFrameData*, void*);
public:
	CAnimBlendClumpData();
	void ForAllFrames(AnimBlendFrameDataCallback callback, void* data);
	void ForAllFramesInSPR(AnimBlendFrameDataCallback, void*, unsigned int);
	void LoadFramesIntoSPR();
	void SetNumberOfBones(int count);
	~CAnimBlendClumpData();

    // accessors
    AnimBlendFrameData** GetFrames() const;
    AnimBlendFrameData* GetFrame(size_t index) const;
    size_t GetAnimBlendNodeCount() const;
    SClumpAnimAssoc* GetClumpAnimAssoc() const;
protected:
private:
    SClumpAnimAssoc* m_clumpAnimAssoc;
    int pad1;
    int m_animBlendNodeCount; // a.k.a bone count
    int pad2;
    struct AnimBlendFrameData** m_frames;
};
