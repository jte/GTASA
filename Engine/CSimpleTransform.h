#pragma once

class CSimpleTransform
{
public:
	void Invert(const CSimpleTransform&);
	void UpdateMatrix(CMatrix*) const;
	void UpdateRwMatrix(RwMatrixTag* matrix) const;
protected:
private:
    CVector m_pos;
    float m_angle;
};