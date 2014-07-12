#pragma once

#undef MoveMemory

class CMemoryMgr
{
public:
	static void *Calloc(unsigned int, unsigned int);
	static void Free(void *pMemory); // DONE
	static void FreeAlign(void **pAligned); // DONE
	static size_t GetBlocksUsed(int);
	static void *GetLargestFreeBlock();
	static size_t GetMemoryUsed(int);
	static size_t GetSizeOfHoles();
	static void Init(); // DONE
	static void InitMemoryTracking(int block_count);
	static void InitScratchPad(); // DONE
	static void *InternalMalloc(unsigned int size); // DONE
	static void *InternalRealloc(void*, unsigned int);
	static void LockScratchPad(); // DONE
	static void *Malloc(unsigned int);
	static void *MallocAlign(unsigned int uiSize, unsigned int uiAlign); // DONE
	//* MallocAlignFromScratchPad(unsigned int, unsigned int)
	//* MallocFromScratchPad(unsigned int)
	static void *MoveMemory(void *mem); // DONE
	static void *MoveMemory(void *mem, void**, int); // DONE
	//* PopMemId()
	//* PrintFrequencies()
	//* PushMemId(int)
	//* Realloc(void*, unsigned int)
	//* RegisterFree(void*)
	//* RegisterMalloc(void*)
	static void ReleaseScratchPad(); // DONE
	//* SetRestrictMemoryMove(bool)
	static void Shutdown(); // DONE
private:
	static bool m_bMallocHintDebugging;
	static size_t m_blocksUsed;
	static size_t m_id;
	static size_t m_idCapture;
	static size_t m_idStack;
	static size_t m_largeAllocation;
	static size_t m_memUsed;
	static size_t m_minLargeAllocation;
	static void *m_pBlockArray;
	static void *m_pMemUsedArray;
};