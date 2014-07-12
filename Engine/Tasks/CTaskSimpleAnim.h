class CTaskSimpleAnim : public CTaskSimple
{
public:
	CTaskSimpleAnim(bool b1);
	* FinishRunAnimCB(CAnimBlendAssociation*, void*)
	bool MakeAbortable(CPed*, int, CEvent const*);
	* ~CTaskSimpleAnim()
private:
    CAnimBlendAssociation *m_pAnimAssoc;
    uint8_t m_ucFlags;
};