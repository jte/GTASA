#include "StdInc.h"

CData::CData()
{
    m_array = NULL;
    m_size = 0;
}

CData::~CData()
{
    if (m_array)
    {
        delete[] m_array;
        m_array = NULL;
    }
    m_size = 0;
}

bool CData::Load(uint32_t size, int file, uint32_t* offset, bool skipRead)
{
    m_size = size;
    m_array = new uint8_t[m_size];
    if (!size)
    {
        return true;
    }
    size_t readSize = skipRead;
    uint8_t readVal = 0;
    for (size_t i = 0; i < m_size; i++)
    {
        if (!skipRead)
        {
            readSize = CFileMgr::Read(file, (char*)&readVal, sizeof(readVal));
        }
        if (readSize != 1)
        {
            return false;
        }
        m_array[i] = readVal;
        (*offset)++;
    }
    return true;
}

void CData::Unload()
{
    if (m_array)
    {
        delete[] m_array;
        m_array = NULL;
    }
    m_size = 0;
}

uint8_t* CData::GetArray() const
{
    return m_array;
}

uint32_t CData::GetSize() const
{
    return m_size;
}