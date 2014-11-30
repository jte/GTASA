#include "StdInc.h"

bool CSprite::CalcScreenCoors(const RwV3d& worldPos, RwV3d* viewPos, float* screenX, float* screenY, bool checkFarClip, bool checkNearClip)
{
    viewPos = TheCamera.CameraViewMatrix * worldPos;
    if(checkNearClip)
    {
	    if(viewPos->z - 1.0f <= CDraw::GetNearClip())
	    {
		    return false;
	    }
    }
    if(checkFarClip)
    {
	    if(viewPos->z <= CDraw::GetFarClip())
	    {
		    return false;
	    }
    }
    //
    *screenX = RsGlobal.maximumWidth / viewPos->z;
    *screenY = RsGlobal.maximumHeight / viewPos->z;
    // Project to 2D plane
    viewPos->x *= *screenX;
    viewPos->y *= *screenY;
    // Adjust for FOV
    *screenX /= CDraw::GetFOV() * 70.0f;
    *screenY /= CDraw::GetFOV() * 70.0f;
    return true;
}

float CSprite::CalcHorizonCoors()
{
    CVector waterHorizon = TheCamera.GetPos();
    waterHorizon.x += TheCamera.CamFrontYNorm * 3000.0f;
    waterHorizon.y += TheCamera.DistanceToWater * 3000.0f;
    waterHorizon.z = 0;
    // Transform world to view projection
    waterHorizon *= TheCamera.cameraViewMatrix;
    // Project to 2D plane
    return (RsGlobal.maximumHeight * waterHorizon.y) / waterHorizon.z;
}

void CSprite::InitSpriteBuffer()
{
    m_f2DNearScreenZ = RwIm2DGetNearScreenZ();
    m_f2DFarScreenZ = RwIm2DGetFarScreenZ();
}

void CSprite::InitSpriteBuffer2D()
{
    m_fRecipNearClipPlane = 1.0f / Scene.Camera->nearPlane;
    InitSpriteBuffer();
}