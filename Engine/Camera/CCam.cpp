#include "StdInc.h"

static CVector DWCineyCamLastUp;
static CVector DWCineyCamLastRight;
static CVector DWCineyCamLastFwd;
static float DWCineyCamLastFov;
static float DWCineyCamLastNearClip;
static CVector DWCineyCamLastPos;
static size_t gLastFrameProcessedDWCineyCam;

void CCam::CacheLastSettingsDWCineyCam()
{
    DWCineyCamLastUp = Up;
    DWCineyCamLastRight = CrossProduct(Front, Up);
    DWCineyCamLastFwd = Front;
    DWCineyCamLastFov = FOV;
    DWCineyCamLastNearClip = Scene.Camera->nearPlane;
    DWCineyCamLastPos = Source;
}

bool CCam::Using3rdPersonMouseCam()
{
    return CCamera::m_bUseMouse3rdPerson && Mode == 4;
}

void CCam::Finalise_DW_CineyCams(CVector* a2, CVector* a3, float a4, float a5, float a6, float a7)
{
    Front = *a3 - *a2;
    Front.Normalise();
    Source = *a2;
    Up = CVector(sin(a4), 0.0f, cos(a4));
    CVector right = CrossProduct(Front, Up);
    right.Normalise();
    Up = CrossProduct(right, Front);
    if(Front.x == 0.0f && Front.y == 0.0f)
    {
        Front.x = 1.0f/10000.0f;
        Front.y = 1.0f/10000.0f;
    }
    right = CrossProduct(Front, Up);
    right.Normalise();
    Up = CrossProduct(right, Front);
    FOV = a5;
    RwCameraSetNearClipPlane(gScene.camera, 0.4f);
    CacheLastSettingsDWCineyCam();
    gLastFrameProcessedDWCineyCam = CTimer::GetFrameCounter();
    gHandShaker.Process(a7);
    Front = Multiply3x3(Front, &gHandShaker.mat);
    Front.Normalise();
    Source = *a2;
    float angle = gHandShaker.field_8 * a7;
    Up = CVector(sin(angle), 0.0f, cos(angle));
    right = CrossProduct(Front, Up);
    right.Normalise();
    Up = CrossProduct(right, Front);
    if (Front.x == 0.0f && Front.y == 0.0f)
    {
        Front.x = 1.0f/10000.0f;
        Front.y = 1.0f/10000.0f;
    }
    right = CrossProduct(Front, Up);
    Up = CrossProduct(right, Front);
}

void CCam::GetCoreDataForDWCineyCamMode(CEntity** targetEntity, CVehicle** targetVehicle, CVector* targetPos, CVector* dwCamLastPos, CVector* at, CVector* right, CVector* top, CVector* linearVelocity, float* linearSpeed, CVector* angularVelocity, float* angularSpeed, CColSphere* colSphere)
{
    *targetEntity = CamTargetEntity;
    *targetVehicle = (CVehicle*)CamTargetEntity;
    *targetPos = CamTargetEntity->GetPos();
    *dwCamLastPos = DWCineyCamLastPos;
    *at = CamTargetEntity->GetAt();
    *right = CamTargetEntity->GetRight();
    *top = CamTargetEntity->GetTop();
    *linearVelocity = CamTargetEntity->GetLinearVelocity();
    *linearSpeed = linearVelocity->Magnitude();
    *angularVelocity = CamTargetEntity->GetAngularVelocity();
    *angularSpeed = angularVelocity->Magnitude();
    float radius = CModelInfo::GetModelInfo(CamTargetEntity->GetModelIndex())->GetColModel()->GetBoundingBox().GetRadius();
    CVector centre;
    CamTargetEntity->GetBoundCentre(centre);
    colSphere->Set(radius, centre, 0, 0, -1);
}

