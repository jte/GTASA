class CTask
{
public:
    CTask();
  
	* CreateTask()
	* IsGoToTask(CTask const*)
	* IsTaskPtr(void*)
	* Serialize()
	* operator delete(void*)
	* operator new(unsigned int)
protected:
    virtual ~CTask();
    virtual CTask *Clone() const = 0;
    virtual CTask *GetSubTask() const = 0;
    virtual bool IsSimpleTask() const = 0;
    virtual TaskType GetType() const = 0;
    virtual void StopTimer(CEvent const* event) = 0;
    virtual void MakeAbortable(CPed *ped, int, int) = 0;
    virtual void ProcessPed(CPed *ped) = 0;
    virtual bool SetPedPosition(CPed *ped) = 0;
private:
    CTask *m_parent_task;
};