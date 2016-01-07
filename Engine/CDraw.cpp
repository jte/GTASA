#include "StdInc.h"

void CDraw::SetFOV(float fov)
{
    ms_fFOV = fov;
}

void CDraw::CalculateAspectRatio()
{
    if (FrontEndMenuManager.widescreenOn)
    {
        ms_fAspectRatio = WIDE_ASPECTRATIO;
    }
    else if (TheCamera.b1rstPersonRunCloseToAWall)
    {
        ms_fAspectRatio = 5/4;
    }
    else
    {
        ms_fAspectRatio = DEFAULT_ASPECTRATIO;
    }
}

float CDraw::GetFOV()
{
    return ms_fFOV;
}

float CDraw::GetAspectRatio()
{
    return ms_fAspectRatio;
}

float CDraw::GetFarClip()
{
    return ms_fFarClipZ;
}

float CDraw::GetLODDistance()
{
    return ms_fLODDistance;
}

float CDraw::GetNearClip()
{
    return ms_fNearClipZ;
}