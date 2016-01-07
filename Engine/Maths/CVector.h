#pragma once

class CVector : public RwV3d
{
public:
    CVector(const RwV3d& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
    }
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
    CVector operator+(const float& rhs)
    {
        CVector t;
        t.x = x + rhs;
        t.y = y + rhs;
        t.z = z + rhs;
        return t;
    }
    CVector operator-(const float& rhs)
    {
        CVector t;
        t.x = x - rhs;
        t.y = y - rhs;
        t.z = z - rhs;
        return t;
    }
    CVector operator-(const RwV3d& vec)
    {
        CVector t;
        t.x = x - vec.x;
        t.y = y - vec.y;
        t.z = z - vec.z;
        return t;
    }
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
        t *= rhs;
        return t;
    }
    CVector& operator*=(const float& rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }
    CVector operator-()
    {
        CVector t;
        t.x = -x;
        t.y = -y;
        t.z = -z;
        return t;
    }
    CVector operator/(const float& rhs)
    {
        CVector t;
        t.x = x / rhs;
        t.y = y / rhs;
        t.z = z / rhs;
        return t;
    }
    CVector& operator+=(const CVector& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }
    bool operator==(const CVector& rhs)
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }
    /*
    CVector& operator=(const RwV3d& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return *this;
    }
    */
};

const float DotProduct(const CVector& first, const CVector& second);
CVector CrossProduct(const CVector& first, const CVector& second);

class CVector2D : public RwV2d
{
public:
	CVector2D(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
 //   float x,y;
};