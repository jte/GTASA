#include "StdInc.h"

void CAESound::UnregisterWithPhysicalEntity()
{
    if (m_physicalEntity)
    {
        m_physicalEntity->CleanUpOldReference(&m_physicalEntity);
        m_physicalEntity = NULL;
    }
}

void CAESound::StopSound()
{
    m_playingState = 1;
    UnregisterWithPhysicalEntity();
}

bool CAESound::GetUncancellable()
{
    return m_uncancellable;
}

bool CAESound::GetFrontEnd()
{
    return m_frontEnd;
}

bool CAESound::GetRequestUpdates()
{
    return m_requestUpdates;
}

bool CAESound::GetUnpausable()
{
    return m_unpausable;
}

bool CAESound::GetPlayPhysically()
{
    return m_playPhysically;
}

bool CAESound::GetStartPercentage()
{
    return m_startPercentage;
}

bool CAESound::GetMusicMastered()
{
    return m_musicMastered;
}

bool CAESound::GetLifespanTiedToPhysicalEntity()
{
    return m_lifespanTiedToPhysicalEntity;
}

bool CAESound::GetUnduckable()
{
    return m_unduckable;
}

bool CAESound::GetUncompressable()
{
    return m_uncompressable;
}

bool CAESound::GetRolledOff()
{
    return m_rolledOff;
}

bool CAESound::GetSmoothDucking()
{
    return m_smoothDucking;
}

bool CAESound::GetForcedFront()
{
    return m_forcedFront;
}

void CAESound::SetIndividualEnvironment(uint16_t flags, uint16_t set)
{
    if (set)
    {
        m_environmentFlags |= flags;
    }
    else
    {
        m_environmentFlags &= ~flags;
    }
}

void CAESound::GetRelativePosition(CVector* pos)
{
    if (GetFrontEnd())
    {
        *pos = m_currentPosition;
    }
    else
    {
        CAEAudioEnvironment::GetPositionRelativeToCamera(pos, m_currentPosition);
    }
}

float CAESound::GetRelativePlaybackFrequencyWithDoppler()
{
    if (GetFrontEnd())
    {
        return m_frequency;
    }
    else
    {
        return CAEAudioEnvironment::GetDopplerRelativeFrequency(m_previousCameraDistance,
                                                                m_currentCameraDistance,
                                                                v2,
                                                                m_currentTimeUpdate,
                                                                m_timeScale) * m_frequency;
    }
}

float CAESound::GetSlowMoFrequencyScalingFactor()
{
    if (GetUnpausable() || !CTimer::GetIsSlowMotionActive() || TheCamera.cams[TheCamera.activeCam].Mode == 46)
    {
        return 1.0f;
    }
    else
    {
        return 0.5f;
    }
}

void CAESound::SetPosition(CVector position)
{
    if (m_lastFrameUpdate)
    {
        if (CTimer::GetFrameCounter() == m_lastFrameUpdate)
        {
            m_currentPosition = position;
            m_currentCameraDistance = (m_currentPosition - TheCamera.GetPos()).Magnitude();
            m_currentTimeUpdate = CTimer::GetCurrentTimeMs();
        }
        else
        {
            m_previousPosition = m_currentPosition;
            m_previousCameraDistance = m_currentCameraDistance;
            m_previousTimeUpdate = m_currentTimeUpdate;
            m_currentPosition = position;
            m_currentCameraDistance = (m_currentPosition - TheCamera.GetPos()).Magnitude();
            m_lastFrameUpdate = CTimer::GetFrameCounter();
            m_currentTimeUpdate = CTimer::GetCurrentTimeMs();
        }
    }
    else
    {
        m_currentPosition = position;
        m_previousPosition = m_currentPosition;
        m_previousCameraDistance = (m_currentPosition - TheCamera.GetPos()).Magnitude();
        m_currentCameraDistance = m_previousCameraDistance;
        m_lastFrameUpdate = CTimer::GetFrameCounter();
        m_currentTimeUpdate = CTimer::GetCurrentTimeMs();
        m_previousTimeUpdate = m_currentTimeUpdate;
    }
}

void CAESound::StopSoundAndForget()
{
    StopSound();
    SetIndividualEnvironment(4, false);
    m_baseAudio = 0;
}

void CAESound::CalculateVolume()
{
    if (GetFrontEnd())
    {
        m_finalVolume = m_volume;
        m_finalVolume -= m_soundHeadroom;
    }
    else
    {
        CVector relPos;
        CAEAudioEnvironment::GetPositionRelativeToCamera(relPos, m_currentPosition);
        float attenuation = CAEAudioEnvironment::GetDistanceAttenuation(relPos.Magnitude() / m_soundDistance);
        m_finalVolume = CAEAudioEnvironment::GetDirectionalMikeAttenuation(relPos) + attenuation + m_volume;
        m_finalVolume -= m_soundHeadroom;
    }
}

void CAESound::RegisterWithPhysicalEntity(CEntity* entity)
{
    if (m_physicalEntity)
    {
        m_physicalEntity->CleanUpOldReference(&m_physicalEntity);
        m_physicalEntity = NULL;
    }
    if (entity)
    {
        m_physicalEntity = entity;
        entity->RegisterReference(&m_physicalEntity);
    }
}

void CAESound::NewVPSLentry()
{
    field_5E = 0;
    m_playingState = 0;
    field_5A = 0;
    m_isUsed = 1;
    m_soundHeadroom = AEAudioHardware.GetSoundHeadroom(m_soundIdInSlot, m_bankSlotId);
    CalculateFrequency();
}

void CAESound::CalculateFrequency()
{
    if (m_maxSpeed <= 0.0f || m_maxSpeed >= m_speed)
    {
        m_frequency = this->m_speed;
    }
    else
    {
        m_frequency = CAEAudioUtility::GetRandomNumberInRange(-m_maxSpeed, m_maxSpeed) + m_speed;
    }
}

void CAESound::UpdateFrequency()
{
    if (m_maxSpeed == 0.0)
    {
        m_frequency = m_speed;
    }
}

void CAESound::UpdatePlayTime(uint16_t maxPlayTime, int16_t moveTimeOffset, uint16_t relativeFrequency)
{
    m_maxPlayTime = maxPlayTime;
    if (field_5E == 0)
    {
        if (m_playingState != 0)
        {
            m_currentPlayPosition = -1;
        }
    }
    else
    {
        m_currentPlayPosition = relativeFrequency * m_frequency + m_currentPlayPosition;
        if (m_currentPlayPosition >= m_maxPlayTime)
        {
            if (moveTimeOffset == -1)
            {
                m_currentPlayPosition = -1;
            }
            else
            {
                m_currentPlayPosition = moveTimeOffset + m_currentPlayPosition % maxPlayTime;
            }
        }
    }
}

void CAESound::UpdateParameters(int16_t a2)
{
    if (GetLifespanTiedToPhysicalEntity())
    {
        if (m_physicalEntity)
        {
            SetPosition(m_physicalEntity->GetPos());
        }
        else
        {
            m_playingState = 1;
        }
    }
    if (GetRequestUpdates())
    {
        if (m_baseAudio)
        {
            (**(void (__stdcall ***)(_DWORD, _DWORD))m_baseAudio)(pThis, a2);
            UpdateFrequency();
        }
    }
}

void CAESound::SoundHasFinished()
{
    UpdateParameters(-1);
    m_isUsed = 0;
    if (m_physicalEntity)
    {
        m_physicalEntity->CleanUpOldReference(&m_physicalEntity);
        m_physicalEntity = NULL;
    }
    field_5E = 0;
    m_currentPlayPosition = 0;
}