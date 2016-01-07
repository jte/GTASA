#pragma once

#include "Engine\CSimpleTransform.h"

class CMatrixLink;

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
    void SetPos(const CVector& pos);
	~CPlaceable();

    //
    CVector GetAt() const;
    CVector GetRight() const;
    CVector GetTop() const;
    void UpdateRwMatrix();
    CMatrixLink* GetMatrixLink()
    {
        return m_xyz;
    }
protected:
private:
    CSimpleTransform m_placement; // static matrix
    CMatrixLink* m_xyz; // matrix
};