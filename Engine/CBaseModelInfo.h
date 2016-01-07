#pragma once

class C2dEffect;
class CAtomicModelInfo;
class CDamageAtomicModelInfo;
class CLodAtomicModelInfo;
class CTimeInfo;

class CBaseModelInfo
{
public:
    // virtual methods
    virtual ~CBaseModelInfo();
	virtual CAtomicModelInfo *AsAtomicModelInfoPtr();
	virtual CDamageAtomicModelInfo *AsDamageAtomicModelInfoPtr();
	virtual CLodAtomicModelInfo *AsLodAtomicModelInfoPtr();
    virtual size_t GetModelType();
    virtual CTimeInfo* GetTimeInfo();
    virtual void Init(); // DONE
    virtual void Shutdown(); // DONE
    virtual void DeleteRwObject();
    virtual size_t GetRwModelType();
    virtual void* CreateInstance(RwMatrixTag*);
    virtual void* CreateInstance();
    virtual void SetAnimFile(char const *szName); // DONE
    virtual void ConvertAnimFileIndex();
	virtual size_t GetAnimFileIndex(); // DONE
	
    //
	void Add2dEffect(C2dEffect*); // DONE
	void AddRef(); // DONE
	void AddTexDictionaryRef(); // DONE
	CBaseModelInfo(); // DONE
	void ClearTexDictionary(); // DONE
	void DeleteCollisionModel(); // DONE
	Plugin2dEffectDescriptor* Get2dEffect(size_t id); // DONE
	void Init2dEffects(); // DONE
	void RemoveRef(); // DONE
	void RemoveTexDictionaryRef(); // DONE
	void SetColModel(CColModel* colModel, bool pairOtherModel); // DONE
	void SetTexDictionary(char const* txdName); // DONE

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
    bool IsLod() const;
    bool GetDontCastShadowsOn() const;
	bool GetHasAnimation() const;
	bool GetHasBeenPrerendered() const;
	void SetHasBeenPrerendered(bool hasBeenPrerendered);
    void SetDrawAdditive(bool drawAdditive);
	bool GetDoWeOwnColModel() const;
protected:
    uint32_t m_hashKey;//0
    uint16_t m_refCount;//4
    int16_t m_textureDictionary;//6
    uint8_t m_alpha;//8
    uint8_t m_2dEffectCount;//9
    int16_t m_2dEffectIndex;//10
    int16_t m_objectInfoIndex;//12

    unsigned char           bHasBeenPreRendered : 1;     // 1 ;; 14
    unsigned char           bAlphaTransparency : 1; // 2
    unsigned char           m_isLod : 1; // 4
    unsigned char           m_dontCastShadowsOn: 1; // 8
    unsigned char           bDontWriteZBuffer: 1; // 0x10
    unsigned char           bDrawAdditive: 1; // 0x20
    unsigned char           m_drawnLast : 1; // 0x40
    unsigned char           m_doWeOwnTheColModel : 1; // 0x80

    unsigned char           m_hasAnimation : 1;         // 0x100
    unsigned char           dwUnknownFlag26: 1; // 0x200
    unsigned char           dwUnknownFlag27: 1; // 0x400
    unsigned char           bSwaysInWind: 1; // 0x800
    unsigned char           bCollisionWasStreamedWithModel: 1;  // CClumpModelInfo::SetCollisionWasStreamedWithModel(unsigned int)
    unsigned char           bDontCollideWithFlyer: 1;           // CAtomicModelInfo::SetDontCollideWithFlyer(unsigned int)
    unsigned char           bHasComplexHierarchy: 1;            // CClumpModelInfo::SetHasComplexHierarchy(unsigned int)
    unsigned char           bWetRoadReflection: 1;              // CAtomicModelInfo::SetWetRoadReflection(unsigned int)

    CColModel* m_colModel; // 16
    float m_loadDistanceUnscaled; // 20
    RwObject* m_rwObject; // 24
};