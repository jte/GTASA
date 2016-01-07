#pragma once

class CTimeModelInfo : public CAtomicModelInfo
{
public:
	virtual ~CTimeModelInfo();
	virtual size_t GetModelType();
	virtual CTimeInfo* GetTimeInfo();
private:
	CTimeInfo m_timeInfo;
};