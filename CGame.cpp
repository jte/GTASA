#include "StdInc.h"

void CGame::InitialiseOnceBeforeRW()
{
    CMemoryMgr::Init();
    CLocalisation::Initialise();
    CFileMgr::Initialise();
    CdStreamInit(5);
    CPad::Initialise();
}

void CGame::ShutdownRenderWare()
{
    RwMatrixDestroy(m_pWorkingMatrix1);
    RwMatrixDestroy(m_pWorkingMatrix2);
    CLoadingScreen::Shutdown(0);
    CHud::Shutdown();
    CFont::Shutdown();
    for(size_t i = 0; i < ELEMS_COUNT(CWorld::Players); i++)
    {
        CWorld::Players[i].DeletePlayerSkin();
    }
    CPlayerSkin::Shutdown();
    DestroyDebugFont();
    LightsDestroy(Scene);
    CVisibilityPlugins::Shutdown();
    RpWorldRemoveCamera(Scene.World, Scene.Camera); // check this order
    RpWorldDestroy(Scene.World);               // of deletion
    CameraDestroy(Scene.Camera);
    Scene.World = NULL;
    Scene.Camera = NULL;
    // sub_730900 called here !?!
    CPostEffects::Close();
}

void CGame::FinalShutdown()
{
    ThePaths.Shutdown();
    CTempColModels::Shutdown();
    CTxdStore::Shutdown();
    CPedStats::Shutdown();
    CText::Unload(0);
    CdStreamShutdown();
}

void CGame::Process()
{
    CPad::UpdatePads();
    g_LoadMonitor.BeginFrame();
    // TODO:Timer mangling mingling
    if(!_mainMenuDisabled)
    {
        if(CTimer::m_CodePause == 0)
        {
            FrontEndMenuManager.Process();
        }
    }
    CTheZones::Update();
    CCover::Update();
    CAudioZones::Update(0, TheCamera.GetPos());
    CWindModifiers::Number = 0;
    if(CTimer::m_UserPause == 0 && CTimer::m_CodePause == 0)
    {
        // TODO: 
        CSprite2d::SetRecipNearClip();
    }
    CWaterLevel::PreRenderWater();
}

void CGame::Initialise(const char *szFileName)
{
    Init1(szFileName);
    CColAccel::startCache();
    CFileLoader::LoadLevel("DATA\\DEFAULT.DAT");
    CFileLoader::LoadLevel(szFileName);
    CColAccel::endCache();
    Init2(szFileName);
    // sub_70F9E0 called here
    LoadingScreen("Loading the Game", "Start script");
    CTheScripts::StartTestScript();
    CTheScripts::Process(szFileName);
    TheCamera.Process();
    Init3();
}

void CGame::Init1(const char *szFileName)
{
    CMaths::InitMathsTables();
    strcpy(aDatFile, szFileName);
    CPools::Initialise();
    CPlaceable::InitMatrixArray();
    CIniFile::LoadIniFile();
    // sub_730ac0 called here
    currLevel = 0;
    currArea = 0;
    gameTxdSlot = CTxdStore::AddTxdSlot("generic", "loadscs.txd", false);
    CTxdStore::Create(gameTxdSlot);
    CTxdStore::AddRef(gameTxdSlot);
    particleTxdSlot = CTxdStore::AddTxdSlot("particle", "loadscs.txd", false);
    CTxdStore::LoadTxd(particleTxdSlot, "MODELS\\PARTICLE.TXD");
    CTxdStore::AddRef(particleTxdSlot);
    CTxdStore::SetCurrentTxd(gameTxdSlot);
    CGameLogic::InitAtStartOfGame();
    CGangWars::InitAtStartOfGame();
    CConversations::Clear();
    CPedToPlayerConversations::Clear();
    CQuadTreeNode::InitPool();
    if(CPlantMgr::Initialise() && CCustomRoadsignMgr::Initialise())
    {
        CReferences__Init();
        CDebug__DebugInitTextBuffer();
        CTagManager__Init();
        CWeather__Init();
        CCover__Init();
        CCullZones__Init();
        COcclusion__Init();
        CCollision__Init();
        CBirds__Init();
        CEntryExitManager__Init();
        CStuntJumpManager__Init();
        CSetPieces__Init();
        CTheZones__Init();
        CUserDisplay__Init();
        CMessages__Init();
        CMessages__ClearAllMessagesDisplayedByGame(0);
        CVehicleRecording__Init();
        CRestart__Initialise();
        CWorld__Initialise();
        CAnimManager__Initialise();
        CCutsceneMgr__Initialise();
        CCarCtrl__Init();
        InitModelIndices();
        CModelInfo::Initialise();
        CPickups::Init();
        CTheCarGenerators::Init();
        CGarages::Init();
        CAudioZones::Init();
        CStreaming::InitImageList();
        CStreaming::ReadIniFile();
        ThePaths.Init();
        CPathFind::AllocatePathFindInfoMem();
        CTaskSimpleFight::LoadMeleeData();
        CCheat::ResetCheats();
        g_fx.Init();
        return true;
    }
    else
    {
        return false;
    }
}

bool CGame::InitialiseRenderWare()
{
    ValidateVersion();
    // sub_730830();
    CTxdStore::Initialise();
    CVisibilityPlugins::Initialise();
    Scene.Camera = CameraCreate(RsGlobal.maximumWidth, RsGlobal.maximumHeight, true);
    if(!Scene.Camera)
    {
        return false;
    }
    TheCamera.Init();
    TheCamera.SetRwCamera(Scene.Camera);
    RwCameraSetFarClipPlane(Scene.Camera, 2000.0f);
    RwCameraSetNearClipPlane(Scene.Camera, 0.9f);
    CameraSize(Scene.Camera, NULL, 0.7f, DEFAULT_ASPECTRATIO);
    RwBBox bbox;
    bbox.sup.x = 10000.0f;
    bbox.sup.y = 10000.0f;
    bbox.sup.z = 10000.0f;
    bbox.inf.x = -10000.0f;
    bbox.inf.y = -10000.0f;
    bbox.inf.z = -10000.0f;
    Scene.World = RpWorldCreate(&bbox);
    if(Scene.World)
    {
        CameraDestroy(Scene.Camera);
        Scene.Camera = NULL;
        return false;
    }
    RpWorldAddCamera(Scene.World, Scene.Camera);
    LightsCreate(Scene.World);
    CreateDebugFont();
    CFont::Initialise();
    CHud::Initialise();
    CPlayerSkin::Initialise();
    CPostEffects::Initialise();
    m_pWorkingMatrix1 = RwMatrixCreate();
    m_pWorkingMatrix2 = RwMatrixCreate();
}