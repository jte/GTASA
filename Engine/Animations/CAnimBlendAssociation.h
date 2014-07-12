#pragma once

class CAnimBlendStaticAssociation;
class CAnimBlendNode;

class CAnimBlendAssociation 
{
public:
    typedef void (*Callback)(CAnimBlendAssociation*, void*);
    enum CallbackType
    {
        FinishCallbackType = 1,
        DeleteCallbackType = 2
    };
	void AllocateAnimBlendNodeArray(int count);
	//* CAnimBlendAssociation()
	CAnimBlendAssociation(CAnimBlendAssociation& assoc);
    CAnimBlendAssociation(CAnimBlendStaticAssociation& staticAssoc);
	//* CAnimBlendAssociation(RpClump *, CAnimBlendHierarchy *)
	//* FreeAnimBlendNodeArray()
	CAnimBlendNode* GetNode(int index);
	//* Init(CAnimBlendAssociation&)
	void Init(CAnimBlendStaticAssociation& staticAssoc);
	void Init(RpClump* clump, CAnimBlendHierarchy* animHier);
	void ReferenceAnimBlock();
	void SetBlend(float blendAmount, float blendDelta);
	void SetBlendTo(float blendAmount, float blendDelta);
	void SetCurrentTime(float currentTime);
	void SetDeleteCallback(Callback deleteCallback, void* userData);
	void SetFinishCallback(Callback finishCallback, void* userData);
	void Start(float startTime);
	void SyncAnimation(CAnimBlendAssociation* otherAssoc);
	bool UpdateBlend(float blendAmount);
	//* UpdateTime(float, float)
	//* UpdateTimeStep(float, float)
	//* ~CAnimBlendAssociation()

    const struct SClumpAnimAssoc& GetClumpAnimAssoc() const;
private:
    SClumpAnimAssoc m_assoc;
    CallbackType m_callbackType;
    void* m_callback;
    void* m_callbackArg;
};