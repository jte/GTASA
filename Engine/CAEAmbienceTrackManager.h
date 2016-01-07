#pragma once

class CAEAmbienceTrackManager
{
public:
	/* CheckForPause()
	* GetAmbienceTrackStatus()
	* Initialise(int)
	* IsAmbienceRadioActive()
	* IsAmbienceTrackActive()
	* PlaySpecialMissionAmbienceTrack(int)
	* Reset()
	* Service(int)
	* StartTrackPlayback()
	* StopAmbienceTrack()
	* StopSpecialMissionAmbienceTrack()
	* UpdateAmbienceTrackAndVolume()
	* ~CAEAmbienceTrackManager()
	*/
private:
	/*
	00000000 bAmbienceTrackOverrideRadio db ?
00000001 field_1         db ?
00000002 field_2         db ?
00000003 ambienceRadioStationId db ?
00000004 channelNum      dw ?
00000006 field_6         db ?
00000007 field_7         db ?
00000008 playState       dd ?
0000000C uiSpecialMissionAmbienceTrack dd ?
00000010 field_10        dd ?
00000014 field_14        dd ?
00000018 volume          dd ?
0000001C frequencyScalingFactor dd ?
00000020 unknown         dd ?
00000024 field_24        dd ?
00000028 field_28        db ?
00000029 field_29        db ?
0000002A field_2A        db ?
0000002B field_2B        db ?
0000002C field_2C        dd ?
00000030 field_30        dd ?
00000034 field_34        dw ?
00000036 field_36        db ?
00000037 field_37        db ?
	*/
};