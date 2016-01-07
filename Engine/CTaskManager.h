#pragma once

class CTask;
class CPed;

class CTaskManager
{
public:
	void AddSubTasks(CTask* task);
	CTaskManager(CPed* ped);
	void ClearTaskEventResponse();
	CTask* FindActiveTaskByType(int taskType) const;
	CTask* FindTaskByType(int id, int type) const;
	/* Flush()
	* FlushImmediately()
	*/ 
	CTask* GetActiveTask() const;
	/* GetSimplestActiveTask() const
	*/ 
	CTask* GetSimplestTask(CTask* task);
	/* GetSimplestTask(int) const
	*/ 
	CTask* GetTaskSecondary(int id) const;
	/* HasTaskSecondary(CTask const*) const
	* ManageTasks()
	* ParentsControlChildren(CTask*)
	* SetNextSubTask(CTask*)
	*/ 
	void SetTask(CTask*, int, bool);
	/* SetTaskSecondary(CTask*, int)
	* StopTimers(CEvent const*)
	*/ 
	~CTaskManager();
private:
	CTask* m_primaryTasks[5];
	CTask* m_secondaryTasks[6];
	CPed* m_ped;
};