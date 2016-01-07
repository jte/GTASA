#pragma once

class CPedIntelligence
{
public:
	/*
		* AddEvent(CEvent&, bool)
	* AddInterestingEntity(CEntity*)
	* AddTaskDefault(CTask*, bool)
	* AddTaskEventResponseNonTemp(CTask*, bool)
	* AddTaskEventResponseTemp(CTask*, bool)
	* AddTaskPhysResponse(CTask*, bool)
	* AddTaskPrimary(CTask*, bool)
	* AddTaskPrimaryMaybeInGroup(CTask*, bool)
	* AddTaskSecondaryPartialAnim(CTask*)
	* AreFriends(CPed const&, CPed const&)
	* CPedIntelligence(CPed*)
	* CanSeeEntityWithLights(CEntity const*, bool) const
	* ClearTaskDuckSecondary()
	* ClearTasks(bool, bool)
	* FindRespectedFriendInInformRange()
	* FindTaskByType(int) const
	* FlushImmediately(bool)
	* FlushIntelligence()
	* GetEffectInUse() const
	* GetMoveStateFromGoToTask() const
	* GetTaskClimb() const
	* GetTaskDuck(bool) const
	* GetTaskFighting() const
	* GetTaskHold(bool) const
	* GetTaskInAir() const
	* GetTaskJetPack() const
	* GetTaskSwim() const
	* GetTaskThrow() const
	* GetTaskUseGun() const
	* GetUsingParachute() const
	* HasInterestingEntites()
	* IsFriendlyWith(CPed const&) const
	* IsInACarOrEnteringOne()
	* IsInHearingRange(CVector const&) const
	* IsInSeeingRange(CEntity*) const
	* IsInSeeingRange(CVector const&) const
	* IsInterestingEntity(CEntity*)
	* IsPedGoingForCarDoor()
	* IsPedGoingSomewhereOnFoot() const
	* IsRespondingToEvent(int) const
	* IsThreatenedBy(CPed const&) const
	* Load()
	* LookAtInterestingEntities()
	* Process()
	* ProcessAfterPreRender()
	* ProcessAfterProcCol()
	* ProcessEventHandler()
	* ProcessFirst()
	* ProcessStaticCounter()
	* RecordEventForScript(int, int)
	* RemoveAllInterestingEntities()
	* RemoveInterestingEntity(CEntity*)
	* Respects(CPed const&) const
	* RestorePedDecisionMakerType()
	* Save()
	* SetEffectInUse(C2dEffect*)
	* SetHearingRange(float)
	* SetPedDecisionMakerType(int)
	* SetPedDecisionMakerTypeInGroup(int)
	* SetSeeingRange(float)
	* SetTaskDuckSecondary(unsigned short)
	* SortAndStaggerAllPedsEventScannerTimers()
	* TestForStealthKill(CPed*, bool)
	* operator delete(void*)
	* operator delete(void*, int)
	* operator new(unsigned int)
	* operator new(unsigned int, int)
	* ~CPedIntelligence()
	*/
private:
	// static members
	/*
		* ms_fFollowNodeThresholdDistance
	* ms_fSenseRange
	* ms_fSenseRangeOfGangPeds
	* ms_fSenseRangeOfMissionPeds
	* ms_iDesiredMoveState
	* ms_iNumCollisionFramesTolerance
	* ms_iStaticCountColPosToleranceSqr
	* ms_iStaticCounterStuckCount
	*/
};