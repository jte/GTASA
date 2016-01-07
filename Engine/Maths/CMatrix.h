#pragma once

class CVector;
class CQuaternion;

// NOTE: uses right-hand system 
class CMatrix : public RwMatrixTag
{
public:
	void Attach(RwMatrixTag *pOtherMat, bool bLink);
	void AttachRW(RwMatrixTag*, bool);
	CMatrix(CMatrix const &mat);
	CMatrix(RwMatrixTag *pOtherMat, bool bLink);
	CMatrix(RwMatrix *mat);
	void CheckIntegrity();
	void ConvertFromEulerAngles(float ex, float ey, float ez, int precision);
	void ConvertToEulerAngles(float &ex, float &ey, float &ez, int precision);
	void CopyOnlyMatrix(CMatrix const&);    
	void CopyToRwMatrix(RwMatrixTag *m) const;                    
	void Detach();
	void ForceUpVector(CVector vecUp);                               
	void Reorthogonalise();
	void ResetOrientation();
	void Rotate(float, float, float);
	void RotateX(float);
	void RotateY(float);
	void RotateZ(float);
	void SetRotate(CQuaternion const&);
	void SetRotate(float, float, float);
	void SetRotateX(float);
	void SetRotateXOnly(float);
	void SetRotateY(float);
	void SetRotateYOnly(float);
	void SetRotateZ(float);
	void SetRotateZOnly(float fZ);
	void SetScale(float scale);                        
	void SetScale(float sx, float sy, float sz);
	void SetTranslate(float x, float y, float z);
	void SetTranslateOnly(float x, float y, float z);
	void SetUnity();                                            
	void Update();
	void UpdateMatrix(RwMatrixTag *pMat);                        
	void UpdateRW() const;
	void UpdateRwMatrix(RwMatrixTag *pRwMatrix) const;

	CMatrix *operator+=(CMatrix const&);
	CMatrix *operator=(CMatrix const &mat);
	~CMatrix();
//private:
    RwMatrixTag *m_pRwMatrix;
    bool m_bLinkedMatrix;
};