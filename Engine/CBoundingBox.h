#pragma once

class CBoundingBox
{
public:
    CBoundingBox();

    //
    const CVector& GetMin() const;
    const CVector& GetMax() const;
    const CVector& GetOffset() const;
    float GetRadius() const;
private:
    CVector m_min;
    CVector m_max;
    CVector m_offset;
    float m_radius;
};