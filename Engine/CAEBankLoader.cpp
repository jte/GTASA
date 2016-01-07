#include "StdInc.h"

CAEBankLoader::CAEBankLoader()
{
    m_initialized = false;
    m_streamingChannel = 4;
}

CAEBankLoader::~CAEBankLoader()
{
    if (m_initialized)
    {
        delete m_bankSlots;
        CMemoryMgr::Free(m_soundBuffers);
        delete m_bankLookup;
        delete m_streamHandles;
    }
}

void CAEBankLoader::InitialiseRequestList()
{
    for (size_t i = 0; i < ELEMS_COUNT(m_soundRequests); i++)
    {
        m_soundRequests[i].m_bankSlot = -1;
        m_soundRequests[i].m_bank = -1;
        m_soundRequests[i].m_sound = -1;
        m_soundRequests[i].m_bankSlot = NULL;
        m_soundRequests[i].m_loadingStatus = 0;
    }
    pad1 = 0;
    m_requestsToLoad = 0;
    m_requestListNext = 0;
}

void CAEBankLoader::CalculateBufferSize()
{
    size_t sumOffset = 0;
    for (size_t i = 0; i < m_numBankSlots; i++)
    {
        m_bankSlots[i].m_offsetOnBuffer = sumOffset;
        sumOffset += m_bankSlots[i].m_slotBufferSize;
    }
}

bool CAEBankLoader::LoadBankLookupFile()
{
    int file = CFileMgr::OpenFile("AUDIO\\CONFIG\\BANKLKUP.DAT", "rb");
    if (file <= 0)
    {
        return false;
    }
    size_t totalSize = CFileMgr::GetTotalSize(file);
    if (!totalSize)
    {
        CFileMgr::CloseFile(file);
        return false;
    }
    m_numBanks = totalSize / sizeof(CAEBankSlotItem);
    m_bankLookup = new CAEBankLookupItem[m_numBanks];
    if (CFileMgr::Read(file, (char*)m_bankLookup, totalSize) != totalSize)
    {
        CFileMgr::CloseFile(file);
        return false;
    }
    CFileMgr::CloseFile(file);
    return true;
}

bool CAEBankLoader::LoadBankSlotFile()
{
    int file = CFileMgr::OpenFile("AUDIO\\CONFIG\\BANKSLOT.DAT", "rb");
    if (file <= 0)
    {
        return false;
    }
    size_t totalSize = CFileMgr::GetTotalSize(file);
    if (totalSize <= 1)
    {
        CFileMgr::CloseFile(file);
        return false;
    }
    if (!CFileMgr::Read(file, (char*)&m_numBankSlots, sizeof(m_numBankSlots)))
    {
        CFileMgr::CloseFile(file);
        return false;
    }
    m_bankSlots = new CAEBankSlot[m_numBankSlots];
    if (!m_bankSlots)
    {
        CFileMgr::CloseFile(file);
        return false;
    }
    if (CFileMgr::Read(file, (char*)m_bankSlots, totalSize - 2) != totalSize - 2)
    {
        CFileMgr::CloseFile(file);
        delete[] m_bankSlots;
        return false;
    }
    CFileMgr::CloseFile(file);
    CalculateBufferSize();
    m_soundBuffersSize = m_bankSlots[m_numBankSlots - 1].m_offsetOnBuffer + m_bankSlots[m_numBankSlots - 1].m_slotBufferSize;
    m_soundBuffers = (char*)CMemoryMgr::Malloc(m_soundBuffersSize);
    return true;
}

void* CAEBankLoader::GetSoundBuffer(uint16_t bankSlotId, int* slotSize)
{
    if (!m_initialized )
    {
    return 0;
    }
    if (bankSlotId >= m_numBankSlots)
    {
        slotSize = 0;
        return NULL;
    }
    *slotSize = m_bankSlots[bankSlotId].m_slotBufferSize;
    return &m_soundBuffers[m_bankSlots[bankSlotId].m_offsetOnBuffer];
}

bool CAEBankLoader::Initialise()
{
    InitialiseRequestList();
    if (LoadBankSlotFile() && LoadBankLookupFile() && LoadSFXPakLookupFile())
    {
        m_initialized = 1;
        return true;
    }
    else
    {
        return false;
    }
}