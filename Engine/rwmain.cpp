#include "StdInc.h"

static bool bGammaChanged;
static D3DGAMMARAMP savedGamma;
static RwRGBA gColourClear;

bool DoRWStuffStartOfFrame_Horizon(short, short, short, short, short, short, short);
void DefinedState();

static RwBool Initialize(void)
{
    RsGlobal.appName = RWSTRING("GTA: San Andreas");
    RsGlobal.maximumWidth = DEFAULT_SCREEN_WIDTH;
    RsGlobal.maximumHeight = DEFAULT_SCREEN_HEIGHT;
    RsGlobal.ps = (void*)30;
    //RsGlobal.frameLimit = 30;
    RsGlobal.quit = false;
    RsGlobal.keyboard.inputDeviceType = rsKEYBOARD;
    RsGlobal.keyboard.inputEventHandler = NULL;
    RsGlobal.keyboard.used = false;
    RsGlobal.mouse.inputDeviceType = rsMOUSE;
    RsGlobal.mouse.inputEventHandler = NULL;
    RsGlobal.mouse.used = false;
    RsGlobal.pad.inputDeviceType = rsPAD;
    RsGlobal.pad.inputEventHandler = NULL;
    RsGlobal.pad.used = false;
    return RsInitialize();
}

static RwBool Initialize3D(void *param)
{
    return RsRwInitialize(param) && CGame::InitialiseRenderWare();
}

static void Terminate3D(void)
{
    CGame::ShutdownRenderWare();
    if (bGammaChanged)
	{
		IDirect3DDevice9 *pDevice = (IDirect3DDevice9*)RwD3D9GetCurrentD3DDevice();
        pDevice->SetGammaRamp(0, D3DSGR_CALIBRATE, &savedGamma);
		bGammaChanged = false; 
	}
    RsRwTerminate();
}

static RwBool AttachPlugins(void)
{
	return RpWorldPluginAttach() &&
		   RpSkinPluginAttach() &&
           RtAnimInitialize() &&
           RpHAnimPluginAttach() &&
           NodeNamePluginAttach() &&
           CVisibilityPlugins::PluginAttach() &&
         //  RpAnimBlendPluginAttach() &&
           CTxdStore::PluginAttach() &&
           RpMatFXPluginAttach() &&
           RpUVAnimPluginAttach() &&
           CCustomBuildingRenderer::PluginAttach() &&
         //  CCarFxRenderer::RegisterPlugins() &&
           RpAnisotPluginAttach() &&
         //  BreakablePluginAttach() &&
         //  CCollisionPlugin::PluginAttach() &&
         //  C2dEffect::PluginAttach() &&
           PipelinePluginAttach();
}

static void Idle(void *param)
{
    CTimer::GetCurrentTimeInCycles();
    CTimer::GetCyclesPerMillisecond();
    do
        v5 = CTimer::GetCurrentTimeInCycles();
    while ( v5 / CTimer::GetCyclesPerMillisecond() - _lastTickTime < 14 );
    v6 = CTimer::GetCurrentTimeInCycles();
    _lastTickTime = v6 / CTimer::GetCyclesPerMillisecond();
    CTimer::Update();
    CSprite2d::InitPerFrame();
    CFont::InitPerFrame();
    CPointLights::NumLights = 0;
    CGame::Process();
    AudioEngine.Service();
    SetLightsWithTimeOfDayColour(gScene.world);
    if (!param)
    {
        return;
    }
    if ( FrontEndMenuManager.bMenuActive || CCamera::GetScreenFadeStatus(&TheCamera) == 2 )
    {
        CDraw::CalculateAspectRatio();
        v8 = tan(CDraw::GetFOV() * 0.0087266462);
        CameraSize(gScene.camera, 0, v8, CDraw::GetAspectRatio());
        CVisibilityPlugins::SetRenderWareCamera(gScene.camera);
        RwCameraClear(gScene.camera, &gColourClear, 2);
        result = RwCameraBeginUpdate(gScene.camera);
        if ( !v9 )
            return result;
    }
    else
    {
        RwV2d pos;
        pos.x = RsGlobal.maximumWidth * 0.5;
        pos.y = RsGlobal.maximumHeight * 0.5;
        RsMouseSetPos(&pos);
        CRenderer::ConstructRenderList();
        CRenderer::PreRender();
        CWorld::ProcessPedsAfterPreRender();
        g_realTimeShadowMan.Update();
        CMirrors::BeforeMainRender();
        if ( CWeather::LightningFlash )
        {
            _1 = 255;
            _7 = 255;
            _6 = 255;
            _5 = 255;
            _4 = 255;
            _3 = 255;
            CTimeCycle::m_CurrentColours.m_skyBottomRed = 255;
            CTimeCycle::m_CurrentColours.m_skyBottomGreen = 255;
            CTimeCycle::m_CurrentColours.m_skyBottomBlue = 255;
            _2 = 255;
        }
        else
        {
            _7 = 255;
            _6 = CTimeCycle::m_CurrentColours.m_skyBottomBlue;
            _5 = CTimeCycle::m_CurrentColours.m_skyBottomGreen;
            _4 = CTimeCycle::m_CurrentColours.m_skyBottomRed;
            _1 = CTimeCycle::m_CurrentColours.m_skyTopRed;
            _3 = CTimeCycle::m_CurrentColours.m_skyTopBlue;
            _2 = CTimeCycle::m_CurrentColours.m_skyTopGreen;
        }
        if (!DoRWStuffStartOfFrame_Horizon(_1, _2, _3, _4, _5, _6, _7))
        {
            return;
        }
        DefinedState();
        RwCameraSetFarClipPlane(gScene.camera, CTimeCycle::m_CurrentColours.m_farClip);
        gScene.camera->fogPlane = CTimeCycle::m_CurrentColours.m_fogStart;
        CMirrors::RenderMirrorBuffer();
        RenderScene();
        CVisibilityPlugins::RenderWeaponPedsForPC();
        sub_53E8D0(&CVisibilityPlugins::ms_weaponPedsForPC);
        RenderEffects();
        if (TheCamera.uiBlurType == 0 || TheCamera.uiBlurType == 2)
        {
            if (TheCamera.fPositionAlongSpline > 0.0)
            {
                TheCamera.SetMotionBlurAlpha(150);
            }
        }
        CCamera::RenderMotionBlur(&TheCamera);
        Render2dStuff();
    }
    if (FrontEndMenuManager.bMenuActive)
    {
        RenderMenus(&FrontEndMenuManager, result, v3);
    }
    RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);
    DoFade();
    CHud::DrawAfterFade();
    sub_69EFC0(0);
    dummy_71A210();
    if ( CCredits::bCreditsGoing )
    {
        if ( !FrontEndMenuManager.bMenuActive )
        {
            CCredits::Render();
        }
        if ( CCredits::bCreditsGoing )
        {
            if ( !FrontEndMenuManager.bMenuActive )
            {
                CCredits::Render();
            }
        }
    }
    nullsub_24();
    nullsub_63();
    RwCameraEndUpdate(gScene.camera);
    RsCameraShowRaster(gScene.camera);
}

