#include "CEntryInfoList.h"

void CEntryInfoList::Flush(CEntryInfoNode **ppList)
{
	CEntryInfoNode
		*pList = *ppList;
	while(pList)
	{
		CEntryInfoNode
			*pHelper;
		pHelper = pList;
		bool
			bTrue = *ppList == pList;
		CEntryInfoList
			**ppUnknown = &pList->field_10;
		pList = pList->field_10
		if(bExpression)
			
	}
}