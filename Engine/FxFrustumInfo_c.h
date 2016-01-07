#pragma once

#include "FxSphere_c.h"

class FxFrustumInfo_c
{
public:
    bool IsCollision(FxSphere_c* sphere);
private:
    FxSphere_c m_sphere;
    uint32_t pad1;
    FxPlane_c m_cameraFrustumPlanes[4];
};