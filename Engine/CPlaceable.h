#pragma once


struct SPlacement
{
    SPlacement()
    {
        pos = CVector(0.0f, 0.0f, 0.0f);
        angle = 0.0f;
    }
    CVector pos;
    float angle;
};

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
protected:
private:
    SPlacement m_placement;
    class CMatrixLink *m_xyz;
};