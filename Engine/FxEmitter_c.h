#pragma once

class FxEmitterBP_c;
class FxSystem_c;

class FxEmitter_c
{
public:
	// virtual methods
	/*
		* virtual ~FxEmitter_c()
		* virtual Init(FxPrimBP_c*, FxSystem_c*)
		* virtual Update(float, float)
		* virtual Reset()
		* virtual AddParticle(RwV3d*, RwV3d*, float, FxPrtMult_c*, float, float, unsigned char)
		* virtual AddParticle(RwMatrixTag*, RwV3d*, float, FxPrtMult_c*, float, float, unsigned char)
	*/
	// 
	   /*
        * CreateParticle(EmissionInfo_t*, RwMatrixTag*, RwV3d*, float, FxPrtMult_c*, float, unsigned char)
        * CreateParticles(float, float)
		*/
	bool IsActive() const;
private:
	FxEmitterBP_c* m_fxEmitterBP;
	FxSystem_c* m_fxSystem;
	bool m_active;
	uint8_t pad1;
	uint8_t pad2;
	uint8_t pad3;
	uint32_t pad4;
};