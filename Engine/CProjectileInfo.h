#pragma once

class CProjectileInfo
{
public:
	* AddProjectile(CEntity*, eWeaponType weaponType, CVector, float, CVector*, CEntity*)
	* GetProjectileInfo(int)
	* Initialise()
	* IsProjectileInRange(float, float, float, float, float, float, bool)
	* RemoveAllProjectiles()
	* RemoveDetonatorProjectiles()
	* RemoveFXSystem(unsigned char)
	* RemoveIfThisIsAProjectile(CObject*)
	* RemoveNotAdd(CEntity*, eWeaponType, CVector)
	* RemoveProjectile(CProjectileInfo*, CProjectile*)
	* Shutdown()
	* Update()
protected:
private:
    static CProjectile* ms_apProjectile;
};