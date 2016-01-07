#include "StdInc.h"

bool FxFrustumInfo_c::IsCollision(FxSphere_c* sphere)
{
    if (!m_sphere.IsCollision(sphere))
    {
        return false;
    }
    size_t planeId = sphere->startPlaneId;
    for (size_t i = 0; i < 4; i++)
    {
        if (sphere->GetDistToPlane(&m_cameraFrustumPlanes[planeId % 4]) > sphere->radius)
        {
            sphere->startPlaneId = planeId % 4;
            return false;
        }
        planeId++;
    }
    return true;
}