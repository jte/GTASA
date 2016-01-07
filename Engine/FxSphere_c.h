#pragma once

#include "FxPlane_c.h"

class FxSphere_c : public RwSphere
{
public:
    FxSphere_c();
	float GetDistToPlane(FxPlane_c* plane);
	//bool IsCollision(FxSphere_c* other);
	//* operator delete(void*)
	//* operator new(unsigned int, int)
public:
    uint8_t startPlaneId;
};