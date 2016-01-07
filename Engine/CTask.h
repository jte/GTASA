#pragma once

enum eTaskType
{
};

class CTask
{
public:
    // virtual methods
    virtual ~CTask();//0
    virtual CTask* Clone() = 0;//4
    virtual CTask* GetSubTask() const = 0;//8
    virtual bool IsSimpleTask() const = 0;//12 0xc
    virtual eTaskType GetTaskType() = 0;//16 0x10
    virtual void StopTimer(CEvent* event) = 0; //20 0x14
    virtual void MakeAbortable(CPed* ped, int priority, const CEvent* event) = 0;//24 0x18
    //
    /*
	* CreateTask()
	* IsGoToTask(CTask const*)
	* IsTaskPtr(void*)
	* Serialize()
	* StopTimer(CEvent const*)
	*/ 
    void operator delete(void* p);
    void* operator new(size_t);
    CTask();
private:
    CTask* m_parent;
};