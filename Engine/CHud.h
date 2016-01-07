#pragma once

#include "CSprite2d.h"

class CHud
{
public:
	void Initialise();
	/*
	        * Draw()
        * DrawAfterFade()
        * DrawAreaName()
        * DrawBustedWastedMessage()
        * DrawCrossHairs()
        * DrawFadeState(DRAW_FADE_STATE, int)
        * DrawHelpText()
        * DrawMissionTimers()
        * DrawMissionTitle()
        * DrawOddJobMessage(unsigned char)
        * DrawRadar()
        * DrawScriptText(unsigned char)
        * DrawSubtitles()
        * DrawSuccessFailedMessage()
        * DrawVehicleName()
        * DrawVitalStats()
        * GetRidOfAllHudMessages(unsigned char)
        * GetYPosBasedOnHealth(unsigned char, float, signed char)
        * ReInitialise()
        * ResetWastedText()
        * SetBigMessage(unsigned short*, unsigned short)
        * SetHelpMessage(char const*, unsigned short*, bool, bool, bool, unsigned int)
        * SetHelpMessageStatUpdate(unsigned char, unsigned short, float, float)
        * SetMessage(unsigned short*)
        * SetVehicleName(unsigned short*)
        * SetZoneName(unsigned short*, unsigned char)
        * Shutdown()
	*/
private:
	CSprite2d Sprites[6];
	/*
        * bDrawingVitalStats
        * bScriptDontDisplayAreaName
        * bScriptDontDisplayRadar
        * bScriptDontDisplayVehicleName
        * bScriptForceDisplayWithCounters
        * m_BigMessage
        * m_DisplayScoreFadeTimer
        * m_DisplayScoreState
        * m_DisplayScoreTimer
        * m_EnergyLostFadeTimer
        * m_EnergyLostState
        * m_EnergyLostTimer
        * m_ItemToFlash
        * m_LastBreathTime
        * m_LastDisplayScore
        * m_LastTimeEnergyLost
        * m_LastWanted
        * m_LastWeapon
        * m_Message
        * m_VehicleFadeTimer
        * m_VehicleNameTimer
        * m_VehicleState
        * m_WantedFadeTimer
        * m_WantedState
        * m_WantedTimer
        * m_Wants_To_Draw_3dMarkers
        * m_Wants_To_Draw_Hud
        * m_WeaponFadeTimer
        * m_WeaponState
        * m_WeaponTimer
        * m_ZoneFadeTimer
        * m_ZoneNameTimer
        * m_ZoneState
        * m_ZoneToPrint
        * m_pLastVehicleName
        * m_pLastZoneName
        * m_pVehicleName
        * m_pVehicleNameToPrint
        * m_pZoneName
		*/
};