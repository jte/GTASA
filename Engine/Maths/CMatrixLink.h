#pragma once

class CMatrixLink
{
public:
	void Insert(CMatrixLink* link);
	void Remove();
    CMatrix& GetMatrix();
    CPlaceable* GetRef();
    CMatrixLink* GetPrev();
    CMatrixLink* GetNext();
    void SetMatrix(const CMatrix& matrix);
    void SetRef(CPlaceable* ref);
    void SetNext(CMatrixLink* next);
    void SetPrev(CMatrixLink* prev);
private:
    CMatrix m_matrix;
    CPlaceable* m_ref;
    CMatrixLink* m_prev;
    CMatrixLink* m_next;
};