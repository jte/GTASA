#pragma once

#include "CWeaponInfo.h"

class CWeaponModelInfo : public CClumpModelInfo
{
public:
    // virtual methods
    virtual ~CWeaponModelInfo();
    virtual size_t GetModelType();
	virtual void Init();
	virtual void SetClump(RpClump* clump);
    //
    void SetWeaponType(eWeaponType weaponType);
    eWeaponType GetWeaponType() const;
private:
    eWeaponType m_weaponType;
};