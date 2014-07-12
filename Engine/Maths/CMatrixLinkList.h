class CMatrixLinkList
{
public:
	CMatrixLink *AddToList1();
	CMatrixLink *AddToList2();
	//* GetNumFree()
	//* GetNumUsed1()
	//* GetNumUsed2()
	void Init(int count);
	//* MoveToFreeList(CMatrixLink*)
	//* MoveToList1(CMatrixLink*)
	void MoveToList2(CMatrixLink *pLink);
	//* Shutdown()
	//* ~CMatrixLinkList()
//protected:
//private:
    CMatrixLink m_list1_head, m_list1_tail;
    CMatrixLink m_list2_head, m_list2_tail;
    CMatrixLink m_freelist_head, m_freelist_tail;
    std::vector<CMatrixLink*> m_links;
};