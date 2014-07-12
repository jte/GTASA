#pragma once

const float DotProduct(const CQuaternion& first, const CQuaternion& second);

class CQuaternion
{
public:
    RwMatrixTag *Get(RwMatrixTag *pMat) const;
	//* Get(RwV3d*, float*) const
	//* Get(float*, float*, float*) const
	void Multiply(CQuaternion const&, CQuaternion const&);
	void Set(RwMatrixTag const&);
	void Set(RwV3d *vector, float angle);
	void Set(float, float, float);
	//* Slerp(CQuaternion const&, CQuaternion const&, float)
	float Slerp(CQuaternion const &q1, CQuaternion const &q2, float lx, float ly, float lz);

    //
    CQuaternion operator-();

    CVector pos;
    float w;
};