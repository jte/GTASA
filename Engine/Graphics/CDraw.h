#pragma once
#include "StdInc.h"

class CDraw
{
public:
	static void CalculateAspectRatio();
	static void SetFOV(float fFOV);
private:
	static bool Drawn;
	static float FadeBlue;
	static float FadeGreen;
	static float FadeRed;
	static float FadeValue;
	static bool NotDrawn;
	static float ms_fAspectRatio;
	static float ms_fFOV;
	static float ms_fFarClipZ;
	static float ms_fLODDistance;
	static float ms_fNearClipZ;
};