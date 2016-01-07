#include "StdInc.h"

bool FxMemoryPool_c::Init()
{
    m_size = 0x100000;
    m_position = 0;
    m_ptr = (char*)CMemoryMgr::Malloc(m_size);
    if (!m_ptr)
    {
        return false;
    }
    memset(m_ptr, 0, m_size);
    return true;
}

void FxMemoryPool_c::Exit()
{
    if (m_ptr)
    {
        CMemoryMgr::Free((void*)m_ptr);
    }
}

void FxMemoryPool_c::Reset()
{
    m_position = 0;
}

void* FxMemoryPool_c::GetMem(int size, int align)
{
    size_t alignedPos = m_position % align;
    if (alignedPos > 0)
    {
        m_position = align + m_position - alignedPos;
    }
    void* mem = &m_ptr[m_position];
    m_position += size;
    return mem;
}

void FxMemoryPool_c::Optimise()
{
    m_ptr = CMemoryMgr::Realloc(m_ptr, m_position);
    m_size = m_position;
}