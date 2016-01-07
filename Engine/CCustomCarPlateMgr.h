#pragma once

class CCustomCarPlateMgr
{
public:
    static RwTexture* CreatePlateTexture(char* plateText, uint8_t plateDesign);
    static bool GeneratePlateText(char* buffer, int length);
	static uint8_t GetMapRegionPlateDesign();
    static bool Initialise();
	/* LoadPlatecharsetDat(char const*, unsigned char*, int, int)
	*/ 
	static RpMaterial* SetupClump(RpClump* clump, char* plateText, uint8_t plateDesign);
	static bool SetupClumpAfterVehicleUpgrade(RpClump* clump, RpMaterial* material, uint8_t plateDesign);
	static RwTexture* SetupMaterialPlateTexture(RpMaterial* material, char* plateText, uint8_t plateDesign);
	static RwTexture* SetupMaterialPlatebackTexture(RpMaterial* material, uint8_t plateDesign);
	static void Shutdown();
private:
    static RwTexture* pCharsetTex;
	//* pPalette1555Tab
	static RwTexture* pPlatebackTexTab[3];
};