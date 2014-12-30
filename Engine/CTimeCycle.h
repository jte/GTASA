#pragma once

class CTimeCycleBox
{
public:
    CBox box;
    uint16_t field_18;
    uint8_t field_1A;
    uint8_t field_1B;
    uint32_t field_1C;
    uint32_t field_20;
    uint32_t field_24;
};

class CTimeCycle
{
public:
    
	static void AddOne(CBox& a1, short a2, int a3, float a4, float a5, float a6);
	/* CalcColoursForPoint(CVector, CColourSet*)
	*/ 
    static float FindFarClipForCoors(CVector coors);
	/* FindTimeCycleBox(CVector, CTimeCycleBox**, float*, bool, bool, CTimeCycleBox*)
	*/ 
    static float GetAmbientBlue();
	/* GetAmbientBlue_BeforeBrightness()
	 */
    static float GetAmbientBlue_Obj();
    static float GetAmbientGreen();
	/* GetAmbientGreen_BeforeBrightness()
	*/ 
    static float GetAmbientGreen_Obj();
    static float GetAmbientRed();
	/* GetAmbientRed_BeforeBrightness()
	*/ 
    static float GetAmbientRed_Obj();
    static void InitForRestart();
    static void Initialise();
	static void SetConstantParametersForPostFX();
	static void Shutdown();
    static void StartExtraColour(int id, bool noInterpol);
	static void StopExtraColour(bool noInterpol);
	static void Update();
protected:
private:
    
	/* m_BelowHorizonGrey
	* m_BrightnessAddedToAmbientBlue
	* m_BrightnessAddedToAmbientGreen
	* m_BrightnessAddedToAmbientRed
	*/ 
    static CColourSet m_CurrentColours;
	static size_t m_CurrentStoredValue;
	static uint32_t m_ExtraColour;
	static float m_ExtraColourInter;
	static uint32_t m_ExtraColourWeatherType;
	static float m_FogReduction;
	static uint32_t m_NumBoxes;
	static CVector m_VectorToSun[16];
    static CTimeCycleBox m_aBoxes[32];
    static bool m_bExtraColourOn;
	static uint8_t m_fCloudAlpha[23][8];
	static uint8_t m_fFarClip[23][8];
	static uint8_t m_fFogStart[23][8];
	static uint8_t m_fLightsOnGroundBrightness[23][8];
	static uint8_t m_fPostFx1Alpha[23][8];
	static uint8_t m_fPostFx1Blue[23][8];
	static uint8_t m_fPostFx1Green[23][8];
	static uint8_t m_fPostFx1Red[23][8];
	static uint8_t m_fPostFx2Alpha[23][8];
	static uint8_t m_fPostFx2Blue[23][8];
	static uint8_t m_fPostFx2Green[23][8];
	static uint8_t m_fPostFx2Red[23][8];
	/* m_fShadowDisplacementX
	* m_fShadowDisplacementY
	* m_fShadowFrontX
	* m_fShadowFrontY
	* m_fShadowSideX
	* m_fShadowSideY
	*/
    static uint8_t m_fSpriteBrightness[23][8];
	static uint8_t m_fSpriteSize[23][8];
	static uint8_t m_fSunSize[23][8];
	static uint8_t m_fWaterAlpha[23][8];
	static uint8_t m_fWaterBlue[23][8];
	static uint8_t m_fWaterGreen[23][8];
	static uint8_t m_fWaterRed[23][8];
	static uint8_t m_nAmbientBlue[23][8];
	static uint8_t m_nAmbientBlue_Obj[23][8];
	static uint8_t m_nAmbientGreen[23][8];
	static uint8_t m_nAmbientGreen_Obj[23][8];
	static uint8_t m_nAmbientRed[23][8];
	static uint8_t m_nAmbientRed_Obj[23][8];
	//* m_nDirectionalMult
	static uint8_t m_nFluffyCloudsBottomBlue[23][8];
	static uint8_t m_nFluffyCloudsBottomGreen[23][8];
	static uint8_t m_nFluffyCloudsBottomRed[23][8];
	static uint8_t m_nHighLightMinIntensity[23][8];
	static uint8_t m_nLightShadowStrength[23][8];
	static uint8_t m_nLowCloudsBlue[23][8];
	static uint8_t m_nLowCloudsGreen[23][8];
	static uint8_t m_nLowCloudsRed[23][8];
	static uint8_t m_nPoleShadowStrength[23][8];
	static uint8_t m_nShadowStrength[23][8];
	static uint8_t m_nSkyBottomBlue[23][8];
	static uint8_t m_nSkyBottomGreen[23][8];
	static uint8_t m_nSkyBottomRed[23][8];
	static uint8_t m_nSkyTopBlue[23][8];
	static uint8_t m_nSkyTopGreen[23][8];
	static uint8_t m_nSkyTopRed[23][8];
	static uint8_t m_nSunCoreBlue[23][8];
	static uint8_t m_nSunCoreGreen[23][8];
	static uint8_t m_nSunCoreRed[23][8];
	static uint8_t m_nSunCoronaBlue[23][8];
	static uint8_t m_nSunCoronaGreen[23][8];
	static uint8_t m_nSunCoronaRed[23][8];
	static uint8_t m_nWaterFogAlpha[23][8];
    static CVector m_vecDirnLightToSun;
};