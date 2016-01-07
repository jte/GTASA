#include "StdInc.h"

CMissionTextOffsets::CMissionTextOffsets()
{
    m_size = 0;
}

void CMissionTextOffsets::Load(uint32_t size, int file, uint32_t* offset, int)
{
    m_size = size / sizeof(MissionTextOffset);
    if (!m_size)
    {
        return;
    }
    uint8_t readVal = 0;
    for (size_t i = 0; i < m_size; i++)
    {
        for (size_t j = 0; j < ELEMS_COUNT(m_offsets[i].name); j++)
        {
            CFileMgr::Read(file, (char*)&readVal, sizeof(readVal));
            (*offset)++;
        }
        char readOffset[4] = {0,0,0,0};
        for (size_t j = 0; j < sizeof (m_offsets[i].offset); j++)
        {
            CFileMgr::Read(file, (char*)&readOffset[j], sizeof(readOffset[j]));
            (*offset)++;
        }
        m_offsets[i].offset = (uint32_t)readOffset;
    }
}

MissionTextOffset CMissionTextOffsets::GetTextOffset(uint32_t id) const
{
    return m_offsets[id];
}

uint32_t CMissionTextOffsets::GetSize() const
{
    return m_size;
}