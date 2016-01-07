#include "StdInc.h"

void* CTask::operator new(size_t)
{
    return CPools::GetTaskPool()->New();
}

void CTask::operator delete(void* p)
{
    CPools::GetTaskPool()->Delete((CTask*)p);
}

CTask::CTask()
{
    m_parent = NULL;
}