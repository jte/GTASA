#include "StdInc.h"

bool CColSphere::IntersectRay(const CVector& R1, const CVector& R2, CVector& I1, CVector& I2) const
{
    CVector dx = R1 - m_center;
    float x1;
    float x2;
    if(!CGeneral::SolveQuadratic(1.0, 2 * DotProduct(dx, R2), dx.MagnitudeSquared() - m_radius * m_radius, x1, x2))
    {
        return false;
    }
    I1 = R1 + R2 * x1;
    I2 = R1 + R2 * x2;
    return true;
}