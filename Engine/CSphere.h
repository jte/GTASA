#pragma once

class CSphere : public RwSphere
{
public:
	CSphere(RwReal x, RwReal y, RwReal z, RwReal radius)
	{
		center.x = x;
		center.y = y;
		center.z = z;
		this->radius = radius;
	}
	CSphere(CVector center, RwReal radius)
	{
		this->center = center;
		this->radius = radius;
	}
    void Set(float radius, const CVector& center);
    CVector GetCenter() const;
    float GetRadius() const;
};