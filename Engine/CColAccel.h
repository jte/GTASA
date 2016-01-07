#pragma once

struct CacheColModel
{
	CBoundingBox boundingBox;
	int32_t modelId;
	uint8_t colNum;
	uint8_t colModelFlag;
	uint8_t pad1;
	uint8_t pad2;
};

class CColAccel
{
public:
    static void addCacheCol(int modelId, const CColModel& colModel);
	/* addColDef(ColDef)
	* addIPLEntity(CEntity**, int, int)
	* cacheIPLSection(CEntity**, int)
	*/ 
	static void cacheLoadCol();
	static void endCache();
	/* getColDef(ColDef&)
	* getIplDef(int)
	* setIplDef(int, IplDef)
	* startCache()
    */
    static bool isCacheLoading();
    static IplDef getIplDef(size_t id);
private:
    static int32_t m_iCacheState;
	static int32_t m_iNumColItems;
	static int32_t m_iNumSections;
	static int32_t m_iNumColBounds;
	static int32_t m_iNumIPLItems;
	static int32_t* m_iSectionSize;
    static IplDef* m_iplDefs; // pointer to array "IplDef[256]"
	static const char mp_cCacheName[];
	static CacheColModel* mp_caccColItems;
};