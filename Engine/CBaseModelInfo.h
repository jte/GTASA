#pragma once

class C2dEffect;
class CAtomicModelInfo;
class CDamageAtomicModelInfo;
class CLodAtomicModelInfo;

class CBaseModelInfo
{
public:
    // virtual methods
	virtual CAtomicModelInfo *AsAtomicModelInfoPtr();
	virtual CDamageAtomicModelInfo *AsDamageAtomicModelInfoPtr();
	virtual CLodAtomicModelInfo *AsLodAtomicModelInfoPtr();
    virtual void ConvertAnimFileIndex();
	virtual size_t GetAnimFileIndex();
	virtual size_t GetTimeInfo();
	virtual void SetAnimFile(char const *szName);

    //
	void Add2dEffect(C2dEffect*);
	void AddRef();
	void AddTexDictionaryRef();
	CBaseModelInfo();
	void ClearTexDictionary();
	void DeleteCollisionModel();
	C2dEffect *Get2dEffect(int);
	void Init();
	void Init2dEffects();
	void RemoveRef();
	void RemoveTexDictionaryRef();
	void SetColModel(CColModel*, bool);
	void SetTexDictionary(char const*, char const*);
	void Shutdown();
	~CBaseModelInfo();
private:
    size_t m_uiHashKey;
    uint16_t m_usRefCount;
    int16_t m_usTextureDictionary;
    uint8_t m_ucAlpha;
    uint8_t m_uc2DFxCount;
    uint16_t m_uc2DIndex;
    uint16_t m_ucObjectInfoIndex;
    // 0x08 - bHasCollisionModel
    unsigned char           ucDynamicIndex: 8;          // +16

    unsigned char           bIsRoad: 1;          // +17
    unsigned char           dwUnknownFlag10: 1;
    unsigned char           dwUnknownFlag11: 1;
    unsigned char           dwUnknownFlag12: 1;
    unsigned char           dwUnknownFlag13: 1;
    unsigned char           dwUnknownFlag14: 1;
    unsigned char           dwUnknownFlag15: 1;
    unsigned char           dwUnknownFlag16: 1;

    // Flags used by CBaseModelInfo
    unsigned char           bHasBeenPreRendered: 1;     // +18
    unsigned char           bAlphaTransparency: 1;
    unsigned char           bIsLod: 1;
    unsigned char           bDontCastShadowsOn: 1;
    unsigned char           bDontWriteZBuffer: 1;
    unsigned char           bDrawAdditive: 1;
    unsigned char           bDrawLast: 1;
    unsigned char           bDoWeOwnTheColModel: 1;

    unsigned char           dwUnknownFlag25: 1;         // +19
    unsigned char           dwUnknownFlag26: 1;
    unsigned char           dwUnknownFlag27: 1;
    unsigned char           bSwaysInWind: 1;
    unsigned char           bCollisionWasStreamedWithModel: 1;  // CClumpModelInfo::SetCollisionWasStreamedWithModel(unsigned int)
    unsigned char           bDontCollideWithFlyer: 1;           // CAtomicModelInfo::SetDontCollideWithFlyer(unsigned int)
    unsigned char           bHasComplexHierarchy: 1;            // CClumpModelInfo::SetHasComplexHierarchy(unsigned int)
    unsigned char           bWetRoadReflection: 1;              // CAtomicModelInfo::SetWetRoadReflection(unsigned int)
    CColModel *pColModel;
    float fLoadDistanceUnscaled;
    RwObject *pRwObject;
};