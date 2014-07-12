#pragma once

class CQuadTreeNode
{
public:
	void AddItem(void *pItem, CRect const &sector);
	CQuadTreeNode(CRect const &sector, int);
	void DeleteItem(void*);
	void DeleteItem(void*, CRect const&);
	void FindSector(CRect const&);
	void FindSector(CVector2D const&);
	void ForAllMatching(CRect const&, void (*)(CRect const&, void*));
	void ForAllMatching(CVector2D const&, void (*)(CVector2D const&, void*));
	void GetAll(CPtrListSingleLink &pList);
	void GetAllMatching(CRect const &sector, CPtrListSingleLink &pList);
	void GetAllMatching(CVector2D const &vecSector, CPtrListSingleLink &pList);
	bool InSector(CRect const &sector, int uiSubdivisionPart);
	void InitPool();
	void operator delete(void *pMemory);
	void *operator new(size_t uiSize);
	~CQuadTreeNode();
private:
    static CPool *ms_pQuadTreeNodePool;
    CRect m_sector;
    CPtrNodeSingleLink *m_pItemList;
    CQuadTreeNode *m_pChildrens[4];
    size_t m_uiChildrenUnusedNodeCount;
};