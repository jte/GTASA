#pragma once

class CWeaponEffects
{
public:
	* CWeaponEffects()
	* ClearCrossHair(int)
	* ClearCrossHairImmediately(int)
	* ClearCrossHairs()
	* ClearCrossHairsImmediately()
	* DrawCrossHair(unsigned int, float)
	void Init();
	* IsLockedOn(int)
	* MarkTarget(int, CVector, unsigned char, unsigned char, unsigned char, unsigned char, float, unsigned char)
	* Render()
	* Shutdown()
	* ~CWeaponEffects()
protected:
private:
};