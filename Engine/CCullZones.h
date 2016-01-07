#pragma once

class CCullZones
{
public:
	//* AddCullZone(CVector const&, float, float, float, float, float, float, unsigned short, short)
	//* AddMirrorAttributeZone(CVector const&, float, float, float, float, float, float, unsigned short, float, float, float, float)
	//* AddTunnelAttributeZone(CVector const&, float, float, float, float, float, float, unsigned short)
	//* Cam1stPersonForPlayer()
	//* CamCloseInForPlayer()
	//* CamNoRain()
	//* CamStairsForPlayer()
	//* DoExtraAirResistanceForPlayer()
	//* DoINeedToLoadCollision()
	//* FewerCars()
	//* FewerPeds()
	static uint32_t FindAttributesForCoors(CVector pos);
	//* FindMirrorAttributesForCoors(CVector)
	//* FindTunnelAttributesForCoors(CVector)
	//* FindZoneWithStairsAttributeForPlayer()
	//* InRoomForAudio()
	//* Init()
	//* NoPolice()
	//* PlayerNoRain()
	//* PoliceAbandonCars()
	//* Update()
protected:
private:
	static union {
        uint32_t CurrentFlags_Camera;
        struct
        {
            uint8_t CamCloseInForPlayer : 1;
            uint8_t CamStairsForPlayer : 1;
            uint8_t Cam1stPersonForPlayer : 1;
            uint8_t NoRain : 1;
            uint8_t NoPolice : 1;
            uint8_t f6;
            uint8_t DoINeedToLoadCollision : 1;
            uint8_t f8;

            uint8_t PoliceAbandonCars : 1;
            uint8_t InRoomForAudio : 1;
            uint8_t f11;
            uint8_t f12;
            uint8_t f13;
            uint8_t AllowWeatherInTunnel : 1;
            uint8_t f15;
            uint8_t f16;

            uint8_t FewerPeds : 1;
            uint8_t f18;
            uint8_t f19;
            uint8_t f20;
            uint8_t DoExtraAirResistanceForPlayer : 1;
            uint8_t FewerCars : 1;
            uint8_t f23;
            uint8_t f24;
        };
    };
	static union 
    {
        uint32_t CurrentFlags_Player;
        struct
        {
        };
    };
	/* NumAttributeZones
	* NumMirrorAttributeZones
	* NumTunnelAttributeZones
	* aAttributeZones
	* aMirrorAttributeZones
	* aTunnelAttributeZones
	* bMilitaryZonesDisabled
	* bRenderCullzones*/
};