#pragma once

enum VehicleUpgradePosn
{
	UPGRADE_BONNET,
	UPGRADE_BONNET_LEFT,
	UPGRADE_BONNET_RIGHT,
	UPGRADE_BONNET_DAM,
	UPGRADE_BONNET_LEFT_DAM,
	UPGRADE_BONNET_RIGHT_DAM,
	UPGRADE_SPOILER,
	UPGRADE_SPOILER_DAM,
	UPGRADE_WING_LEFT,
	UPGRADE_WING_RIGHT,
	UPGRADE_FRONTBULLBAR,
	UPGRADE_BACKBULLBAR,
	UPGRADE_LIGHTS,
	UPGRADE_LIGHTS_DAM,
	UPGRADE_ROOF,
	UPGRADE_NITRO,
};

struct UpgradePosnDesc
{
    UpgradePosnDesc();
	~UpgradePosnDesc();

	CVector m_position;
	CQuaternion m_rotation;
    int32_t m_parentHierarchyId;
};

struct EditableMaterial
{
    RpMaterial* material;
    RwTexture* texture;
};

class CVehicleModelInfo : public CClumpModelInfo
{
public:
    // virtual methods
    virtual ~CVehicleModelInfo();
    virtual size_t GetModelType();
    virtual void Init();
    virtual void Shutdown(); 
    virtual void DeleteRwObject();
    virtual size_t GetRwModelType();
    virtual void* CreateInstance();
    virtual void SetAnimFile(char const* animFile);
    virtual void ConvertAnimFileIndex();
	virtual size_t GetAnimFileIndex();
    virtual void SetClump(RpClump* clump); // DONE

