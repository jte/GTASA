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
    RwCameraSetNearClipPlane(Scene.Camera, 0.4f);
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
    colSphere.Set(radius, centre, 0, 0, -1);
}