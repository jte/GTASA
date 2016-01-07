#pragma once

#include "CRect.h"
#include "CPool.h"
#include "CPtrNodeSingleLink.h"

/* spatial division via quad trees */
typedef void (*RectSectorMatchingCB)(const CRect&, void*);
typedef void (*Vec2dSectorMatchingCB)(const CVector2D&, void*);
class CQuadTreeNode
{
public:
	void AddItem(void* item, const CRect& sector);
	CQuadTreeNode(const CRect& sector, int numFreeNodes);
	/*void DeleteItem(void*);
	void DeleteItem(void*, CRect const&);
	*/
    void ForAllMatching(const CRect& sector, RectSectorMatchingCB callback);
	void ForAllMatching(const CVector2D& sector, Vec2dSectorMatchingCB callback);
	/*void GetAll(CPtrListSingleLink &pList);
	*/
    void GetAllMatching(const CRect& sector, CPtrListSingleLink& list);
    void GetAllMatching(const CVector2D& sector, CPtrListSingleLink& list);
	/*void InitPool();
	void operator delete(void *pMemory);
	void *operator new(size_t uiSize);
	~CQuadTreeNode();*/
    int FindSector(const CRect& rect);
    int FindSector(const CVector2D& vector);
    bool InSector(const CRect& sector, int quadrant);
private:
    static CPool<int>* m_quadTreeNodePool;
    CRect m_sector;
    CPtrListSingleLink* m_itemList;
    CQuadTreeNode* m_childrens[4];
    size_t m_numFreeNodes;
};