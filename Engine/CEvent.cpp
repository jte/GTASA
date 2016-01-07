#include "StdInc.h"

void* CEvent::operator new(size_t)
{
    return CPools::GetEventPool()->New();
}

void CEvent::operator delete(void* p)
{
    CPools::GetEventPool()->Delete((CEvent*)p);
}

bool CEvent::AffectsPed(CPed* ped) const
{
    return true;
}

bool CEvent::AffectsPedGroup(CPedGroup* pedGroup) const
{
    return true;
}

bool CEvent::IsCriminalEvent() const
{
    return false;
}

bool CEvent::HasEditableResponse() const
{
    return false;
}

CEntity* CEvent::GetSourceEntity() const
{
    return NULL;
}

bool CEvent::TakesPriorityOver(const CEvent& other) const
{
    return GetEventPriority() >= other.GetEventPriority();
}

float CEvent::GetLocalSoundLevel() const
{
    return 0.0f;
}

bool CEvent::DoInformVehicleOccupants(CPed* ped) const
{
    return false;
}

bool CEvent::CanBeInterruptedBySameEvent() const
{
    return false;
}

float CEvent::CalcSoundLevelIncrement(float a1, float a2)
{
    if (a2 == 0.0f)
    {
        return 0.0f;
    }
    else if (a1 == 0.0f)
    {
        return a2;
    }
    else
    {
        float v3 = powf(10.0f, a1 / 10);
        return log10f((pow(10.0f, a2 * 0.1f) + v3) / v3) * 10.0f;
    }
}

CEvent::CEvent()
{
    m_eventType = 0;
    m_eventEntity = 0;
}