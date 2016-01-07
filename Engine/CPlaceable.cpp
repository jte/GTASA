#include "StdInc.h"

CPlaceable::CPlaceable()
{
    m_placement.pos = CVector(0.0f,0.0f,0.0f);
    m_placement.angle = 0.0f;
    m_xyz = NULL;
}

CMatrix *CPlaceable::GetMatrix()
{
    return &m_xyz->matrix;
}

CVector CPlaceable::GetPos() const
{
    if (m_xyz)
    {
        return CVector(m_xyz->GetMatrix().pos);
    }
    else
    {
        return m_placement.GetPos();
    }
}

void CPlaceable::SetPos(const CVector& pos)
{
    if (m_xyz)
    {
        m_xyz->GetMatrix().pos = pos;
    }
    else
    {
        m_placement.SetPos(pos);
    }
}

bool CPlaceable::IsWithinArea(float x_min, float y_min, float x_max, float y_max) const
{
    CVector pos = GetPos();
    if(x_min > x_max)
    {
        Exchange(x_max, x_min);
    }
    if(y_min > y_max)
    {
        Exchange(y_max, y_min);
    }
    return pos.x >= x_min && pos.x <= x_max && pos.y >= y_min && pos.y <= y_max;
}

bool CPlaceable::IsWithinArea(float x_min, float y_min, float z_min, float x_max, float y_max, float z_max) const
{
    CVector pos = GetPos();
    if(x_min > x_max)
    {
        Exchange(x_max, x_min);
    }
    if(y_min > y_max)
    {
        Exchange(y_max, y_min);
    }
    if(z_min > z_max)
    {
        Exchange(z_max, z_min);
    }
    return pos.x >= x_min && pos.x <= x_max && pos.y >= y_min && pos.y <= y_max && pos.z >= z_min && pos.z <= z_max;
}

void CPlaceable::AllocateStaticMatrix()
{
    if(m_xyz)
    {
        g_MatrixLinkList.MoveToList2(m_xyz);
    }
    else
    {
        CMatrixLink *link = g_MatrixLinkList.AddToList2();
        if(!link)
        {
            g_MatrixLinkList.m_list1_tail.prev->ref->ShutdownMatrixArray();
            link = g_MatrixLinkList.AddToList2();
        }
        link->ref = this;
        m_xyz = link;
    }
}

void CPlaceable::AllocateMatrix()
{
    if(!m_xyz)
    {
        CMatrixLink *link = g_MatrixLinkList.AddToList1();
        if(!link)
        {
            g_MatrixLinkList.m_list1_tail.prev->ref->ShutdownMatrixArray();
            link = g_MatrixLinkList.AddToList1();
        }
        link->ref = this;
        m_xyz = link;
    }
}

void CPlaceable::SetMatrix(CMatrix const &mat)
{
    if(!m_xyz)
    {
        if(mat.m_matrix.at.z == 1.0f)
        {
            m_placement.pos = CVector(mat.m_matrix.pos.x, mat.m_matrix.pos.y, mat.m_matrix.pos.z);
            m_placement.angle = atan2(mat.m_matrix.up.y, -mat.m_matrix.up.x);
        }
        AllocateMatrix();
    }
    m_xyz->matrix = mat;
}

CVector CPlaceable::GetAt()
{
    if(m_xyz)
    {
        return m_xyz->matrix.matrix.at;
    }
    else
    {
        return CVector(0.0f, 0.0f, 1.0f);
    }
}

CVector CPlaceable::GetRight()
{
    if(m_xyz)
    {
        return m_xyz->matrix.matrix.right;
    }
    else
    {
        return CVector(cos(m_transform.angle), sin(m_transform.angle), 0.0f);
    }
}

CVector CPlaceable::GetTop()
{
    if(m_xyz)
    {
        return m_xyz->matrix.matrix.top;
    }
    else
    {
        return CVector(-sin(m_transform.angle), cos(m_transform.angle), 0.0f);
    }
}

void CPlaceable::UpdateRwMatrix()
{
    if(m_xyz)
    {
        m_xyz->UpdateMatrix(GetClumpFrame(m_rwObject)->modelling);
    }
    else
    {
        m_transform.UpdateRwMatrix(GetClumpFrame(m_rwObject)->modelling);
    }
}