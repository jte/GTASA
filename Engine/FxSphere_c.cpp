#include "StdInc.h"

FxSphere_c::FxSphere_c()
{
    startPlaneId = 0;
}

float FxSphere_c::GetDistToPlane(FxPlane_c* plane)
{
    return DotProduct(plane->normal, center) - plane->distance;
}