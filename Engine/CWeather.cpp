#include "StdInc.h"

void CWeather::Init()
{
    NewWeatherType = 0;
    OldWeatherType = 0;
    WeatherRegion = 0;
    InterpolationValue = 0;
    WeatherTypeInList = 0;
    ForcedWeatherType = -1;
    WhenToPlayLightningSound = 0;
    bScriptsForceRain = 0;
    Rain = 0;
    Sandstorm = 0;
    CurrentRainParticleStrength = 0;
    InTunnelness = 0;
    LightningStartX = 0;
    LightningStartY = 0;
    StreamAfterRainTimer = 0;
}

void CWeather::ForceWeather(int16_t weather)
{
    ForcedWeatherType = weather;
}

void CWeather::ForceWeatherNow(int16_t weather)
{
    ForcedWeatherType = weather;
    OldWeatherType = weather;
    NewWeatherType = weather;
}

void CWeather::ReleaseWeather()
{
    ForcedWeatherType = -1;
}

uint8_t* CWeather::FindWeatherTypesList()
{
    switch (WeatherRegion)
    {
        case 1:
            return WeatherTypesListLA;
        break;
        case 2:
            return WeatherTypesListSF;
        break;
        case 3:
            return WeatherTypesListVegas;
        break;
        case 4:
            return WeatherTypesListDesert;
        break;
        default:
            return WeatherTypesListDefault;
        break;
    }
}

bool CWeather::ForecastWeather(int weather, int numWeathers)
{
    for (size_t i = 0; i < numWeathers; i++)
    {
        uint8_t* weatherList = FindWeatherTypesList();
        if (weatherList[(WeatherTypeInList + i) % 64] == weather)
        {
            return true;
        }
    }
    return false;
}

void CWeather::UpdateWeatherRegion(CVector* pos)
{
    CVector refPos = pos ? *pos : TheCamera.GetPos();
    if (refPos.x > 1000.0 && refPos.y > 910.0)
    {
        CWeather::WeatherRegion = 3;
    }
    else if (refPos.x > -850.0 && refPos.x < 1000.0 && refPos.y > 1280.0)
    {
        CWeather::WeatherRegion = 4;
    }
    else if (refPos.x < -1430.0 && refPos.y > -580.0 && refPos.y < 1430.0)
    {
        CWeather::WeatherRegion = 2;
    }
    if (refPos.x > 250.0 && refPos.x < 3000.0 && refPos.y > -3000.0 && refPos.y < -850.0)
    {
        CWeather::WeatherRegion = 1;
    }
    else
    {
        CWeather::WeatherRegion = 0;
    }
}

void CWeather::SetWeatherToAppropriateTypeNow()
{
    CVector playerCoors;
    FindPlayerCoors(&playerCoors, -1);
    CWeather::UpdateWeatherRegion(&playerCoors);
    uint8_t* weatherList = FindWeatherTypesList();
    ReleaseWeather();
    OldWeatherType = weatherList[0];
    NewWeatherType = weatherList[0];
}

void CWeather::AddSandStormParticles()
{
    FxPrtMult_c prtInfo(0.67000002, 0.64999998, 0.55000001, 0.25, 1.0, 0.0, 0.2);
    CVector pos = TheCamera.GetPos();
    CVector dir = WindDir * 25.0f;
    pos.x += TheCamera.m_cameraMatrix.up.x * 10.0f;
    pos.y += TheCamera.m_cameraMatrix.up.y * 10.0f;
    pos.x = rand() * 1/32767 * 40.0f + pos.x - 20.0;
    pos.y = rand() * 1/32767 * 40.0f + pos.y - 20.0;
    pos.z = rand() * 1/32767 * 7.0f + pos.z - 2.0;
    g_fx.GetStaticSystem(FX_STATIC_SAND2)->AddParticle(pos, dir, 0, prtInfo, -1.0, 1.2, 0.6, 0);
}

float CWeather::GetWetRoads()
{
    return WetRoads;
}

uint8_t CWeather::GetWeatherRegion()
{
	return WeatherRegion;
}