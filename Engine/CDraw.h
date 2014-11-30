#pragma once

#include "CRGBA.h"

class CDraw
{
public:
	static void CalculateAspectRatio();
	static void SetFOV(float fov);

    //
    static float GetFOV();
    static float GetAspectRatio();
    static float GetFarClip();
    static float GetLODDistance();
    static float GetNearClip();
protected:
private:
    static bool Drawn;
	//* FadeBlue
	//* FadeGreen
	//* FadeRed
	static CRGBA FadeValue;
	static bool NotDrawn;
	static float ms_fAspectRatio;
	static float ms_fFOV;
	static float ms_fFarClipZ;
	static float ms_fLODDistance;
	static float ms_fNearClipZ;
};