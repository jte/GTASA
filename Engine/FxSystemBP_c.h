#pragma once

#include "ListItem_c.h"

class FxSphere_c;
class FxEmitterBP_c;

class FxSystemBP_c
{
public:
    FxSystemBP_c();
    	/* FreePrtFromSystem(FxSystem_c*)
	* Render(RwCamera*, float, unsigned char)
	* Update(float)
	*/ 
    void Load(char*, int file, int a3);
    void SetBoundingSphere(RwV3d* center, float radius);
    FxSphere_c* GetBoundingSphere();
    void Unload();
    void* operator new(unsigned int size);
    ListItem_c* GetInList();
	uint32_t GetNumFxEmitterBP();
private:
    ListItem_c m_inList;
    uint32_t m_fxSystemNameHash;
    float m_length;
    float m_intervalMin;
    float m_length2;
    FixedPoint m_cullDist;//_C=256
    uint8_t m_playMode;
    uint32_t m_numFxEmitterBP;
    FxEmitterBP_c** m_fxEmitterBPArray;
    FxSphere_c* m_boundingSphere;
};