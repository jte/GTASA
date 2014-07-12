

class CEntryInfoList
{
	public:
		static void Flush(CEntryInfoList **ppList);
	protected:
	private:
		CPtrNodeDouble
			m_pNext;
		CPtrNodeDouble
			*m_pDoubleNode;
		CEntryInfoNode
			*m_pUnknown;
		uint32_t
			m_uiSize;
		CEntryInfoNode
			*m_pUnknown2;
};