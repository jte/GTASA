#include "StdInc.h"

void CDraw::SetFOV(float fov)
{
    ms_fFOV = fov;
}

void CDraw::CalculateAspectRatio()
{
    if(FrontEndMenuManager.widescreenOn)
    {
        ms_fAspectRatio = WIDE_ASPECTRATIO;
    }
    else if(TheCamera.b1rstPersonRunCloseToAWall)
    {
        ms_fAspectRatio = 5/4;
    }
    else
    {
        ms_fAspectRatio = DEFAULT_ASPECTRATIO;
    }
}