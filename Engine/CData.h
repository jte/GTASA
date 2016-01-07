#pragma once

class CData
{
public:
    CData();
    ~CData();
    bool Load(uint32_t size, int file, uint32_t* offset, bool skipRead);
    void Unload();
    //
    uint8_t* GetArray() const;
    uint32_t GetSize() const;
private:
    uint8_t* m_array;
    uint32_t m_size;
};