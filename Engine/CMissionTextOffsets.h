#pragma once

struct MissionTextOffset
{
    char name[8];
    uint32_t offset;
};

class CMissionTextOffsets
{
public:
    CMissionTextOffsets();
    void Load(uint32_t size, int file, uint32_t* offset, int);
    //
    MissionTextOffset GetTextOffset(uint32_t id) const;
    uint32_t GetSize() const;
private:
    MissionTextOffset m_offsets[200];
    uint32_t m_size;
};