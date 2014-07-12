#include "StdInc.h"

void *CMemoryMgr::MallocAlign(unsigned int uiSize, unsigned int uiAlign)
{
    void *pMemory = malloc(uiSize + uiAlign);
    void **pAligned = (void**)(((size_t)pMemory + uiAlign) & ~(uiAlign - 1));
    pAligned[-1] = pMemory;
    return pAligned;
}

void CMemoryMgr::FreeAlign(void **pAligned)
{
    free(pAligned[-1]);
}

void CMemoryMgr::Free(void *pMemory)
{
    RwFree(pMemory);
}

void CMemoryMgr::Init()
{
}

void CMemoryMgr::ReleaseScratchPad()
{
}

void CMemoryMgr::Shutdown()
{
}

void *CMemoryMgr::InternalMalloc(unsigned int size)
{
    if(size > 2048)
    {
        void *mem = CMemoryHeap::Malloc(size);
        if(!mem)
        {
            return CMemoryHeap::Malloc(size);
        }
    }
    return NULL;
}

void CMemoryMgr::InitScratchPad()
{
    return CMemoryHeap::Init(&dword_C87BD0, &PC_Scratch, 0x4000u, 0);
}

void CMemoryMgr::LockScratchPad()
{
}

void *CMemoryMgr::MoveMemory(void *mem)
{
    return mem;
}

void *CMemoryMgr::MoveMemory(void *mem, void**, int)
{
    return mem;
}

void CMemoryMgr::InitMemoryTracking(int block_count)
{
    m_memUsed = 0;
    PushMemId(0);
    PushMemId(-1);
}