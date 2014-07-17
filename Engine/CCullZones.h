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
	static struct {
        union {
            uint32_t flags;

            uint8_t CloseInForPlayer : 1;// 1
            uint8_t StairsForPlayer : 1;  // 2
            uint8_t Person1stForPlayer : 1; // 3
            uint8_t NoRain : 1; // 4
            uint8_t NoPolice : 1; // 5
            uint8_t InTunnel : 1; // 6
            uint8_t NeedToLoadCollision : 1; // 7
            uint8_t flag8 : 1; // 8
            uint8_t PoliceAbandonCars : 1; // 9
            uint8_t InRoomForAudio : 1; // 10
            uint8_t FewerPeds : 1;
            uint8_t flag12 : 1;
            uint8_t flag13 : 1;
            uint8_t flag14 : 1;
            uint8_t DoExtraAirResistanceForPlayer : 1;
            uint8_t FewerCars : 1;

        };
    } CurrentFlags_Camera;
	* CurrentFlags_Player
	* NumAttributeZones
	* NumMirrorAttributeZones
	* NumTunnelAttributeZones
	* aAttributeZones
	* aMirrorAttributeZones
	* aTunnelAttributeZones
	* bMilitaryZonesDisabled
	* bRenderCullzones
};