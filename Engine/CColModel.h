#pragma once

#include "Engine\CBoundingBox.h"

class CCollisionData;

class CColModel
{
public:
	void AllocateData(); // DONE
	void AllocateData(int);
	void AllocateData(int, int, int, int, int, bool);
	CColModel(); // DONE
	void CalculateTrianglePlanes(); // DONE
	void MakeMultipleAlloc(); // DONE
	void RemoveCollisionVolumes(); // DONE
	void RemoveTrianglePlanes(); // DONE
	void operator delete(void* object); // DONE
	void* operator new(uint32_t size); // DONE
	CColModel& operator=(CColModel const&); // DONE
	~CColModel(); // DONE

    //
    const CBoundingBox& GetBoundingBox() const;
    const CCollisionData* GetColData() const;
private:
    CBoundingBox m_boundingBox;
    uint8_t pad1;
    // 0x02 - bHasTrianglePlanes
    // 0x04 - bInitialized
    uint8_t m_flags;
    uint8_t pad2;
    uint8_t pad3;
    CCollisionData* m_colData;
};