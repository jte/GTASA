#pragma once

#include "CRegisteredCorona.h"

class CCoronas
{
public:
    static void DoSunAndMoon();
	/* Init()
	* RegisterCorona(unsigned int, CEntity*, unsigned char, unsigned char, unsigned char, unsigned char, CVector const&, float, float, RwTexture*, unsigned char, unsigned char, unsigned char, unsigned char, float, bool, float, bool, float, bool, bool)
	* RegisterCorona(unsigned int, CEntity*, unsigned char, unsigned char, unsigned char, unsigned char, CVector const&, float, float, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, float, bool, float, bool, float, bool, bo
	* Render()
	* RenderOutGeometryBufferForReflections()
	* RenderReflections()
	* RenderSunReflection()
	* Shutdown()
	* Update()
	* UpdateCoronaCoors(unsigned int, CVector const&, float, float)
    */
private:
    /* LightsMult
	* MoonSize
	*/ 
    static size_t NumCoronas;
	static float ScreenMult;
    static bool SunBlockedByClouds;
	static float SunScreenX;
	static float SunScreenY;
	static CRegisteredCorona aCoronas[64];
    static bool bChangeBrightnessImmediately;
};