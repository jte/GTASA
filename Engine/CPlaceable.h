#pragma once

#include "Engine\CSimpleTransform.h"

class CPlaceable
{
public:
	void AllocateStaticMatrix();
	void AllocateMatrix();
	CPlaceable();
	void FreeStaticMatrix();
	void* GetForward() const;
	CMatrix *GetMatrix();
	void InitMatrixArray();
    // rectangle
	bool IsWithinArea(float x_min, float y_min, float x_max, float y_max) const;
    // cube
	bool IsWithinArea(float x_min, float y_min, float z_min, float x_max, float y_max, float z_max) const;
	void RemoveMatrix();
	void SetMatrix(CMatrix const &mat);
	void ShutdownMatrixArray();
    CVector GetPos() const;
	~CPlaceable();

    //
    CVector GetAt() const;
    CVector GetRight() const;
    CVector GetTop() const;
protected:
private:
    CSimpleTransform m_placement;
    class CMatrixLink *m_xyz;
};