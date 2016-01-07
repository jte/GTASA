#pragma once

#include "CBaseModelInfo.h"

class CAtomicModelInfo : public CBaseModelInfo
{
public:
    // virtual methods
    virtual ~CAtomicModelInfo();
	virtual CAtomicModelInfo *AsAtomicModelInfoPtr();
    virtual size_t GetModelType();
    virtual void DeleteRwObject(); 
	virtual void Init();
    virtual size_t GetRwModelType();
    virtual void* CreateInstance(RwMatrixTag* matrix);
    virtual void* CreateInstance(); 
	virtual void SetAtomic(RpAtomic* atomic);

	//
    
	RpAtomic* GetAtomicFromDistance(float distance);
	// * SetupVehicleUpgradeFlags(char const*)
};