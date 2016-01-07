#pragma once

#include "CCompressedMatrixNotAligned.h"

struct FxName32_t
{
    char texName[4][32];
};

class FxSystem_c;

class FxPrimBP_c
{
public:
    // virtual methods
    virtual ~FxPrimBP_c();//0
    virtual void Load(int, int, FxName32_t*);//4
    virtual void LoadTextures(FxName32_t *,int)=0;//8
    virtual FxPrim_c* CreateInstance()=0;
    virtual void Update(float)=0;
    virtual void Render(RwCamera*,uint32_t,float,uint8_t)=0;
    virtual void FreePrtFromPrim(FxSystem_c*)=0;
    //
	FxPrimBP_c();
	void GetRWMatrix(RwMatrixTag* rwMatrix);
private:
    //vtbl
    uint8_t pad1;
    uint8_t m_srcBlendId;
    uint8_t m_destBlendId;
    uint8_t m_alphaOn;
    CCompressedMatrixNotAligned<32767>* m_matrix;//_C=32767
    RwTexture* m_textures[4];
    uint32_t pad2;
    List_c emitterPrts;
    FxInfoManager_c m_fxInfoManager;
};