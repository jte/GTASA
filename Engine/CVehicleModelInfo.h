#pragma once

class CVehicleModelInfo : public CClumpModelInfo
{
public:
	//* AddRemap(char const*)
	//* AddWheelUpgrade(int, int)
	//* CVehicleModelInfo()
	//* ChooseComponent()
	//* ChooseSecondComponent()
	//* ChooseVehicleColour(unsigned char&, unsigned char&, unsigned char&, unsigned char&, int)
	//* ClearAtomicFlagCB(RwObject*, void*)
	//* CollapseFramesCB(RwFrame*, void*)
	//* ConvertAnimFileIndex()
	//* CreateInstance()
	//* DeleteRwObject()
	void DeleteVehicleColourTextures();
	//* DisableEnvMap()
	//* FindEditableMaterialList()
	//* FindTextureCB(char const*)
	//* GetAnimFileIndex()
	//* GetCustomCarPlateText()
	//* GetEditableMaterialListCB(RpAtomic*, void*)
	//* GetEditableMaterialListCB(RpMaterial*, void*)
	//* GetMatFXEffectMaterialCB(RpMaterial*, void*)
	//* GetMaximumNumberOfPassengersFromNumberOfDoors(int)
	//* GetModelType()
	//* GetNumRemaps()
	//* GetNumWheelUpgrades(int)
	//* GetOriginalCompPosition(CVector&, int)
	//* GetWheelPosn(int, CVector&, bool)
	//* GetWheelUpgrade(int, int)
	//* HasAlphaMaterialCB(RpMaterial*, void*)
	//* HideAllComponentsAtomicCB(RpAtomic*, void*)
	//* HideDamagedAtomicCB(RpAtomic*, void*)
	//* Init()
	//* IsUpgradeAvailable(VehicleUpgradePosn)
	//* LoadEnvironmentMaps()
	//* LoadVehicleColours()
	//* LoadVehicleUpgrades()
	//* MoveObjectsCB(RwObject*, void*)
	//* PreprocessHierarchy()
	//* ReduceMaterialsInVehicle()
	//* ResetEditableMaterials(RpClump*)
	//* SetAnimFile(char const*)
	//* SetAtomicFlagCB(RwObject*, void*)
	//* SetAtomicRenderCallbacks()
	//* SetAtomicRendererCB(RpAtomic*, void*)
	//* SetAtomicRendererCB_BigVehicle(RpAtomic*, void*)
	//* SetAtomicRendererCB_Boat(RpAtomic*, void*)
	//* SetAtomicRendererCB_Heli(RpAtomic*, void*)
	//* SetAtomicRendererCB_RealHeli(RpAtomic*, void*)
	//* SetAtomicRendererCB_Train(RpAtomic*, void*)
	//* SetCarCustomPlate()
	//* SetClump(RpClump*)
	//* SetCustomCarPlateText(char*)
	//* SetEditableMaterials(RpClump*)
	//* SetEditableMaterialsCB(RpAtomic*, void*)
	//* SetEditableMaterialsCB(RpMaterial*, void*)
	//* SetEnvMapCoeff(float)
	//* SetEnvMapCoeffAtomicCB(RpAtomic*, void*)
	//* SetEnvMapCoeffCB(RpMaterial*, void*)
	//* SetEnvironmentMapAtomicCB(RpAtomic*, void*)
	//* SetEnvironmentMapCB(RpMaterial*, void*)
	//* SetRenderPipelines()
	//* SetRenderPipelinesCB(RpAtomic*, void*)
	//* SetVehicleColour(unsigned char, unsigned char, unsigned char, unsigned char)
	//* SetVehicleComponentFlags(RwFrame*, unsigned int)
	//* SetupCommonData()
	//* SetupLightFlags(CVehicle*)
	//* ShutdownEnvironmentMaps()
	//* ShutdownLightTexture()
	//* StopUsingCommonVehicleTexDicationary()
	//* UseCommonVehicleTexDicationary()
	//* ~CVehicleModelInfo()
protected:
private:
    	//* ms_compsToUse
	//* ms_compsUsed
	//* ms_currentCol
	//* ms_lightsOn
	//* ms_linkedUpgrades
	//* ms_numWheelUpgrades
	//* ms_pLightsOnTexture
	//* ms_pLightsTexture
	//* ms_pRemapTexture
	//* ms_upgradeWheels
	//* ms_vehicleColourTable
	//* ms_vehicleDescs
};