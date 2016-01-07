#pragma once

class CBoundingBox
{
public:
    CBoundingBox();

    //
    const CVector& GetMin() const;
    const CVector& GetMax() const;
	const CSphere& GetBoundingSphere() const;

	void SetBoundingSphere(const CSphere& sphere);
	void SetBoundingBox(const CBox& box);
private:
    CVector m_min;
    CVector m_max;
	CBox boundingBox;
	CSphere boundingSphere;
};