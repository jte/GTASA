
class CTaskManager
{
public:
	AddSubTasks(CTask*)
	CTaskManager(CPed *ped);
	ClearTaskEventResponse()
	CTask *FindActiveTaskByType(int type) const;
	FindTaskByType(int, int) const
	Flush()
	FlushImmediately()
	CTask *GetActiveTask() const;
	GetSimplestActiveTask() const
	GetSimplestTask(CTask*)
	GetSimplestTask(int) const
	GetTaskSecondary(int) const
	HasTaskSecondary(CTask const*) const
	ManageTasks()
	ParentsControlChildren(CTask*)
	SetNextSubTask(CTask*)
	SetTask(CTask*, int, bool)
	SetTaskSecondary(CTask*, int)
	StopTimers(CEvent const*)
	~CTaskManager();
protected:
private:
    CTask m_primary_tasks[5];
    CTask m_secondary_tasks[6];
    CPed *m_ped;
};