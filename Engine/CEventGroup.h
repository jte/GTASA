#pragma once

class CPed;
class CEvent;

class CEventGroup
{
public:
    CEventGroup(CPed* ped);
    bool Add(CEvent& event, bool a);
    /* Add(CEvent&, bool)
	* Flush(bool)
	* GetEventOfType(int) const
	* GetHighestPriorityEvent() const
	* HasEvent(CEvent const*) const
	* HasEventOfType(CEvent const*) const
	* HasScriptCommandOfTaskType(int) const
	* Remove(CEvent*)
	* RemoveInvalidEvents(bool)
	* Reorganise()
	* TickEvents()
	* ~CEventGroup()
    */
private:
    CPed* m_ped;
    size_t m_count;
    CEvent** m_events;
};