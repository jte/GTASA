#pragma once

#include "CAtomicModelInfo.h"

class CDamageAtomicModelInfo : public CAtomicModelInfo
{
public:
    // virtual methods
    virtual ~CDamageAtomicModelInfo();
	virtual CDamageAtomicModelInfo *AsAtomicModelInfoPtr();
    virtual void DeleteRwObject();
    virtual void Init();
    virtual void* CreateInstance(RwMatrixTag* matrix);
    virtual void* CreateInstance();
    //
	void SetDamagedAtomic(RpAtomic* atomic);
private:
    RpAtomic* m_damagedAtomic;
    static bool m_bCreateDamagedVersion;
};