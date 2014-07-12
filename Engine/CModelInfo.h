#pragma once

class CBaseModelInfo;

class CModelInfo
{
public:
	//* AddAtomicModel(int)
	//* AddClumpModel(int)
	//* AddDamageAtomicModel(int)
	//* AddLodAtomicModel(int)
	//* AddLodTimeModel(int)
	//* AddPedModel(int)
	//* AddTimeModel(int)
	//* AddVehicleModel(int)
	//* AddWeaponModel(int)
	//* Get2dEffectStore()
	static CBaseModelInfo* GetModelInfo(char const* name, int* index);
	static CBaseModelInfo* GetModelInfo(char const* name, int low, int high);
	static CBaseModelInfo* GetModelInfoFromHashKey(unsigned int hash, int* index);
	static CBaseModelInfo* GetModelInfoUInt16(char const* name, unsigned short* index);
	static void Initialise();
	static bool IsBikeModel(int index);
	static bool IsBmxModel(int index);
	static bool IsBoatModel(int index);
	static bool IsCarModel(int index);
	static bool IsFakePlaneModel(int index);
	static bool IsHeliModel(int index);
	static bool IsMonsterTruckModel(int index);
	static bool IsPlaneModel(int index);
	static bool IsQuadBikeModel(int index);
	static bool IsTrailerModel(int index);
	static bool IsTrainModel(int index);
	static uint32_t IsVehicleModelType(int index);
	//* PrintModelInfoStoreUsage()
	//* ReInit2dEffects()
	//* ShutDown()

    static CBaseModelInfo* GetModelInfo(size_t id);
private:
	static size_t ms_lastPositionSearched;
	static CBaseModelInfo* ms_modelInfoPtrs[20000];
};