#pragma once

class CMatrixLink
{
public:
	void Insert(CMatrixLink*);
	void Remove();
//private:
    CMatrix matrix;
    CPlaceable *ref;
    CMatrixLink *prev;
    CMatrixLink *next;
};