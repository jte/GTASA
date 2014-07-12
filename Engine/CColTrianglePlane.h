#pragma once

class CColTrianglePlane
{
public:
    void Set(const CompressedVector* vector, CColTriangle& triangle);
protected:
private:
    CompressedVector m_n;
    FixedPoint m_d; // compressed
    uint8_t m_pad1;
    uint8_t m_pad2;
};