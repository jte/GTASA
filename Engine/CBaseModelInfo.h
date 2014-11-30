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

    //
    uint32_t GetHashKey() const;
    uint16_t GetRefCount() const;
    uint16_t GetTextureDictionary() const;
    uint8_t GetAlpha() const;
    uint8_t Get2dEffectCount() const;
    uint16_t Get2dEffectIndex() const;
    uint16_t GetObjectInfoIndex() const;
    CColModel* GetColModel() const;
    float GetLoadDistanceUnscaled() const;
    RwObject* GetRwObject() const;
    bool GetIsRoad() const;
protected:
    uint32_t m_hashKey;
    uint16_t m_refCount;
    uint16_t m_textureDictionary;
    uint8_t m_alpha;
    uint8_t m_2dEffectCount;
    uint16_t m_2dEffectIndex;
    uint16_t m_objectInfoIndex;
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
    CColModel* m_colModel;
    float m_loadDistanceUnscaled;
    RwObject* m_rwObject;
};