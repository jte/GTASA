#pragma once

class CPostEffects
{
public:
	* CCTV()
	* Close()
	* ColourFilter(RwRGBA, RwRGBA)
	* DarknessFilter(int)
	* DoScreenModeDependentInitializations()
	void DrawQuad(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a, RwRaster* raster);
	* DrawQuadSetDefaultUVs()
	* DrawQuadSetPixelUVs(float, float, float, float, float, float, float, float)
	* DrawQuadSetUVs(float, float, float, float, float, float, float, float)
	* FilterFX_RestoreDayNightBalance()
	* FilterFX_StoreAndSetDayNightBalance()
	* Fog()
	* Grain(int, bool)
	* HeatHazeFX(float, bool)
	* HeatHazeFXInit()
	* ImmediateModeFilterStuffInitialize()
	* ImmediateModeRenderStatesReStore()
	* ImmediateModeRenderStatesSet()
	* ImmediateModeRenderStatesStore()
	* InfraredVision(RwRGBA, RwRGBA)
	* InfraredVisionRestoreLightsForHeatObjects()
	void InfraredVisionSetLightsForDefaultObjects();
	* InfraredVisionSetLightsForHeatObjects()
	* InfraredVisionStoreAndSetLightsForHeatObjects(CPed*)
	void Initialise();
	bool IsVisionFXActive();
	* MobileRender()
	* NightVision(RwRGBA)
	void NightVisionSetLights();
	* Radiosity(int, int, int, int)
	* RasterCreatePostEffects(RwRect)
	* Render()
	* ScriptCCTVSwitch(bool)
	* ScriptDarknessFilterSwitch(bool, int)
	* ScriptHeatHazeFXSwitch(bool)
	* ScriptInfraredVisionSwitch(bool)
	* ScriptNightVisionSwitch(bool)
	* ScriptResetForEffects()
	* SetFilterMainColour(RwRaster*, RwRGBA)
	* SetupBackBufferVertex()
	* SpeedFX(float)
	* UnderWaterRipple(RwRGBA, float, float, int, float, float)
	* UnderWaterRippleFadeToFX()
	* Update()
protected:
private:
	* SCREEN_EXTRA_MULT_BASE_CAP
	* SCREEN_EXTRA_MULT_BASE_MULT
	* SCREEN_EXTRA_MULT_CHANGE_RATE
	* m_CCTVcol
	* m_DarknessFilterAlpha
	* m_DarknessFilterAlphaDefault
	* m_DarknessFilterRadiosityIntensityLimit
	* m_HeatHazeFXHourOfDayEnd
	* m_HeatHazeFXHourOfDayStart
	* m_HeatHazeFXIntensity
	* m_HeatHazeFXRandomShift
	* m_HeatHazeFXRenderSizeX
	* m_HeatHazeFXRenderSizeY
	* m_HeatHazeFXScanSizeX
	* m_HeatHazeFXScanSizeY
	* m_HeatHazeFXSpeedMax
	* m_HeatHazeFXSpeedMin
	* m_HeatHazeFXType
	* m_HeatHazeFXTypeLast
	* m_InfraredVisionCol
	* m_InfraredVisionGrainStrength
	* m_InfraredVisionMainCol
	* m_NightVisionGrainStrength
	* m_NightVisionMainCol
	* m_RadiosityFilterPasses
	* m_RadiosityFilterUCorrection
	* m_RadiosityFilterVCorrection
	* m_RadiosityIntensity
	* m_RadiosityIntensityLimit
	* m_RadiosityPixelsX
	* m_RadiosityPixelsY
	* m_RadiosityRenderPasses
	* m_SeamRemoverMode
	* m_SeamRemoverShiftBottomRight
	* m_SeamRemoverShiftTopLeft
	* m_SpeedFXAlpha
	* m_VisionFXDayNightBalance
	* m_bCCTV
	* m_bColorEnable
	* m_bDarknessFilter
	* m_bDisableAllPostEffect
	* m_bFog
	* m_bGrainEnable
	* m_bHeatHazeFX
	* m_bHeatHazeMaskModeTest
	* m_bHilightEnable
	* m_bInCutscene
	* m_bInfraredVision
	* m_bNightVision
	* m_bRadiosity
	* m_bRadiosityBypassTimeCycleIntensityLimit
	* m_bRadiosityDebug
	* m_bRadiosityLinearFilter
	* m_bRadiosityStripCopyMode
	* m_bRainEnable
	* m_bSavePhotoFromScript
	* m_bSeamRemover
	* m_bSeamRemoverDebugMode
	* m_bSeamRemoverSeamSearchMode
	* m_bSpeedFX
	* m_bSpeedFXTestMode
	* m_bSpeedFXUserFlag
	* m_bSpeedFXUserFlagCurrentFrame
	* m_bWaterDepthDarkness
	* m_colour1Multiplier
	* m_colour2Multiplier
	* m_colourBottomVOffset
	* m_colourLeftUOffset
	* m_colourRightUOffset
	* m_colourTopVOffset
	* m_defScreenXPosn
	* m_defScreenYPosn
	* m_fHeatHazeFXFadeSpeed
	* m_fHeatHazeFXInsideBuildingFadeSpeed
	* m_fInfraredVisionFilterRadius
	* m_fInfraredVisionHeatObjectCol
	* m_fInfraredVisionSwitchOnFXCount
	* m_fNightVisionSwitchOnFXCount
	* m_fNightVisionSwitchOnFXTime
	* m_fSpeedFXManualSpeedCurrentFrame
	* m_fWaterFXStartUnderWaterness
	* m_fWaterFullDarknessDepth
	* m_grainStrength
	* m_hilightMblur
	* m_hilightOffset
	* m_hilightScale
	* m_hilightStrength
	* m_pDmaPkt
	* m_pGrainRaster
	* m_smokeyDistance
	* m_smokeyEnable
	* m_smokeyStrength
	* m_waterCol
	* m_waterEnable
	* m_waterFreq
	* m_waterSpeed
	* m_waterStrength
	* m_xoffset
	* m_yoffset
	* ms_imf
	* pRasterFrontBuffer
};
