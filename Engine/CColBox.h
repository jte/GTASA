#pragma once

class CColBox
{
public:
	void Set(const CVector& max, const CVector& min, uint8_t, uint8_t , uint8_t);
	CColBox& operator=(CColBox const&);
protected:
private:
    CVector max;
    CVector min;
//    ColSurface surface;
};