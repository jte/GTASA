#pragma once

#include "CTask.h"

class CEvent;
class CPed;

class CTaskComplex : public CTask
{
public:
	virtual ~CTaskComplex();
	virtual CTask* Clone() = 0;
	virtual CTask* GetSubTask() const;
	virtual bool IsSimpleTask() const;
	virtual eTaskType GetTaskType() const = 0;
	virtual void StopTimer(CEvent* event); // inherited from CTask
	virtual void MakeAbortable(CPed* ped, int priority, const CEvent* event);
	virtual void SetSubTask(CTask* subTask);
	virtual CTask* CreateNextSubTask(CPed* ped) = 0;
	virtual CTask* CreateFirstSubTask(CPed* ped) = 0;
	virtual void ControlSubTask(CPed* ped) = 0;

	.data.rel.ro:005786D8                 DCD _ZN30CTaskComplexPlayHandSignalAnimD2Ev+1
.data.rel.ro:005786DC                 DCD _ZN30CTaskComplexPlayHandSignalAnimD0Ev+1
.data.rel.ro:005786E0                 DCD _ZNK30CTaskComplexPlayHandSignalAnim5CloneEv+1
.data.rel.ro:005786E4                 DCD _ZNK12CTaskComplex10GetSubTaskEv+1
.data.rel.ro:005786E8                 DCD _ZNK12CTaskComplex12IsSimpleTaskEv+1
.data.rel.ro:005786EC                 DCD _ZNK30CTaskComplexPlayHandSignalAnim11GetTaskTypeEv+1
.data.rel.ro:005786F0                 DCD _ZN5CTask9StopTimerEPK6CEvent+1
.data.rel.ro:005786F4                 DCD _ZN12CTaskComplex13MakeAbortableEP4CPediPK6CEvent+1
.data.rel.ro:005786F8                 DCD _ZN30CTaskComplexPlayHandSignalAnim9SerializeEv+1
.data.rel.ro:005786FC                 DCD _ZN12CTaskComplex10SetSubTaskEP5CTask+1
.data.rel.ro:00578700                 DCD _ZN30CTaskComplexPlayHandSignalAnim17CreateNextSubTaskEP4CPed+1
.data.rel.ro:00578704                 DCD _ZN30CTaskComplexPlayHandSignalAnim18CreateFirstSubTaskEP4CPed+1
.data.rel.ro:00578708                 DCD _ZN30CTaskComplexPlayHandSignalAnim14ControlSubTaskEP4CPed+1

.rdata:0086D4D0                     dd offset __purecall
.rdata:0086D4D4                     dd offset CTaskComplex__GetSubTask ; return _f8; // u32
.rdata:0086D4D8                     dd offset CTaskComplex__IsSimpleTask ; return false;
.rdata:0086D4DC                     dd offset __purecall
.rdata:0086D4E0                     dd offset CTask__StopTimer ; ;
.rdata:0086D4E4                     dd offset CTaskComplex__MakeAbortable ; return _f8->_m18 (...);
.rdata:0086D4E8                     dd offset CTaskComplex__SetSubTask
.rdata:0086D4EC                     dd offset __purecall
.rdata:0086D4F0                     dd offset __purecall
.rdata:0086D4F4                     dd offset __purecall
private:
};