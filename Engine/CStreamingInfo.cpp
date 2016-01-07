#include "StdInc.h"

void CStreamingInfo::Init()
{
    m_loadStatus = 0;
    m_fileOffset = 0;
    m_fileSize = 0;
    pad2 = 0;
    m_nextIndex = -1;
    m_prevIndex = -1;
    pad1 = -1;
}

void CStreamingInfo::AddToList(CStreamingInfo* info)
{
    m_nextIndex = info->m_nextIndex;
    m_prevIndex = GET_INDEX_FROM_BASE(info, ms_pArrayBase);
    info->m_nextIndex = GET_INDEX_FROM_BASE(this, ms_pArrayBase);
    ms_pArrayBase[m_nextIndex].m_prevIndex = info->m_nextIndex;
}

void CStreamingInfo::RemoveFromList()
{
    CStreamingInfo::ms_pArrayBase[m_nextIndex].m_prevIndex = m_prevIndex;
    CStreamingInfo::ms_pArrayBase[m_prevIndex].m_nextIndex = m_nextIndex;
    m_nextIndex = -1;
    m_prevIndex = -1;
}

CStreamingInfo* CStreamingInfo::GetNext()
{
    return (m_nextIndex == -1) ? 0 : &ms_pArrayBase[m_nextIndex];
}

CStreamingInfo* CStreamingInfo::GetPrev()
{
    return (m_prevIndex == -1) ? 0 : &ms_pArrayBase[m_prevIndex];
}

bool CStreamingInfo::InList()
{
    return m_nextIndex != -1;
}

uint32_t CStreamingInfo::GetCdSize()
{
    return m_fileSize;
}