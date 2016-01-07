#include "StdInc.h"

size_t CLodAtomicModelInfo::GetModelType()
{
    return 8;
}

CLodAtomicModelInfo* CLodAtomicModelInfo::AsLodAtomicModelInfoPtr()
{
	return this;
}

void CLodAtomicModelInfo::Init()
{
	CAtomicModelInfo::Init();
}

CLodAtomicModelInfo::~CLodAtomicModelInfo()
{
}