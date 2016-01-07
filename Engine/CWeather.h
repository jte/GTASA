#pragma once

class CWeather
{
public:
    /* AddHeatHaze()
	* AddRain()
	*/ 
    static void AddSandStormParticles();
	/* AddStreamAfterRain()
	*/
    static uint8_t* FindWeatherTypesList(); 
    static void ForceWeather(int16_t weather);
	static void ForceWeatherNow(int16_t weather);
	static bool ForecastWeather(int weather, int numWeathers);
	static void Init();
	static void ReleaseWeather();
	/* RenderRainStreaks()
	*/ 
    static void SetWeatherToAppropriateTypeNow();
	/* Update()
	* UpdateInTunnelness()
    */
	static void UpdateWeatherRegion(CVector* pos);
    //
    static float GetWetRoads();
	static uint8_t GetWeatherRegion();
private:
    /*
	* CloudCoverage
	* CurrentRainParticleStrength
	* Earthquake
	* ExtraSunnyness
	* Foggyness
	* Foggyness_SF
	*/ 
    static int16_t ForcedWeatherType;
	/* HeadLightsSpectrum
	* HeatHaze
	* HeatHazeFXControl
	* InTunnelness
	* InterpolationValue
	* LightningBurst
	* LightningDuration
	* LightningFlash
	* LightningFlashLastChange
	* LightningStart
	* LightningStartX
	* LightningStartY
	*/ 
    static int16_t NewWeatherType;
	static int16_t OldWeatherType;
	/* Rain
	* Rainbow
	* Sandstorm
	* StreamAfterRainTimer
	* SunGlare
	* TrafficLightsBrightness
	* TwisterIndex
	* TwisterPos
	* UnderWaterness
	* WaterDepth
	* WaterFog
	* WaterFogFXControl
	* Wavyness
	*/ 
    static uint8_t WeatherRegion;
	static uint32_t WeatherTypeInList;
	static float WetRoads;
	/* WhenToPlayLightningSound
	* Wind
	* WindClipped
	*/ 
    static CVector WindDir;
	/* bScriptsForceRain
	* m_WeatherAudioEntity
    */
};