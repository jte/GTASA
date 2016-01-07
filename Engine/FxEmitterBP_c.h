#pragma once

#include "FxPrimBP_c.h"

class FxEmitterBP_c : public FxPrimBP_c
{
    friend class FxPrimBP_c;
public:
    // virtual methods
    //virtual ~FxPrimBP_c();//0
    virtual void Load(int file, int, FxName32_t* texNames);//4
    virtual void LoadTextures(FxName32_t *,int);//8
    virtual FxPrim_c* CreateInstance();
    //virtual void Update(float)=0;
    //virtual void Render(RwCamera*,uint32_t,float,uint8_t)=0;
    //virtual void FreePrtFromPrim(FxSystem_c*)=0;
    //
	//* FxEmitterBP_c()
	//* IsFxInfoPresent(int)
	//* RenderHeatHaze(RwCamera*, unsigned int, float)
	//* UpdateParticle(float, FxEmitterPrt_c*)
	void* operator new(size_t size);
private:
};