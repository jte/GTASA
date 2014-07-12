#include "StdInc.h"

void CMatrix::Attach(RwMatrixTag *pOtherMat, bool bLink)
{
    if(m_pRwMatrix && m_bLinkedMatrix)
    {
        RwMatrixDestroy(m_pRwMatrix);
    }
    m_pRwMatrix = pOtherMat;
    m_bLinkedMatrix = bLink;
    m_matrix = *pOtherMat;
}

CMatrix::CMatrix(CMatrix const &mat)
{
    m_pRwMatrix = NULL;
    m_bLinkedMatrix = false;
    m_matrix = mat.m_matrix;
}

CMatrix::CMatrix(RwMatrixTag *pOtherMat, bool bLink)
{
    m_pRwMatrix = NULL;
    Attach(pOtherMat, bLink);
}

void CMatrix::ConvertToEulerAngles(float &ex, float &ey, float &ez, int precision)
{/*
  v9 = this->matrix.right.y;
  v33 = this->matrix.right.x;
  v35 = this->matrix.right.z;
  v37 = this->matrix.top.y;
  v10 = this->matrix.at.x;
  v34 = v9;
  v11 = this->matrix.top.x;
  v39 = v10;
  v12 = this->matrix.at.z;
  v36 = v11;
  v13 = this->matrix.top.z;
  v41 = v12;
  v38 = v13;
  v14 = this->matrix.at.y;
  v8 = (unsigned __int8)byte_866D9C[(precision >> 3) & 3];
  v32 = precision & 1;
  v7 = (precision >> 2) & 1;
  v6 = (unsigned __int8)byte_866D95[v8 - v7];
  v40 = v14;
  v5 = (unsigned __int8)*(&byte_866D94[v8] + v7);
  if ( ((precision >> 1) & 1) == 1 )
  {
    v18 = 3 * v8 + v6;
    v19 = *(&v33 + v18);
    v20 = 3 * v8 + v5;
    v21 = *(&v33 + v20);
    v16 = &v33 + v18;
    v15 = &v33 + v20;
    v17 = sqrt(v21 * v21 + v19 * v19);
    if ( v17 > 0.0000019073486328125 )
    {
      result = ex;
      *ex = atan2(*v16, *v15);
      v24 = ey;
      *ey = atan2(*(&v33 + 4 * v8), v17);
      v25 = -*(&v33 + v8 + 2 * v6 + v6);
      v23 = ez;
      *ez = atan2(v25, *(&v33 + v8 + 2 * v5 + v5));
      goto LABEL_9;
    }
    result = ex;
    *ex = atan2(*(&v33 + 4 * v5), -*(&v33 + v6 + 2 * v5 + v5));
    v26 = *(&v33 + 4 * v8);
  }
  else
  {
    v29 = 3 * v5 + v8;
    v30 = *(&v33 + v29);
    v27 = (long double *)(&v33 + v29);
    v42 = (long double *)(&v33 + 4 * v8);
    v28 = sqrt(v30 * v30 + *(&v33 + 4 * v8) * *(&v33 + 4 * v8));
    result = ex;
    if ( v28 > 0.0000019073486328125 )
    {
      *ex = atan2(*(&v33 + 4 * v6), *(&v33 + 3 * v6 + v5));
      v24 = ey;
      *ey = atan2(v28, -*(&v33 + v8 + 3 * v6));
      v23 = ez;
      *ez = atan2(*(float *)v42, *(float *)v27);
      goto LABEL_9;
    }
    *ex = atan2(*(&v33 + 4 * v5), -*(&v33 + v6 + 3 * v5));
    v26 = v28;
    v17 = -*(&v33 + v8 + 2 * v6 + v6);
  }
  v24 = ey;
  v23 = ez;
  *ey = atan2(v26, v17);
  *(_DWORD *)ez = 0;
LABEL_9:
  if ( v7 == 1 )
  {
    *result = -*result;
    *v24 = -*v24;
    *v23 = -*v23;
  }
  if ( v32 == 1 )
  {
    v31 = *result;
    *(_DWORD *)result = *(_DWORD *)v23;
    *v23 = v31;
  }
  return result;*/
}

void CMatrix::CopyToRwMatrix(RwMatrixTag *m) const
{
    m->right = m_matrix.right;
    m->up = m_matrix.up;
    m->at = m_matrix.at;
    m->pos = m_matrix.pos;
    RwMatrixUpdate(m);
}

void CMatrix::SetScale(float scale)
{
    //      [s 0 0]
    // m =  [0 s 0]
    //      [0 0 s]
    m_matrix.right.x = scale;
    m_matrix.up.y = scale;
    m_matrix.at.z = scale;
}

void CMatrix::SetScale(float sx, float sy, float sz)
{
    //      [sx 0 0]
    // m =  [0 sy 0]
    //      [0 0 sz]
    m_matrix.right.x = sx;
    m_matrix.up.y = sy;
    m_matrix.at.z = sz;
}

void CMatrix::UpdateRW() const
{
    if(m_pRwMatrix)
    {
        UpdateRwMatrix(m_pRwMatrix);
    }
}

void CMatrix::UpdateMatrix(RwMatrixTag *pMat)
{
    m_matrix = *pMat;
}

void CMatrix::Detach()
{
    if(m_bLinkedMatrix && m_pRwMatrix)
    {
        RwMatrixDestroy(m_pRwMatrix);
    }
    m_pRwMatrix = NULL;
}

void CMatrix::UpdateRwMatrix(RwMatrixTag *pRwMatrix) const
{
    *pRwMatrix = m_matrix;
    RwMatrixUpdate(pRwMatrix);
}

void CMatrix::SetRotateZOnly(float fZ)
{
    m_matrix.right.z = 0;
    m_matrix.up.z = 0;
    m_matrix.at.x = 0.0;
    m_matrix.at.y = 0.0;
    m_matrix.at.z = 1.0;
    float fCos = cos(fZ);
    float fSin = sin(fZ);
    m_matrix.right.x = fCos;
    m_matrix.right.y = fSin;
    m_matrix.up.x = -fSin;
    m_matrix.up.y = fCos;
}

void CMatrix::ForceUpVector(CVector vecUp)
{
    m_matrix.right.z = vecUp.y * m_matrix.up.x - vecUp.x * m_matrix.up.y;
}

CMatrix *CMatrix::operator+=(CMatrix const &mat)
{
    m_matrix = mat.m_matrix;
    if(m_pRwMatrix)
    {
        UpdateRwMatrix(m_pRwMatrix);
    }
    return this;
}

void CMatrix::SetTranslateOnly(float x, float y, float z)
{
    m_matrix.pos.x = x;
    m_matrix.pos.y = y;
    m_matrix.pos.z = z;
}

void CMatrix::SetTranslate(float x, float y, float z)
{
    m_matrix.right.x = 1.0f;
    m_matrix.right.y = 0.0f;
    m_matrix.right.z = 0.0f;
    m_matrix.at.x = 0.0f;
    m_matrix.at.y = 0.0f;
    m_matrix.at.z = 1.0f;
    m_matrix.up.x = 0.0f;
    m_matrix.up.y = 1.0f;
    m_matrix.up.z = 0.0f;
    SetTranslateOnly(x, y, z);
}

CMatrix::~CMatrix()
{
    Detach();
}