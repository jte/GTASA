#pragma once

class CColModel
{
public:
	void AllocateData();
	void AllocateData(int);
	void AllocateData(int, int, int, int, int, bool);
	CColModel();
	void CalculateTrianglePlanes();
	void MakeMultipleAlloc();
	void RemoveCollisionVolumes();
	void RemoveTrianglePlanes();
	void operator delete(void*);
	void* operator new(unsigned int);
	void operator=(CColModel const&);
	~CColModel();
private:
    CBoundingBox boundingBox;
    BYTE level;
    // 0x02 - bHasTrianglePlanes
    // 0x04 - bInitialized
    BYTE unknownFlags;
    BYTE pad [ 2 ];
    CCollisionData* m_pColData;
};