#include "StdInc.h"

void CSimpleTransform::UpdateRwMatrix(RwMatrixTag* matrix) const
{
    matrix->right.z = 0.0f;
    matrix->up.z = 0.0f;
    matrix->at.z = 1.0f;
    matrix->at.x = 0.0f;
    matrix->at.y = 0.0f;
    matrix->right.x = cos(m_angle);
    matrix->right.y = sin(m_angle);
    matrix->up.x = -sin(m_angle);
    matrix->up.y = cos(m_angle);
    matrix->pos.x = m_pos.x;
    matrix->pos.y = m_pos.y;
    matrix->pos.z = m_pos.z;
}