#pragma once

#include "CBaseModelInfo.h"

class CLodAtomicModelInfo : public CAtomicModelInfo
{
public:
    // virtual methods
    virtual ~CLodAtomicModelInfo();
	virtual CLodAtomicModelInfo *AsLodAtomicModelInfoPtr();
    virtual size_t GetModelType();
    virtual void Init();
};