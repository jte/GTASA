#include "CTaskSimpleAnim.h"

CTaskSimpleAnim::CTaskSimpleAnim(bool b1)
{
    m_ucFlags &= ~0x07;
    if(b1)
    {
        m_ucFlags |= 0x04;
    }
}

CTaskSimpleAnim::~CTaskSimpleAnim()
{
    if(m_pAnimAssoc)
    {
        m_pAnimAssoc->SetFinishCallback(NULL, 0);
        m_pAnimAssoc->ClumpAnimAssoc.usFlags |= 0x04;
        if(!(m_ucFlags & 0x04))
        {
            if(m_pAnimAssoc->ClumpAnimAssoc.fBlendAmount > 0.0 && m_pAnimAssoc->ClumpAnimAssoc.fBlendDelta >= 0.0)
            {
                m_pAnimAssoc->ClumpAnimAssoc.fBlendDelta = -4.0;
            }
        }
        m_pAnimAssoc = NULL;
    }
}

bool CTaskSimpleAnim::MakeAbortable(CPed *pPed, int uiReason, CEvent const *pEvent)
{
    if(uiReason == 2)
    {
        
    }
    else if(m_ucFlags & 0x02)
    {
        if(pEvent && pEvent->GetId() != 32)
        {
            return false;
        }
    }
    if(pEvent && pEvent->GetId() == 32)
    {
        if(pEvent->field_10)
        {
            if(pEvent->field_10->GetId() == 401)
            {
                if(
            }
        }
    }
}