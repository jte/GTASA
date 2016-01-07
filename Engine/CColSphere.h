#pragma once

#include "CSphere.h"

class CColSphere : public CSphere
{
public:
    //* IntersectEdge(CVector const&, CVector const&, CVector&, CVector&) const
	//* IntersectPoint(CVector const&) const
	bool IntersectRay(const CVector& R1, const CVector& R2, CVector& I1, CVector& I2) const;
	//* IntersectSphere(CColSphere const&) const
	//* Set(float, CVector const&, unsigned char, unsigned char, unsigned char)
    EColSurface GetSurfaceTypeA() const;
    EColSurface GetSurfaceTypeB() const;
    CColLighting GetLighting() const;
	void SetSurfaceTypeA(EColSurface colSurface);
	void SetSurfaceTypeB(EColSurface colSurface);
protected:
private:
    EColSurface m_surfaceTypeA;
    EColSurface m_surfaceTypeB;
    CColLighting m_lighting;
};