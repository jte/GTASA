#pragma once

class FxInfoManager_c
{
public:
	//* AddFxInfo(int)
	//* Load(int, int)
	//* ProcessEmissionInfo(float, float, float, unsigned char, EmissionInfo_t*)
	//* ProcessMovementInfo(float, float, float, float, unsigned char, MovementInfo_t*)
	//* ProcessRenderInfo(float, float, float, float, unsigned char, RenderInfo_t*)
	//* ~FxInfoManager_c()
private:
    /*
    00000000 FxInfoManager_c struc ; (sizeof=0x14)   ; XREF: FxPrimBP_cr
00000000 count           dd ?
00000004 fxInfoAray      dd ?                    ; offset
00000008 uiParticleEmittersCount db ?
00000009 uiParticleEmittersInfoCount db ?
0000000A field_A         db ?
0000000B field_B         db ?
0000000C fLodStart       dw ?
0000000E fLodEnd         dw ?
00000010 bHasFlatParticleEmitter db ?
00000011 bHasHeatHazeParticleEmitter db ?
00000012 field_12        db ?
00000013 field_13        db ?
00000014 FxInfoManager_c ends
*/
};