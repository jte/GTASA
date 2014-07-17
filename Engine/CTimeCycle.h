#pragma once

class CTimeCycle
{
public:
	* AddOne(CBox&, short, int, float, float, float)
	* CalcColoursForPoint(CVector, CColourSet*)
	* FindFarClipForCoors(CVector)
	* FindTimeCycleBox(CVector, CTimeCycleBox**, float*, bool, bool, CTimeCycleBox*)
	* GetAmbientBlue()
	* GetAmbientBlue_BeforeBrightness()
	* GetAmbientBlue_Obj()
	* GetAmbientGreen()
	* GetAmbientGreen_BeforeBrightness()
	* GetAmbientGreen_Obj()
	* GetAmbientRed()
	* GetAmbientRed_BeforeBrightness()
	* GetAmbientRed_Obj()
	* InitForRestart()
	* Initialise(bool)
	* SetConstantParametersForPostFX()
	* Shutdown()
	* StartExtraColour(int, bool)
	* StopExtraColour(bool)
	* Update()
protected:
private:
	* m_BelowHorizonGrey
	* m_BrightnessAddedToAmbientBlue
	* m_BrightnessAddedToAmbientGreen
	* m_BrightnessAddedToAmbientRed
	* m_CurrentColours
	* m_CurrentStoredValue
	* m_ExtraColour
	* m_ExtraColourInter
	* m_ExtraColourWeatherType
	* m_FogReduction
	* m_NumBoxes
	* m_VectorToSun
	* m_aBoxes
	* m_bExtraColourOn
	* m_fCloudAlpha
	* m_fFarClip
	* m_fFogStart
	* m_fLightsOnGroundBrightness
	* m_fPostFx1Alpha
	* m_fPostFx1Blue
	* m_fPostFx1Green
	* m_fPostFx1Red
	* m_fPostFx2Alpha
	* m_fPostFx2Blue
	* m_fPostFx2Green
	* m_fPostFx2Red
	* m_fShadowDisplacementX
	* m_fShadowDisplacementY
	* m_fShadowFrontX
	* m_fShadowFrontY
	* m_fShadowSideX
	* m_fShadowSideY
	* m_fSpriteBrightness
	* m_fSpriteSize
	* m_fSunSize
	* m_fWaterAlpha
	* m_fWaterBlue
	* m_fWaterGreen
	* m_fWaterRed
	* m_nAmbientBlue
	* m_nAmbientBlue_Obj
	* m_nAmbientGreen
	* m_nAmbientGreen_Obj
	* m_nAmbientRed
	* m_nAmbientRed_Obj
	* m_nDirectionalMult
	* m_nFluffyCloudsBottomBlue
	* m_nFluffyCloudsBottomGreen
	* m_nFluffyCloudsBottomRed
	* m_nHighLightMinIntensity
	* m_nLightShadowStrength
	* m_nLowCloudsBlue
	* m_nLowCloudsGreen
	* m_nLowCloudsRed
	* m_nPoleShadowStrength
	* m_nShadowStrength
	* m_nSkyBottomBlue
	* m_nSkyBottomGreen
	* m_nSkyBottomRed
	* m_nSkyTopBlue
	* m_nSkyTopGreen
	* m_nSkyTopRed
	* m_nSunCoreBlue
	* m_nSunCoreGreen
	* m_nSunCoreRed
	* m_nSunCoronaBlue
	* m_nSunCoronaGreen
	* m_nSunCoronaRed
	* m_nWaterFogAlpha
	* m_vBlueGrade
	* m_vGreenGrade
	* m_vRedGrade
	* m_vecDirnLightToSun
};