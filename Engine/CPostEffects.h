#pragma once

class CPostEffects
{
public:
	/* CCTV()
	* Close()
	* ColourFilter(RwRGBA, RwRGBA)
	* DarknessFilter(int)
	* DoScreenModeDependentInitializations()*/
	static void DrawQuad(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a, RwRaster* raster);
	/* DrawQuadSetDefaultUVs()
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
	*/
    static void ImmediateModeRenderStatesStore();
	/* InfraredVision(RwRGBA, RwRGBA)
	* InfraredVisionRestoreLightsForHeatObjects()*/
	static void InfraredVisionSetLightsForDefaultObjects();
	/* InfraredVisionSetLightsForHeatObjects()
	* InfraredVisionStoreAndSetLightsForHeatObjects(CPed*)*/
	static void Initialise();
	static bool IsVisionFXActive();
	/* MobileRender()*/
	static void NightVision(RwRGBA color);
	static void NightVisionSetLights();
	/* Radiosity(int, int, int, int)
	* RasterCreatePostEffects(RwRect)
	* Render()
	*/ 
    static void ScriptCCTVSwitch(bool enable);
	static void ScriptDarknessFilterSwitch(bool enable, int alpha);
	static void ScriptHeatHazeFXSwitch(bool enable);
	static void ScriptInfraredVisionSwitch(bool enable);
	static void ScriptNightVisionSwitch(bool enable);
	static void ScriptResetForEffects();
	/* SetFilterMainColour(RwRaster*, RwRGBA)
	* SetupBackBufferVertex()
	* SpeedFX(float)
	* UnderWaterRipple(RwRGBA, float, float, int, float, float)
	* UnderWaterRippleFadeToFX()
	* Update()*/
protected:
private:
	/* SCREEN_EXTRA_MULT_BASE_CAP
	* SCREEN_EXTRA_MULT_BASE_MULT
	* SCREEN_EXTRA_MULT_CHANGE_RATE
	* m_CCTVcol
	*/ 
    static int m_DarknessFilterAlpha;
	static int m_DarknessFilterAlphaDefault;
	static int m_DarknessFilterRadiosityIntensityLimit;
	/* m_HeatHazeFXHourOfDayEnd
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
	*/ 
    static bool m_bCCTV;
	static bool m_bColorEnable;
	static bool m_bDarknessFilter;
	static bool m_bDisableAllPostEffect;
	static bool m_bFog;
	static bool m_bGrainEnable;
	static bool m_bHeatHazeFX;
	static bool m_bHeatHazeMaskModeTest;
	static bool m_bHilightEnable;
	static bool m_bInCutscene;
	static bool m_bInfraredVision;
	static bool m_bNightVision;
	static bool m_bRadiosity;
	static bool m_bRadiosityBypassTimeCycleIntensityLimit;
	static bool m_bRadiosityDebug;
	static bool m_bRadiosityLinearFilter;
	static bool m_bRadiosityStripCopyMode;
	static bool m_bRainEnable;
	static bool m_bSavePhotoFromScript;
	static bool m_bSeamRemover;
	static bool m_bSeamRemoverDebugMode;
	static bool m_bSeamRemoverSeamSearchMode;
	static bool m_bSpeedFX;
	static bool m_bSpeedFXTestMode;
	static bool m_bSpeedFXUserFlag;
	static bool m_bSpeedFXUserFlagCurrentFrame;
	static bool m_bWaterDepthDarkness;
	/* m_colour1Multiplier
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
	*/ 
    static float m_fNightVisionSwitchOnFXCount;
	static float m_fNightVisionSwitchOnFXTime;
	/* m_fSpeedFXManualSpeedCurrentFrame
	* m_fWaterFXStartUnderWaterness
	* m_fWaterFullDarknessDepth
	* m_grainStrength
	* m_hilightMblur
	* m_hilightOffset
	* m_hilightScale
	* m_hilightStrength
	* m_pDmaPkt
	*/ 
    static RwRaster* m_pGrainRaster;
	/* m_smokeyDistance
	* m_smokeyEnable
	* m_smokeyStrength
	* m_waterCol
	* m_waterEnable
	* m_waterFreq
	* m_waterSpeed
	* m_waterStrength
	* m_xoffset
	* m_yoffset
	*/ 
    static RwIm2DVertex ms_imf[4];
	static RwRaster* pRasterFrontBuffer;
};
