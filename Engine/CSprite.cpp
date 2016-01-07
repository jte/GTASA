#include "StdInc.h"

bool CSprite::CalcScreenCoors(const RwV3d& worldPos, RwV3d* viewPos, float* screenX, float* screenY, bool checkFarClip, bool checkNearClip)
{
    viewPos = TheCamera.CameraViewMatrix * worldPos;
    if (checkNearClip)
    {
	    if(viewPos->z - 1.0f <= CDraw::GetNearClip())
	    {
		    return false;
	    }
    }
    if (checkFarClip)
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
    m_fRecipNearClipPlane = 1.0f / gScene.camera->nearPlane;
    InitSpriteBuffer();
}

void CSprite::RenderOneXLUSprite_Rotate_Aspect(float camSpaceX, float camSpaceY, float camSpaceZ, float width, float height, uint8_t red, uint8_t green, uint8_t blue, uint16_t alpha, float recipFarClip, float angle, uint8_t alpha2)
{
    if (camSpaceZ >= 2.3)
    {
        LOBYTE(v12) = blue;
        LOBYTE(v13) = green;
        LOBYTE(v14) = red;
        LOWORD(v15) = alpha;
    }
    else if (camSpaceZ < 1.3)
    {
        return;
    }
    else
    {
        float intensity = ((camSpaceZ - 1.3) * 255.0);
        v14 = (intensity * red) / 256.0f;
        v13 = (intensity * green) / 256.0f;
        v12 = (intensity * blue) / 256.0f;
        v15 = (intensity * alpha) / 256.0f;
    }
    float sinz = sin(angle);
    float cosz = cos(angle);
    
    float v29 = (-cosz - sinz)* width + camSpaceX;
    float v30 = (sinz - cosz) * width + camSpaceX;
    float v31 = (cosz + sinz) * width + camSpaceX;
    float v32 = (cosz - sinz) * width + camSpaceX;

    float v33 = (sinz - cosz) * height + camSpaceY;
    float v34 = (cosz + sinz) * height + camSpaceY;
    float v35 = (cosz - sinz) * height + camSpaceY;
    float v36 = (-cosz - sinz)* height + camSpaceY;
    if (v29 >= 0.0 || v30 >= 0.0 || v31 >= 0.0 || v32 >= 0.0 && v33 >= 0.0 || v34 >= 0.0 || v35 >= 0.0 || v36 >= 0.0 &&
        v29 <= RsGlobal.maximumWidth || v30 <= RsGlobal.maximumWidth || v31 <= RsGlobal.maximumWidth || v32 <= RsGlobal.maximumWidth &&
        v33 <= RsGlobal.maximumHeight || v34 <= RsGlobal.maximumHeight || v35 <= RsGlobal.maximumHeight || v36 <= RsGlobal.maximumHeight)
    {
        gSpriteVertices[0].x = v29; (-cos(z) - sin(z))
        gSpriteVertices[0].y = v33; (sin(z) - cos(z))

        gSpriteVertices[1].x = v30; (sin(z) - cos(z))
        gSpriteVertices[1].y = v34; (cos(z) + sin(z))

        gSpriteVertices[2].x = v31; (cos(z) + sin(z))
        gSpriteVertices[2].y = v35; (cos(z) - sin(z))

        gSpriteVertices[3].x = v32; (cos(z) - sin(z))
        gSpriteVertices[3].y = v36; (-cos(z) - sin(z))

        v23 = (RwIm2DGetFarScreenZ  - RwIm2DGetNearScreenZ()) * (camSpaceZ - CDraw::ms_fNearClipZ) * CDraw::ms_fFarClipZ / ((CDraw::ms_fFarClipZ - CDraw::ms_fNearClipZ) * camSpaceZ) + RwIm2DGetNearScreenZ();
        gSpriteVertices[0].rhw = recipFarClip;
        gSpriteVertices[1].rhw = recipFarClip;
        gSpriteVertices[2].rhw = recipFarClip;
        gSpriteVertices[3].rhw = recipFarClip;
        gSpriteVertices[0].z = v23;
        gSpriteVertices[1].z = v23;
        gSpriteVertices[2].z = v23;
        gSpriteVertices[3].z = v23;
        //  (0)     (1)
        // [0,0]   [0,1]
        //
        // [1,0]   [1,1]
        //  (3)     (2)
        gSpriteVertices[0].u = 0;
        gSpriteVertices[0].v = 0;
        gSpriteVertices[1].u = 0;
        gSpriteVertices[1].v = 1.0f;
        gSpriteVertices[2].u = 1.0f;
        gSpriteVertices[2].v = 1.0f;
        gSpriteVertices[3].u = 1.0f;
        gSpriteVertices[3].v = 0;
        // setup colors
        HIWORD(v14) = 0;
        BYTE1(v14) = alpha2;
        LOBYTE(v14) = (unsigned __int16)(v15 * (unsigned __int8)v14) / 256;
        v24 = ((unsigned __int8)((unsigned __int16)(v15 * (unsigned __int8)v13) >> 8) | (v14 << 8)) << 8;
        v22 = (unsigned __int8)((unsigned __int16)(v15 * (unsigned __int8)v12) >> 8);
        gSpriteVertices[0].emissiveColor = v22 | v24;
        gSpriteVertices[1].emissiveColor = v22 | v24;
        gSpriteVertices[2].emissiveColor = v22 | v24;
        gSpriteVertices[3].emissiveColor = v22 | v24;
        RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, gSpriteVertices, ELEMS_COUNT(gSpriteVertices));
    }
}