#pragma once

class CLocalisation
{
public:
	static bool Blood();
    static bool FrenchGame();
	static bool GermanGame();
    static void Initialise();
	static bool KickingWhenDown();
	static bool KillFrenzy();
	static bool KillPeds();
    static bool KnockDownPeds();
    static bool Metric();
	static bool PedsOnFire();
    static bool Porn();
	static bool Prostitutes();
    static bool ScreamsFromKills();
    static void SetFrenchGame();
	static void SetGermanGame();
    static void SetNormalGame();
	static bool ShootLimbs();
	static bool StealFromDeadPed();
private:
    static bool frenchGame;
	static bool germanGame;
	static bool nastyGame;
};