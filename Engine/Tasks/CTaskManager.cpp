#include "CTaskManager.h"

CTaskManager::CTaskManager(CPed *ped)
{
    m_ped = ped;
    memset(m_primary_tasks, 0, sizeof(m_primary_tasks));
    memset(m_secondary_tasks, 0, sizeof(m_secondary_tasks));
}

CTaskManager::~CTaskManager()
{
    for(size_t i = 0; i < sizeof(m_primary_tasks); i++)
    {
        delete m_primary_tasks[i];
        m_primary_tasks[i] = NULL;
    }
    for(size_t i = 0; i < sizeof(m_secondary_tasks); i++)
    {
        delete m_secondary_tasks[i];
        m_secondary_tasks[i] = NULL;
    }
}

CTask *CTaskManager::GetActiveTask() const
{
    for(size_t i = 0; i < sizeof(m_primary_tasks); i++)
    {
        if(m_primary_tasks[i])
        {
            return m_primary_tasks[i];
        }
    }
    return NULL;
}

CTask *CTaskManager::FindActiveTaskByType(int type) const
{
}

