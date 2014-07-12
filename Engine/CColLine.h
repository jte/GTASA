#pragma once

class CColLine
{
public:
	CColLine(const CVector& start, const CVector& end);
	void Set(const CVector& start, const CVector& end);

    const CVector& GetStart() const;
    const CVector& GetEnd() const;
protected:
private:
    CVector m_start;
    uint32_t pad1;
    CVector m_end;
    uint32_t pad2;
};