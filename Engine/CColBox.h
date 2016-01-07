#pragma once

class CColBox
{
public:
	void Set(const CVector& max, const CVector& min, uint8_t, uint8_t , uint8_t);
	CColBox& operator=(CColBox const&);
    //
    void SetMax(const CVector& max);
    void SetMin(const CVector& min);
protected:
private:
    CVector max;
    CVector min;
    EColSurface surface;
};