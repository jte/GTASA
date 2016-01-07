#include "StdInc.h"

CEventGroup::CEventGroup(CPed* ped)
{
    m_ped = ped;
    m_count = 0;
    memset(m_events, 0, sizeof(m_events));
}

bool CEventGroup::Add(CEvent& event, bool a4)
{
  CEventGroup *v4; // edi@1
  char v5; // bl@2
  CPedIntelligence *v12; // ebx@18
  int v13; // ST04_4@18
  int v14; // eax@18
  int v15; // esi@20
  CEventGroup *v17; // [sp-4h] [bp-10h]@1
  void *v18; // [sp+Ch] [bp+0h]@3

  v17 = pThis;
  v4 = a1;
    if (m_ped)
    {
        bool triggerLookAt = false;
        if (event.HasEditableResponse())
        {
            CEventEditableResponse& eventEditable = (CEventEditableResponse&)event;
            triggerLookAt = eventEditable.ComputeResponseTaskOfType(m_ped, 1200);
            a4 = eventEditable.ComputeResponseTaskOfType(m_ped, 1700);
            BYTE2(v18) = eventEditable.ComputeResponseTaskOfType(m_ped, 300);
            CEventEditableResponse::ComputeResponseTaskType(m_ped, 0);
            if (!CEventEditableResponse::WillRespond(a3))
            {
                if ( (*(int (__thiscall **)(CEventEditableResponse *))(a3->__parent.__vmt__ + 4))(a3) != 9 || !a3->field_C )
                BYTE3(v18) = 0;
            }
        }
        if (!event.AffectsPed(m_ped))
        {
            return false;
        }
        if (event.HasEditableResponse())
        {
            CEventEditableResponse& eventEditable = (CEventEditableResponse&)event;
            if (triggerLookAt)
            {
                eventEditable.TriggerLookAt(m_ped);
            }
            eventEditable.InformVehicleOccupants(m_ped);
        }
        m_ped->GetPedIntelligence()->RecordEventForScript(event.GetEventType(), event.GetEventPriority());
    }
    if (m_count >= 16)
    {
        return false;
    }
    CEvent* clonedEvent = event.Clone();
  *(_BYTE *)(clonedEvent + 8) = a4;
  if ( v4->pPed )
    (*(void (__thiscall **)(int, CPed *))(*(_DWORD *)clonedEvent + 32))(clonedEvent, v4->pPed);
  v4->pEvents[v4->count++] = clonedEvent;
    return true;
}