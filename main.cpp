#include "StdInc.h"

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
    if(bGammaChanged)
	{
		IDirect3DDevice9 *pDevice = (IDirect3DDevice9*)RwD3D9GetCurrentD3DDevice();
		//pDevice->0x54(0, 1, uiSavedGamma);
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
           RpAnimBlendPluginAttach() &&
           CTxdStore::PluginAttach() &&
           RpMatFXPluginAttach() &&
           RpUVAnimPluginAttach() &&
           CCustomBuildingRenderer::PluginAttach() &&
           CCarFxRenderer::RegisterPlugins() &&
           RpAnisotPluginAttach() &&
           BreakablePluginAttach() &&
           CCollisionPlugin::PluginAttach() &&
           C2dEffect::PluginAttach() &&
           PipelinePluginAttach();
}

static void Render(void)
{
    RwCameraClear(Camera, &BackgroundColor, rwCAMERACLEARZ|rwCAMERACLEARIMAGE);

    if( RwCameraBeginUpdate(Camera) )
    {

        RwCameraEndUpdate(Camera);
    }

    /* 
     * Display camera's raster...
     */
    RsCameraShowRaster(Camera);

    return;
}

static void Idle(void *param)
{
    // TODO: some more timer stuff at start
    CTimer::Update();
    CSprite2d::InitPerFrame();
    CFont::InitPerFrame();
    CPointLights::NumLights = 0;
    CGame::Process();
    AudioEngine.Service();
    SetLightsWithTimeOfDayColour(Scene);
    bool bOk = param;
    if(bOk)
    {
        if(FrontEndMenuManager.bMenuActive || RsCameraShowRaster(Camera) == 2)
        {
            CDraw::CalculateAspectRatio();
            CameraSize(Camera, (RwRect *)param, tan(CDraw::ms_fFOV * 0.0087266), CDraw::ms_fAspectRatio);
            CVisibilityPlugins::SetRenderWareCamera(Camera);
            RwCameraClear(Camera, gColourTop, 2);
            bool bCamOk = RsCameraBeginUpdate(Camera);
            if(!bCamOk)
            {
                return bCamOk;
            }
            goto LABEL_16;
        }
        RwV2D mousePos;
        mousePos.x = RsGlobal.maximumWidth * 0.5;
        mousePos.y = RsGlobal.maximumHeight * 0.5;
        RsMouseSetPos(&mousePos);
        CRenderer::ConstructRenderList();
        CRenderer::PreRender();
        CWorld::ProcessPedsAfterPreRender();
        g_realTimeShadowMan.Update();
        CMirrors::BeforeMainRender();
        short sColours[7];
        if(CWeather::LightingFlash)
        {
            sColours[0] = 255;
            sColours[1] = 255;
            sColours[2] = 255;
            sColours[3] = 255;
            sColours[4] = 255;
            sColours[5] = 255;
            sColours[6] = 255;
        }
        else
        {
            sColours[0] = CTimeCycle::m_CurrentColours[0];
            sColours[1] = CTimeCycle::m_CurrentColours[1];
            sColours[2] = CTimeCycle::m_CurrentColours[2];
            sColours[3] = CTimeCycle::m_CurrentColours[3];
            sColours[4] = CTimeCycle::m_CurrentColours[4];
            sColours[5] = CTimeCycle::m_CurrentColours[5];
            sColours[6] = 255;
        }
        bOk = DoRWStuffStartOfFrame_Horizon(sColours[0], sColours[1], sColours[2], sColours[3], sColours[4], sColours[5], sColours[6]);
        if(bOk)
        {
            DefinedState();
            RwCameraSetFarClipPlane(Camera, CTimeCycle::0x50);
            RwCameraSetFogDistanceMacro(Camera, CTimeCycle::0x54);
            CMirrors::RenderMirrorBuffer();
            RenderScene();
            CVisibilityPlugins::RenderWeaponPedsForPC();
            // HERE: called unk func: sub_53E8D0
            RenderEffects();
            if(TheCamera.uiBlurType == 0 || TheCamera.uiBlurType == 2)
            {
                if(TheCamera.fPositionAlongSpline > 0.0)
                {
                    TheCamera.SetMotionBlurAlpha(150);
                }
            }
            TheCamera.RenderMotionBlur();
            Render2dStuff();
LABEL_16:
            if(FrontEndMenuManager.bMenuActive)
            {
                FrontEndMenuManager.RenderMenus();
            }
            RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);
            DoFade();
            CHud::DrawAfterFade();
            // sub_69EFC0
            // dummy_71A210
            if(CCredits::bCreditsGoing)
            {
                if(!FrontEndMenuManager.bMenuActive)
                {
                    CCredits::Render();
                }
                if(CCredits::bCreditsGoing)
                {
                    if(!FrontEndMenuManager.bMenuActive)
                    {
                        CCredits::Render();
                    }
                }
            }
            DoRwStuffEndOfFrame(true);
        }
    }
    // Render(); here?
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
            CameraSize(Scene.Camera, (RwRect *)param, tan(CDraw::ms_fFOV * 0.0087266), DEFAULT_ASPECTRATIO);
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