void CCam::Process_Rocket(const CVector& a2, float a3, float a4, float a5, bool a6)
{
    if (CamTargetEntity->GetType() == ENTITY_TYPE_PED)
    {
        CPed* ped = (CPed*)CamTargetEntity;
        FOV = 70.0f;
        if (ResetStatics)
        {
            if (!CCamera::m_bUseMouse3rdPerson || ped->pAutoAimTarget)
            {
                m_fVerticalAngle = 0.0f;
                m_fHorizontalAngle = ped->fCurrentRotation - M_PI/2;
                m_fInitialPlayerOrientation = m_fHorizontalAngle;
            }
            else
            {
                m_fInitialPlayerOrientation = m_fHorizontalAngle;
            }
            ResetStatics = 0;
            byte_B70000 = 0;
            dword_B6FFFC = 0;
            dword_B6FFF8 = 0;
            m_bCollisionChecksOn = 1;
        }
        if (ped->GetRwObject())
        {
            ped->UpdateRwMatrix();
        }
        ped->UpdateRwFrame();
        ped->UpdateRpHAnim();
        ped->GetBonePosition(Source, 5, 1);
        Source.z += 0.1f;

        CPad::GetPad(0);
        if(CPad::NewMouseControllerState.X != 0.0 || CPad::NewMouseControllerState.Y != 0.0)
        {
            m_fHorizontalAngle += CPad::NewMouseControllerState.X * -3.0 * FOV / 80.0f * CCamera::m_fMouseAccelHorzntl;
            m_fVerticalAngle += CPad::NewMouseControllerState.Y * 4.0 * FOV / 80.0f * CCamera::m_fMouseAccelVertical;
        }
        else
        {
            float h = -CPad::AimWeaponLeftRight(CPad::GetPad(0), ped);
            float dir_h = h < 0.0f ? -1.0f : 1.0f;
            float v = CPad::AimWeaponUpDown(CPad::GetPad(0), pad);
            float dir_v = v < 0.0f ? -1.0f : 1.0f;
            m_fHorizontalAngle += dir_h * h * h / 10000.0f * (FOV / 80.0f) / 17.5f * CTimer::GetTimeStep();
            m_fVerticalAngle += v * v / 10000.0f * (FOV / 80.0f) / 14.0f * CTimer::GetTimeStep() * dir_v;
        }
        sub_509C50(pThis);
        if ( pThis->m_fVerticalAngle <= 1.047197580337524 )
        {
          if ( pThis->m_fVerticalAngle < -1.562069773674011 )
            LODWORD(pThis->m_fVerticalAngle) = -1077415449;
        }
        else
        {
          LODWORD(pThis->m_fVerticalAngle) = 1065749138;
        }
        Front = CVector(-(cos(m_fHorizontalAngle) * cos(m_fVerticalAngle)),
                        -(sin(m_fHorizontalAngle) * cos(m_fVerticalAngle)),
                        sin(m_fVerticalAngle));
        GetVectorsReadyForRW();
        TheCamera.pTargetEntity->fCurrentRotation = CGeneral::GetATanOfXY(pThis->Front.x, pThis->Front.y) - M_PI/2;
        TheCamera.pTargetEntity->fTargetRotation = TheCamera.pTargetEntity->fCurrentRotation;
        if ( a6 ) // mark target
        {
            CPlayerPed* playerPed = FindPlayerPed(-1);
            CPlayerInfo* playerData = playerPed->pPlayerData;
            if(!playerData->FireHSMisslePressedTime)
            {
                playerData->FireHSMisslePressedTime = CTimer::GetCurrentTimeMs();
            }
          CEntity* hsTarget = CWeapon::PickTargetForHeatSeekingMissile(Source, Front, 1.2f, playerPed, 0, playerData->m_LastHSMissleTarget);
          if ( hsTarget && (CTimer::GetCurrentTimeMs() - playerData->LastHSMissleLOSTime) > 1000)
          {
              playerData->LastHSMissleLOSTime = CTimer::GetCurrentTimeMs();
                playerPed->BitFlags &= 0xFFFFFFFEu;
                hsTarget->BitFlags &= 0xFFFFFFFEu;
                playerPed->BitFlags ^= ((unsigned __int8)v31->__parent.__parent.BitFlags ^ (playerPed->BitFlags & 1)) & 1;
                hsTarget->BitFlags ^= ((unsigned __int8)hsTarget->BitFlags ^ (hsTarget->BitFlags & 1)) & 1;
                playerData->m_bLastHSMissileLOS = CWorld::GetIsLineOfSightClear(playerPos->GetPos(), hsTarget->GetPos(), 1, 1, 0, 1, 0, 1i64);
          }
          if(playerData->m_bLastHSMissileLOS)
          {
            if ( v34 && v34 == (CEntity *)v30->m_LastHSMissleTarget )
              goto LABEL_41;
          }
          else
          {
            hsTarget = NULL;
          }
          if ( hsTarget )
          {
                CWeaponEffects__MarkTarget(0, hsTarget.GetPos(), -1, -1, -1, 100, 0x3FA66666, 1);
          }
          else
          {
              playerData->FireHSMisslePressedTime = CTimer::GetCurrentTimeMs();
          }
          gCrossHair[0].field_4 = 0;
          gCrossHair[0].field_14 = -1;
          if(CTimer::GetCurrentTimeMs() - v30->FireHSMisslePressedTime <= 1500)
          {
            gCrossHair[0].field_15 = -1;
            gCrossHair[0].field_16 = -1;
            gCrossHair[0].field_24 = 0;
          }
          else
          {
            gCrossHair[0].field_15 = 0;
            gCrossHair[0].field_16 = 0;
            gCrossHair[0].field_24 = 1065353216;
          }
          playerData->m_LastHSMissleTarget = hsTarget;
        }
    }
    return RwCameraSetNearClipPlane(Scene.Camera, flt_8CCC9C);
}