RsEventStatus AppEventHandler(RsEvent event, void *param)
{
    switch(event)
    {
        case rsINITIALIZE:
        {
            CGame::InitialiseOnceBeforeRW();
            return Initialize() ? rsEVENTPROCESSED : rsEVENTERROR;
        }

        case rsCAMERASIZE:
        {
            CameraSize(gScene.camera, (RwRect *)param, tan(CDraw::GetFOV() * M_PI / 360), DEFAULT_ASPECTRATIO);
            return rsEVENTPROCESSED;
        }

        case rsRWINITIALIZE:
        {
            return Initialize3D(param) ? rsEVENTPROCESSED : rsEVENTERROR;
        }

        case rsRWTERMINATE:
        {
            Terminate3D();
            return rsEVENTPROCESSED;
        }

        case rsPLUGINATTACH:
        {
            return AttachPlugins() ? rsEVENTPROCESSED : rsEVENTERROR;
        }

        case rsINPUTDEVICEATTACH:
        {
            AttachInputDevices();

            return rsEVENTPROCESSED;
        }

        case rsIDLE:
        {
            Idle(param);

            return rsEVENTPROCESSED;
        }

		case rsQUITAPP:
		{
			CGame::FinalShutdown();
			
			return rsEVENTPROCESSED;
		}

        default:
        {
            return rsEVENTNOTPROCESSED;
        }
    }
}

bool DoRWStuffStartOfFrame_Horizon(short, short, short, short, short, short, short)
{
    CDraw::CalculateAspectRatio();
    CameraSize(gScene.camera, 0, tan(CDraw::GetFOV() * 0.0087266462), CDraw::GetAspectRatio());
    CVisibilityPlugins::SetRenderWareCamera(gScene.camera);
    RwCameraClear(gScene.camera, &gColourClear, 6);
    if (!RwCameraBeginUpdate(gScene.camera))
    {
        return false;
    }
    DefinedState();
    CMatrix::SetFromPtr(&TheCamera.m_viewMatrix);
    CClouds::RenderSkyPolys();
    return true;
}

void DefinedState()
{
    RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, (void*)rwTEXTUREADDRESSWRAP);
    RwRenderStateSet(rwRENDERSTATETEXTUREPERSPECTIVE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEGOURAUD);
    RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, FALSE);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEBORDERCOLOR, (void*)RWRGBALONG(0, 0, 0, 255));
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, FALSE);
    RwRenderStateSet(rwRENDERSTATEFOGCOLOR, (void*)RWRGBALONG(CTimeCycle::m_CurrentColours.m_skyBottomRed, CTimeCycle::m_CurrentColours.m_skyBottomGreen, CTimeCycle::m_CurrentColours.m_skyBottomBlue, 255));
    RwRenderStateSet(rwRENDERSTATEFOGTYPE, (void*)rwFOGTYPELINEAR);
    RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
    RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)rwALPHATESTFUNCTIONGREATER);
    RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, (void*)2);
}