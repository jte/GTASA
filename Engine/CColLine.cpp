#include "StdInc.h"

CColLine::CColLine(const CVector& start, const CVector& end)
{
    Set(start, end);
}

void CColLine::Set(const CVector& start, const CVector& end)
{
    m_start = start;
    m_end = end;
}

const CVector& CColLine::GetStart() const
{
    return m_start;
}

const CVector& CColLine::GetEnd() const
{
    return m_end;
}