void CCam::Process_SpecialFixedForSyphon(const CVector& target, float, float, float)
{
    Source = m_cvecCamFixedModeSource;
    m_cvecTargetCoorsForFudgeInter = target;
    m_cvecTargetCoorsForFudgeInter.z += m_fSyphonModeTargetZOffSet;
    CVector target_ = target;
    Front = target_ - Source;
    TheCamera.AvoidTheGeometry(m_cvecCamFixedModeSource, m_cvecTargetCoorsForFudgeInter, Source, FOV);
    Front.z += m_fSyphonModeTargetZOffSet;
    GetVectorsReadyForRW();
    Up += m_cvecCamFixedModeUpOffSet;
    Up.Normalise();
    CVector UxF = CrossProduct(Up, Front);
    UxF.Normalise();
    Front = CrossProduct(UxF, Up);
    Front.Normalise();
    FOV = 70.0f;

    CPed* ped = reinterpret_cast<CPed*>(CamTargetEntity);
    if (CamTargetEntity && CamTargetEntity->GetType() == ENTITY_TYPE_PED && ped->pAutoAimTarget )
    {
        CWeaponInfo* weapInfo = CWeaponInfo::GetWeaponInfo(ped->Weapons[ped->bCurrentWeaponSlot].m_eWeaponType, ped->GetWeaponSkill());
        if (weapInfo && !weapInfo->m_aimWithArm || ped->bIsDucking && weapInfo->m_fireType != WEAPON_FIRE_TYPE_MELEE)
        {
            CVector pedToTarget = ped->pAutoAimTarget->GetPos() - ped->GetPos();
            ped->fTargetRotation = atan2(m_cvecCamFixedModeSource.y, -pedToTarget.x);
            ped->fCurrentRotation = ped->fTargetRotation;
            ped->SetAngle(ped->fTargetRotation);
            ped->UpdateRW();
        }
    }
}

void CCam::GetVectorsReadyForRW()
{
    CVector up = Up;
    Up = CVector(0.0f, 0.0f, 1.0f);
    Front.Normalise();
    if (Front.x == 0.0f && Front.y == 0.0f)
    {
        Front.x = 0.0001f;
        Front.y = 0.0001f;
    }
    CVector FxU = CrossProduct(Front, up);
    FxU.Normalise();
    Up = CrossProduct(FxU, Front);
}