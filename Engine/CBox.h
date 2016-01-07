#pragma once

class CBox : public RwBBox
{
public:
    void Recalc();
	void Set(const CVector& min, const CVector& max);
private:
    //CVector m_min; //=inf
    //CVector m_max; //=sup
};

