#include "StdInc.h"

CTaskManager::CTaskManager(CPed* ped)
{
	m_ped = ped;
	memset(m_primaryTasks, NULL, sizeof(m_primaryTasks));
	memset(m_secondaryTasks, NULL, sizeof(m_secondaryTasks));
}

CTaskManager::~CTaskManager()
{
	for (size_t i = 0; i < ELEMS_COUNT(m_primaryTasks); i++)
	{
		if (m_primaryTasks[i])
		{
			delete m_primaryTasks[i];
		}
		m_primaryTasks[i] = NULL;
	}
	for (size_t i = 0; i < ELEMS_COUNT(m_secondaryTasks); i++)
	{
		if (m_secondaryTasks[i])
		{
			delete m_secondaryTasks[i];
		}
		m_secondaryTasks[i] = NULL;
	}
}

CTask* CTaskManager::GetTaskSecondary(int id) const
{
	return m_secondaryTasks[id];
}

CTask* CTaskManager::GetSimplestTask(CTask* task)
{
	CTask* simplest;
	do
	{
		simplest = task;
		task = task->GetSubTask();
	} while (task);
	return simplest;
}

CTask* CTaskManager::GetActiveTask() const
{
	for (size_t i = 0; i < ELEMS_COUNT(m_primaryTasks); i++)
	{
		if (m_primaryTasks[i])
		{
			return m_primaryTasks[i];
		}
	}
	return NULL;
}

CTask* CTaskManager::FindActiveTaskByType(int taskType) const
{//CHECK LOGIC
	CTask* task = GetActiveTask();
	if (task)
	{
		for (CTask* subTask = task; subTask; subTask = subTask->GetSubTask())
		{
			if (subTask->GetTaskType() == taskType)
			{
				return subTask;
			}
		}
	}
	for (size_t i = 0; i < ELEMS_COUNT(m_secondaryTasks); i++)
	{
		for (CTask* subTask = m_secondaryTasks[i]; subTask; subTask = subTask->GetSubTask())
		{
			if (subTask->GetTaskType() == taskType)
			{
				return subTask;
			}
		}
	}
	return NULL;
}

CTask* CTaskManager::FindTaskByType(int id, int type) const
{
	for (CTask* task = m_primaryTasks[id]; task; task = task->GetSubTask())
	{
		if (task->GetTaskType() == type)
		{
			return task;
		}
	}
	return NULL;
}

void CTaskManager::ClearTaskEventResponse()
{
	if (m_primaryTasks[1])
	{
		SetTask(NULL, 1, 0);
	}
	else if (m_primaryTasks[1])
	{
		SetTask(NULL, 2, m_primaryTasks[1]);
	}
}

void CTaskManager::SetTask(CTask* task, int id, bool)
{
	if (!task)
	{
		if (m_primaryTasks[id])
		{
			delete m_primaryTasks[id];
			m_primaryTasks[id] = NULL;
		}
		return;
	}
	if (m_primaryTasks[id] != task)
	{
		if (m_primaryTasks[id])
		{
			delete m_primaryTasks[id];
		}
		m_primaryTasks[id] = task;
		AddSubTasks(task);
		if (m_primaryTasks[id])
		{
			CTask* simplestTask = GetSimplestTask(m_primaryTasks[id]);
			if (!simplestTask->IsSimpleTask())
			{
				if (m_primaryTasks[id])
				{
					delete m_primaryTasks[id];
				}
				m_primaryTasks[id] = NULL;
			}
		}
	}
}

void CTaskManager::AddSubTasks(CTask* task)
{
  CTaskManager *v2; // ebx@1
  int v4; // eax@3
  CTask *v5; // edi@3

  CTask* v3 = task;
  v2 = this;
  if ( task )
  {
    do
    {
		if (v3->IsSimpleTask())
		{
			break;
		}
      v4 = (*(int (__thiscall **)(_DWORD, _DWORD))(v3->__vmt + 0x24))(v3, v2->pPed);
      v5 = (CTask *)v4;
      if ( v4 )
        (*(void (__thiscall **)(_DWORD, _DWORD))(v3->__vmt + 0x1C))(v3, v4);
      else
        SetNextSubTask(0, v3->pParentTask);
      v3 = v5;
    }
    while ( v5 );
  }
}