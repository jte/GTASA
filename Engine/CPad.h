#pragma once

class CPad
{
public:
	/*
	* AimWeaponLeftRight(CPed*, bool*)
	* AimWeaponUpDown(CPed*, bool*)
	* CarGunJustDown(bool)
	* CheckPointSave()
	* CinemaCamera()
	* Clear(bool, bool)
	* ClearKeyBoardHistory()
	* ClearMouseHistory()
	* CollectPickupJustDown()
	* ConversationNoJustDown()
	* ConversationYesJustDown()
	* CraneReleaseJustDown()
	* CycleCameraModeDownJustDown(bool)
	* CycleWeaponLeftJustDown()
	* CycleWeaponRightJustDown()
	* DiveJustDown()
	* DropItemJustDown()
	* DuckJustDown(CPed*)
	* EditCodesForControls(int*, int)
	* EditString(char*, int)
	* ExitVehicleJustDown(bool, CVehicle*, bool, CVector const&)
	* FixPadsAfterSave()
	* GetAbortClimb()
	* GetAccelerate()
	* GetAccelerateJustDown()
	* GetAutoClimb()
	* GetBrake()
	*/ 
	int16_t GetCarGunFired(bool, bool);
	/* GetCarGunLeftRight(bool, bool)
	* GetCarGunUpDown(bool, CAutomobile*, float, bool)
	* GetDisplayVitalStats(CPed*)
	* GetDuck()
	* GetEnterTargeting()
	* GetEscapeJustDown()
	* GetExitTargeting()
	* GetExitVehicle()
	* GetForceCameraBehindPlayer()
	* GetGroupControlBack(bool, bool)
	* GetGroupControlForward(bool, bool)
	*/ 
	int16_t GetHandBrake();
	bool GetHorn(bool);
	bool GetHydraulicJump();
	bool GetJump(bool);
	/* GetLeftAnalogue(CVector2D*)
	*/ 
	bool GetLookBehindForCar();
	bool GetLookBehindForPed();
	bool GetLookLeft(bool);
	bool GetLookRight(bool);
	/* GetMeleeAttack(bool)
	* GetNitroFired()
	*/ 
	static CPad* GetPad(int num);
	int16_t GetPedWalkLeftRight();
	int16_t GetPedWalkUpDown();
	/* GetRightAnalogue(CVector2D*)
	* GetSprint(int)
	*/ 
	int16_t GetSteeringLeftRight();
	/* GetSteeringMode()
	*/ 
	int16_t GetSteeringUpDown();
	/* GetTarget(bool)
	* GetTurretLeft()
	* GetTurretRight()
	* GetWeapon(CPed*, bool)
	* GroupControlBackJustDown(bool)
	* GroupControlForwardJustDown(bool)
	* HornJustDown()
	* Initialise()
	* IsFlyingRCVehicle()
	* JumpJustDown()
	* LastStationJustUp()
	* LookAroundLeftRight()
	* LookAroundUpDown()
	* MeleeAttackJustDown(bool)
	* NextStationJustUp()
	* ProcessPCSpecificStuff()
	* ProcessStoppie()
	* ProcessWheelie(float)
	* ReconcileTwoControllersInput(CControllerState const&, CControllerState const&)
	*/ 
	void SetDrunkInputDelay(int drunkInputDelay);
	/* ShiftTargetLeftJustDown()
	* ShiftTargetRightJustDown()
	* SniperZoomIn()
	* SniperZoomOut()
	* SprintJustDown()
	* StartShake(short, unsigned char, unsigned int)
	* StartShake_Distance(short, unsigned char, float, float, float)
	* StartShake_Train(float, float)
	* StopPadsShaking()
	* StopShaking(short)
	* SwimJumpJustDown()
	* Update(short)
	* UpdateMouse()
	* UpdatePads()
	* UpdatePadsTillStable()
	* UseBomb()
	* UseRocket()
	* WeaponJustDown(CPed*, bool, bool)
	* WeaponUsesAttackButton(CPed*)
	* WeaponUsesTargetingButton()
	* WillBulletHitSomething(CPed*)
	*/
private:
	CControllerState NewState;
	CControllerState OldState;
	int16_t SteeringLeftRightBuffer[10];
	uint32_t DrunkInputDelay;
	CControllerState PCTempKeyState;
	CControllerState PCTempJoyState;
	CControllerState PCTempMouseState;
	uint8_t Phase;
	uint8_t real_pad;
	uint16_t Mode;
	uint16_t ShakeDur;
	uint16_t DisablePlayerControls;
	uint8_t ShakeFreq;
	bool bHornHistory[5];
	uint8_t iCurrHornHistory;
	uint8_t JustOutOfFrontEnd;
	bool bApplyBrakes;
	bool bDisablePlayerEnterCar;
	bool bDisablePlayerDuck;
	bool bDisablePlayerFireWeapon;
	bool bDisablePlayerFireWeaponWithL1;
	bool bDisablePlayerCycleWeapon;
	bool bDisablePlayerJump;
	bool bDisablePlayerDisplayVitalStats;
	uint32_t LastTimeTouched;
	uint32_t AverageWeapon;
	uint32_t AverageEntries;
	uint32_t NoShakeBeforeThis;
	uint8_t NoShakeFreq;
	uint8_t real_pad2;
	uint16_t real_pad3;
private:
	// Static members
	/* NewMouseControllerState
	* OldMouseControllerState
	* PCTempMouseControllerState
	* bInvertLook4Pad
	* bSniperAimWithRightStick
	* bSniperAllowMovement
	* m_bDebugCamPCOn
	* m_bMapPadOneToPadTwo
	* m_fLastCarCheatScroll
	* m_fLastCheatScroll
	* m_fLastMissionSkipScroll
	*/
};