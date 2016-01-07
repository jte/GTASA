#include "StdInc.h"

void CLocalisation::Initialise()
{
    SetNormalGame();
}

void CLocalisation::SetNormalGame()
{
    nastyGame = true;
    frenchGame = false;
    germanGame = false;
}

void CLocalisation::SetFrenchGame()
{
    frenchGame = true;
    germanGame = false;
    nastyGame = true;
}

void CLocalisation::SetGermanGame()
{
    germanGame = true;
    frenchGame = false;
    nastyGame = false;
}

bool CLocalisation::GermanGame()
{
    return germanGame;
}

bool CLocalisation::FrenchGame()
{
    return frenchGame;
}

bool CLocalisation::Metric()
{
    return FrontEndMenuManager.GetLanguage() != 0;
}

bool CLocalisation::Blood()
{
    return true;
}

bool CLocalisation::Porn()
{
    return true;
}

bool CLocalisation::ScreamsFromKills()
{
    return !GermanGame();
}

bool CLocalisation::Prostitutes()
{
    return true;
}

bool CLocalisation::KickingWhenDown()
{
    return true;
}

bool CLocalisation::ShootLimbs()
{
    return nastyGame;
}

bool CLocalisation::KnockDownPeds()
{
    return nastyGame;
}

bool CLocalisation::KillFrenzy()
{
    return nastyGame;
}

bool CLocalisation::StealFromDeadPed()
{
    return nastyGame;
}

bool CLocalisation::KillPeds()
{
    return nastyGame;
}

bool CLocalisation::PedsOnFire()
{
    return nastyGame;
}