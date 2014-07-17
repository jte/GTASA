#pragma once

class CAtomicModelInfo : public CBaseModelInfo
{
public:
	virtual CAtomicModelInfo* AsAtomicModelInfoPtr();
	* CreateInstance()
	* CreateInstance(RwMatrixTag*)
	* DeleteRwObject()
	* GetAtomicFromDistance(float)
	* GetModelType()
	* GetRwModelType()
	* Init()
	virtual void SetAtomic(RpAtomic* atomic);
	* SetupVehicleUpgradeFlags(char const*)
	* ~CAtomicModelInfo()
protected:
private:
};