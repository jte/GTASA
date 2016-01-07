#pragma once

class CSimpleTransform
{
public:
	void Invert(const CSimpleTransform&);
	void UpdateMatrix(CMatrix*) const;
	void UpdateRwMatrix(RwMatrixTag* matrix) const;
    const CVector GetPos() const;
    void SetPos(const CVector& pos);
    float GetAngle() const;
    void SetAngle(float angle);
protected:
private:
    CVector m_pos;
    float m_angle;
};