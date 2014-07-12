#pragma once

class CVector
{
public:
    CVector(float ix, float iy, float iz)
    {
        x = ix;
        y = iy;
        z = iz;
    }
    CVector()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }
	void FromMultiply(CMatrix const&, CVector const&);
	void FromMultiply3X3(CMatrix const&, CVector const&);
	float Magnitude() const;
	float Magnitude2D() const;
    float MagnitudeSquared() const;
    float Magnitude2DSquared() const;
	void Normalise();
	float NormaliseAndMag();

    // operators
    CVector operator+(const CVector& rhs)
    {
        CVector t;
        t.x = x + rhs.x;
        t.y = y + rhs.y;
        t.z = z + rhs.z;
        return t;
    }
    CVector operator-(const CVector& rhs)
    {
        CVector t;
        t.x = x - rhs.x;
        t.y = y - rhs.y;
        t.z = z - rhs.z;
        return t;
    }
    CVector operator*(const float& rhs)
    {
        CVector t;
        t.x = x * rhs;
        t.y = y * rhs;
        t.z = z * rhs;
        return t;
    }

    float x,y,z;
};

const float DotProduct(const CVector& first, const CVector& second);
const CVector CrossProduct(const CVector& first, const CVector& second);