	void AddRemap(int16_t id);
	void AddWheelUpgrade(uint32_t wheelSet, int16_t modelId);
	CVehicleModelInfo();
	//* ChooseComponent()
	//* ChooseSecondComponent()
	void ChooseVehicleColour(uint8_t& color1, uint8_t& color2, uint8_t& color3, uint8_t& color4, int colorIndex);
	static RwObject* ClearAtomicFlagCB(RwObject* object, void* data);
	static RwFrame* CollapseFramesCB(RwFrame* frame, void* data);
	void DeleteVehicleColourTextures();
	//* DisableEnvMap()
	//* FindEditableMaterialList()
	static RwTexture* FindTextureCB(const RwChar* name);
	const char* GetCustomCarPlateText();
	//* GetEditableMaterialListCB(RpAtomic*, void*)
	//* GetEditableMaterialListCB(RpMaterial*, void*)
	static RpMaterial* GetMatFXEffectMaterialCB(RpMaterial* material, void* data);
	//* GetMaximumNumberOfPassengersFromNumberOfDoors(int)
	size_t GetNumRemaps();
	uint16_t GetNumWheelUpgrades(int wheelNum);
	bool GetOriginalCompPosition(CVector& out, int id);
	void GetWheelPosn(int id, CVector& out, bool dontTransform);
	uint16_t GetWheelUpgrade(int upgradeNum, int wheelNum);
	static RpMaterial* HasAlphaMaterialCB(RpMaterial* material, void* data);
	static RpAtomic* HideAllComponentsAtomicCB(RpAtomic* atomic, void* data);
    static RpAtomic* HideDamagedAtomicCB(RpAtomic* atomic, void* data);
	//* IsUpgradeAvailable(VehicleUpgradePosn)
	void LoadEnvironmentMaps();
	//* LoadVehicleColours()
	//* LoadVehicleUpgrades()
	static RwObject* MoveObjectsCB(RwObject* object, void* data);
	void PreprocessHierarchy();
	void ReduceMaterialsInVehicle();
	static void ResetEditableMaterials(RpClump* clump);
	static RwObject* SetAtomicFlagCB(RwObject* object, void* data);
	void SetAtomicRenderCallbacks();
	static RpAtomic* SetAtomicRendererCB(RpAtomic* atomic, void* data);
	static RpAtomic* SetAtomicRendererCB_BigVehicle(RpAtomic* atomic, void* data);
	static RpAtomic* SetAtomicRendererCB_Boat(RpAtomic* atomic);
	static RpAtomic* SetAtomicRendererCB_Heli(RpAtomic* atomic, void* data);
	static RpAtomic* SetAtomicRendererCB_RealHeli(RpAtomic* atomic, void* data);
	static RpAtomic* SetAtomicRendererCB_Train(RpAtomic* atomic, void* data);
	void SetCarCustomPlate();
	void SetCustomCarPlateText(char* text);
	void SetEditableMaterials(RpClump* clump);
	static RpAtomic* SetEditableMaterialsCB(RpAtomic* atomic, void* data);
	static RpMaterial* SetEditableMaterialsCB(RpMaterial* material, void* data);
	void SetEnvMapCoeff(float coeff);
	static RpAtomic* SetEnvMapCoeffAtomicCB(RpAtomic* atomic, void* data);
	static RpMaterial* SetEnvMapCoeffCB(RpMaterial* material, void* data);
	static RpAtomic* SetEnvironmentMapAtomicCB(RpAtomic* atomic, void* data);
	static RpMaterial* SetEnvironmentMapCB(RpMaterial* material, void* data);
	void SetRenderPipelines();
	static RpAtomic* SetRenderPipelinesCB(RpAtomic* atomic, void* data);
	void SetVehicleColour(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void SetVehicleComponentFlags(RwFrame* frame, uint32_t flags);
	void SetupCommonData();
	//* SetupLightFlags(CVehicle*)
	//* ShutdownEnvironmentMaps()
	//* ShutdownLightTexture()
	void StopUsingCommonVehicleTexDicationary();
	void UseCommonVehicleTexDicationary();
protected:
private:
	RpMaterial* m_plateMaterial;
    char m_plateText[8];
	__int8 field_30;
	uint8_t m_plateType;
	char m_nGameName[8];
	__int16 pad0;
	uint32_t m_vehicleType;
	float m_wheelSizeFront;
	float m_wheelSizeRear;
	uint16_t m_wheelModelId;
	uint16_t m_handlingId;
	uint8_t m_numDoors;
	uint8_t m_nClass;
	uint8_t m_nFlags;
	uint8_t m_nWheelUpgradeClass;
	uint16_t m_wTimesUsed;
	uint16_t m_wFrq;
	union{
		unsigned __int32 m_dwCompRules;
		struct{
			unsigned __int32 m_nExtraA_comp1 : 4;
			unsigned __int32 m_nExtraA_comp2 : 4;
			unsigned __int32 m_nExtraA_comp3 : 4;
			unsigned __int32 m_nExtraA_rule : 4;
			unsigned __int32 m_nExtraB_comp1 : 4;
			unsigned __int32 m_nExtraB_comp2 : 4;
			unsigned __int32 m_nExtraB_comp3 : 4;
			unsigned __int32 m_nExtraB_rule : 4;
		};
	};
	float m_steerAngle;

	class CVehicleStructure
    {
	public:
        CVehicleStructure();
		~CVehicleStructure();

		CVector m_dummyPos[15];
		UpgradePosnDesc m_upgrades[18];
		RpAtomic* m_extras[6];
        uint8_t m_numExtras;
        uint32_t m_maskComponentsDamagable;
		static class CPool* m_infoPool;
	}* m_vehicleStruct;

	__int8 field_60[464];
	RpMaterial* m_dirtMaterials[16];
	__int8 field_270[64];
	unsigned __int8 m_primaryColors[8];
	unsigned __int8 m_secondaryColors[8];
	unsigned __int8 m_tertiaryColors[8];
	unsigned __int8 m_quaternaryColors[8];
	unsigned __int8 m_numColorVariations;
	unsigned __int8 m_lastColorVariation;
    
    CRGBA m_colour;

	__int16 m_awUpgrades[18];

	int16_t m_remaps[4];

	class CAnimBlock *m_pAnimBlock;

	static class CLinkedUpgradeList{
	public:
		int16_t m_wUpgrade1[30];
		int16_t m_wUpgrade2[30];
		uint32_t m_dwLinksCount;
		// add upgrade with components upgrade1 and upgrade2
		void AddUpgradeLink(int16_t upgrade1, int16_t upgrade2);
		// find linked upgrade for this upgrade. In this case upgrade param could be upgrade1 or 
		// upgrade2
		int16_t FindOtherUpgrade(int16_t upgrade);
	} &ms_linkedUpgrades;

	static RwObjectNameIdAssocation ms_vehicleDescs[12];
	static RwTexture* ms_pLightsTexture;
	static RwTexture* ms_pLightsOnTexture;
	static RwTexture* ms_pRemapTexture;
	static CRGBA ms_currentCol;
	static uint16_t ms_numWheelUpgrades[4];
	static uint16_t ms_upgradeWheels[15][4];
	static int8_t* ms_lightsOn[4];
	static int8_t ms_compsUsed[2];
	static CRGBA ms_vehicleColourTable[128];
	
};