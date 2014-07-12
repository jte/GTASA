#include "StdInc.h"

const float DotProduct(const CVector& first, const CVector& second)
{
    return first.x * second.x + first.y * second.y + first.z * second.z;
}

const CVector CrossProduct(const CVector& first, const CVector& second)
{
    return CVector(second.z * first.y - first.z * second.y, first.z * second.x - second.z * first.x, first.x * second.y - second.x * first.y);
}

void CVector::Normalise()
{
    float fLength = MagnitudeSquared();
    if(fLength > 0.0)
    {
        x /= fLength;
        y /= fLength;
        z /= fLength;
    }
    else
    {
        x = 1.0;
    }
}

float CVector::MagnitudeSquared() const
{
    return x * x + y * y + z * z;
}

float CVector::Magnitude2DSquared() const
{
    return x * x + y * y;
}

float CVector::Magnitude() const
{
    return sqrt(MagnitudeSquared());
}

float CVector::Magnitude2D() const
{
    return sqrt(Magnitude2DSquared());
}

float CVector::NormaliseAndMag()
{
    Normalise();
    float fLength = Magnitude();
    if(fLength > 0.0)
    {
        return fLength;
    }
    else
    {
        return 1.0;
    }
}