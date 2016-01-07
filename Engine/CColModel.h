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
    CBoundingBox& GetBoundingBox() const;
    CCollisionData* GetColData() const;
	uint8_t GetColNum() const;
	void SetBoundingBox(const CBoundingBox& boundingBox);
	void SetColNum(uint8_t colNum);
private:
    CBoundingBox m_boundingBox;
    uint8_t m_colNum;
    // 0x02 - bHasTrianglePlanes
    // 0x04 - bInitialized
    uint8_t m_flags;
    uint8_t pad2;
    uint8_t pad3;
    CCollisionData* m_colData;
};