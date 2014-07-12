#include "CDraw.h"

void CDraw::CalculateAspectRatio()
{
    if(FrontEndMenuManager.widescreenOn)
    {
        ms_fAspectRatio = 16.0f / 9.0f;
    }
    else if(TheCamera.b1rstPersonRunCloseToAWall)
    {
        ms_fAspectRatio = 5.0f / 4.0f;
    }
    else
    {
        ms_fAspectRatio = 4.0f / 3.0f;
    }
}

void CDraw::SetFOV(float fFOV)
{
    ms_fFOV = fFOV;
}