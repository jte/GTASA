#pragma once

class CPtrList
{
public:
	size_t CountElements();
	bool IsMemberOfList(void *pList);
public:
    void *pData;
    CPtrList *pNext;
};