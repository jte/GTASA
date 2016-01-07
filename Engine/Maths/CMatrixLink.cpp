#include "StdInc.h"

void CMatrixLink::Insert(CMatrixLink* link)
{
}

void CMatrixLink::Remove()
{
}

CMatrix& CMatrixLink::GetMatrix()
{
    return m_matrix;
}

CPlaceable* CMatrixLink::GetRef()
{
    return m_ref;
}

CMatrixLink* CMatrixLink::GetPrev()
{
    return m_prev;
}

CMatrixLink* CMatrixLink::GetNext()
{
    return m_next;
}

void CMatrixLink::SetMatrix(const CMatrix& matrix)
{
    m_matrix = matrix;
}

void CMatrixLink::SetRef(CPlaceable* ref)
{
    m_ref = ref;
}

void CMatrixLink::SetNext(CMatrixLink* next)
{
    m_next = next;
}

void CMatrixLink::SetPrev(CMatrixLink* prev)
{
    m_prev = prev;
}