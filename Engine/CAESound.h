#pragma once

class CAESound
{
/*
    	* CAESound(CAESound&)
	* CAESound(short, short, CAEAudioEntity*, CVector, float, float, float, float, unsigned char, unsigned short, float)
	* Initialise(short, short, CAEAudioEntity*, CVector, float, float, float, float, unsigned char, unsigned short, float, short)
	* operator=(CAESound&)
	* ~CAESound()
    */
    void SoundHasFinished();
    void UpdateParameters(int16_t p);
    void UpdatePlayTime(uint16_t maxPlayTime, int16_t moveTimeOffset, uint16_t relativeFrequency);
    void UpdateFrequency();
    void CalculateFrequency();
    void NewVPSLentry();
    void RegisterWithPhysicalEntity(CEntity* entity);
    void CalculateVolume();
    void StopSoundAndForget();
    void SetPosition(CVector position);
    void GetRelativePosition(CVector* pos);
    void UnregisterWithPhysicalEntity();
    void StopSound();
    bool GetUncancellable();
    bool GetFrontEnd();
    bool GetRequestUpdates();
    bool GetUnpausable();
    bool GetPlayPhysically();
    bool GetMusicMastered();
    bool GetStartPercentage();
    bool GetLifespanTiedToPhysicalEntity();
    bool GetUnduckable();
    bool GetRolledOff();
    bool GetUncompressable();
    bool GetSmoothDucking();
    bool GetForcedFront();
    void SetIndividualEnvironment(uint16_t flags, uint16_t set);
    float GetRelativePlaybackFrequencyWithDoppler();
    float GetSlowMoFrequencyScalingFactor();
private:
    int16_t m_bankSlotId;
    int16_t m_soundIdInSlot;
	class CAEAudioEntity* m_baseAudio;
	class CEntity* m_physicalEntity;
	uint32_t m_event;
	float m_maxVolume;
	float m_volume;
	float m_soundDistance;
	float m_speed;
	float m_maxSpeed;
	CVector m_currentPosition;
	CVector m_previousPosition;
	uint32_t m_lastFrameUpdate;
	uint32_t m_currentTimeUpdate;
	uint32_t m_previousTimeUpdate;
	float m_currentCameraDistance;
	float m_previousCameraDistance;
	float m_timeScale;
	__int8 field_54;
	__int8 field_55;
	union 
    {
		unsigned __int16 m_environmentFlags;
		struct
        {
			unsigned __int16 m_frontEnd : 1;
			unsigned __int16 m_uncancellable : 1;
			unsigned __int16 m_requestUpdates : 1;
			unsigned __int16 m_playPhysically : 1;
			unsigned __int16 m_unpausable : 1;
			unsigned __int16 m_startPercentage : 1;
			unsigned __int16 m_musicMastered : 1;
			unsigned __int16 m_lifespanTiedToPhysicalEntity : 1;
			unsigned __int16 m_unduckable : 1;
			unsigned __int16 m_uncompressable : 1;
			unsigned __int16 m_rolledOff : 1;
			unsigned __int16 m_smoothDucking : 1;
			unsigned __int16 m_forcedFront : 1;
		};
	};
	uint16_t      m_isUsed;
	__int16 field_5A;
	int16_t m_currentPlayPosition;
	__int16 field_5E;
	float m_finalVolume;
	float m_frequency;
	uint16_t m_playingState;
	__int8 field_6A[2];
	float m_soundHeadroom;
	int16_t m_maxPlayTime;
	__int16 field_72;
};