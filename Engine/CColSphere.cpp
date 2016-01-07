#include "StdInc.h"

bool CColSphere::IntersectRay(const CVector& R1, const CVector& R2, CVector& I1, CVector& I2) const
{
    CVector rayStart = R1;
    CVector rayEnd = R2;
    // (o - c)
    CVector dx = rayStart - GetCenter();
    float x1;
    float x2;
    // 2 (o - c)
    float b = 2 * DotProduct(dx, rayEnd);
    float c = dx.MagnitudeSquared() - GetRadius() * GetRadius();
    if(!CGeneral::SolveQuadratic(1.0, b, c, x1, x2))
    {
        return false;
    }
    I1 = rayStart + rayEnd * x1;
    I2 = rayStart + rayEnd * x2;
    return true;
}

EColSurface CColSphere::GetSurfaceTypeA() const
{
    return m_surfaceTypeA;
}

EColSurface CColSphere::GetSurfaceTypeB() const
{
    return m_surfaceTypeB;
}

CColLighting CColSphere::GetLighting() const
{
    return m_lighting;
}