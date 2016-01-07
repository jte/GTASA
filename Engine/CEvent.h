#pragma once

class CPed;
class CPedGroup;
class CEntity;

enum eEventType
{
    EVENT_NULL = 0,
    EVENT_ASSAULT,
    EVENT_RUN_REDLIGHT,
    EVENT_ASSAULT_POLICE,
    EVENT_GUNSHOT,
    EVENT_INJURED_PED,
    EVENT_DEAD_PED,
    EVENT_FIRE,
    EVENT_STEAL_CAR,
    EVENT_HIT_AND_RUN,
    EVENT_HIT_AND_RUN_COP,
    EVENT_SHOOT_PED,
    EVENT_SHOOT_COP,
    EVENT_EXPLOSION,
    EVENT_PED_SET_ON_FIRE,
    EVENT_COP_SET_ON_FIRE,
    EVENT_CAR_SET_ON_FIRE,
    EVENT_ASSAULT_NASTYWEAPON,
    EVENT_ASSAULT_NASTYWEAPON_POLICE,
    // extra eventy - not strictly 'events' but used to notice interesting stuff
    EVENT_ICECREAM,     /////
    EVENT_ATM,          //  // queue for these 3
    EVENT_SHOPSTALL,    /////
    EVENT_SHOPWINDOW,
    EVENT_LAST_EVENT
};

enum eEventEntityType
{
    ENTITY_EVENT_PED = 0,
    ENTITY_EVENT_VEHICLE,
    ENTITY_EVENT_OBJECT
};

class CEvent
{
public:
    // virtual methods
    virtual ~CEvent();
    virtual eEventType GetEventType() const = 0;
    virtual int GetEventPriority() const = 0;
    virtual size_t GetLifeTime() const = 0;
    virtual CEvent* Clone() const = 0;
    virtual bool AffectsPed(CPed* ped) const;
    virtual bool AffectsPedGroup(CPedGroup* pedGroup) const;
    virtual bool IsCriminalEvent() const;
    virtual void ReportCriminalEvent(CPed* ped) const;
    virtual bool HasEditableResponse() const;
    virtual CEntity* GetSourceEntity() const;
    virtual bool TakesPriorityOver(const CEvent& other) const;
    virtual float GetLocalSoundLevel() const;
    virtual bool DoInformVehicleOccupants(CPed* ped) const;
    virtual bool IsValid(CPed* ped) const; //TODO
    virtual bool CanBeInterruptedBySameEvent() const;
	/* 
	* GetSoundLevel(CEntity const*, CVector const&) const
	* 
	*/ 
    CEvent();
    float CalcSoundLevelIncrement(float a1, float a2);
    void operator delete(void* p);
    void* operator new(size_t);
private:
    eEventType m_eventType;
    eEventEntityType m_eventEntity;
};