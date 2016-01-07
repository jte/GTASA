#include "StdInc.h"

void CSphere::Set(float radius, const CVector& center)
{
    this->radius = radius;
    this->center = center;
}

CVector CSphere::GetCenter() const
{
    return center;
}

float CSphere::GetRadius() const
{
    return radius;
}