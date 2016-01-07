#include "StdInc.h"

CKeyArray::CKeyArray()
{
    m_array = NULL;
    m_size = 0;
}

CKeyArray::~CKeyArray()
{
    if (m_array)
    {
        delete[] m_array;
        m_array = NULL;
    }
    m_size = 0;
}

bool CKeyArray::Load(uint32_t size, int file, uint32_t* offset, bool skipRead)
{
    m_size = size / sizeof(CKeyEntry);
    m_array = (uint8_t*)new CKeyEntry[m_size];
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

void CKeyArray::Unload()
{
    if (m_array)
    {
        delete[] m_array;
        m_array = NULL;
    }
    m_size = 0;
}

uint16_t* CKeyArray::Search(const char* key, uint8_t* found)
{
    uint32_t hash = CKeyGen::GetUppercaseKey(key);
    CKeyEntry* result = BinarySearch(hash, (CKeyEntry*)m_array, 0, m_size - 1);
    *found =  result != NULL;
    return *found ? result->data : NULL;
}

CKeyEntry* CKeyArray::BinarySearch(uint32_t key, CKeyEntry* array, uint16_t firstIndex, uint16_t lastIndex)
{
    while (firstIndex <= lastIndex)
    {
        uint16_t pivot = (lastIndex + firstIndex) / 2;
        if (key == array[pivot].keyHash)
        {
            return &array[pivot];
        }
        else if (key > array[pivot].keyHash)
        {
            firstIndex = pivot + 1;
        }
        else
        {
            lastIndex = pivot - 1;
        }
    }
    return NULL;
}

void CKeyArray::Update(uint16_t* p)
{
    CKeyEntry* array = (CKeyEntry*)m_array;
    for (size_t i = 0; i < m_size; i++)
    {
        array[i].data += (uint32_t)p;
    }
}