#include "StdInc.h"

CTimeInfo* CTimeModelInfo::GetTimeInfo()
{
	return &m_timeInfo;
}

size_t CTimeModelInfo::GetModelType()
{
	return 3;
}