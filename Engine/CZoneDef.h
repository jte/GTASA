#pragma once

class CZoneDef
{
public:
    * FindBoundingBox(CVector*, CVector*)
	* FindCenter()
	* IsPointWithin(CVector)
private:
    uint16_t m_x;
    uint16_t n_y;
    uint16_t pad1;
    uint16_t m_yWidth;
    uint16_t m_xWidth;
    uint16_t pad2;
    uint16_t zBottom;
    uint16_t zTop;
};