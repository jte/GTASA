#pragma once

class CColSphere
{
public:
    //* IntersectEdge(CVector const&, CVector const&, CVector&, CVector&) const
	//* IntersectPoint(CVector const&) const
	bool IntersectRay(const CVector& R1, const CVector& R2, CVector& I1, CVector& I2) const;
	//* IntersectSphere(CColSphere const&) const
	//* Set(float, CVector const&, unsigned char, unsigned char, unsigned char)

    // 
    const CVector& GetCenter() const;
    const float GetRadius() const;
protected:
private:
    CVector m_center;
    float m_radius;
    uint16_t m_surfaceType;
    uint8_t m_lighting;
};