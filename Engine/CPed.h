#pragma once

enum ePedState
{
    PED_NONE = 0,
    PED_IDLE,
    PED_LOOK_ENTITY,
    PED_LOOK_HEADING,
    PED_WANDER_RANGE,
    PED_WANDER_PATH,
    PED_SEEK_POSITION,
    PED_SEEK_ENTITY,
    PED_FLEE_POSITION,
    PED_FLEE_ENTITY,
    PED_PURSUE,
    PED_FOLLOW_PATH,
    PED_SNIPER_MODE,
    PED_ROCKETLAUNCHER_MODE,
    PED_DUMMY,
    PED_PAUSE,
    PED_ATTACK,
    PED_FIGHT,      // new close combat fist fight mode
    PED_FACE_PHONE,
    PED_MAKE_PHONECALL,
    PED_CHAT,
    PED_MUG,
    PED_AIMGUN,
    PED_AI_CONTROL,
    PED_SEEK_CAR, //24
    PED_SEEK_BOAT_POSITION,
    PED_FOLLOW_ROUTE,
    PED_CPR,
    PED_SOLICIT,
    PED_BUY_ICE_CREAM,
    PED_INVESTIGATE_EVENT,
    PED_EVADE_STEP,
    PED_ON_FIRE,
    PED_SUNBATHE,
    PED_FLASH,
    PED_JOG,
    PED_ANSWER_MOBILE,  
    PED_HANG_OUT,
    PED_STATES_NO_AI, // only put states after here that do not require AI, put other ones before this  
    PED_ABSEIL_FROM_HELI,
    PED_SIT,
    PED_JUMP,
    PED_FALL,
    PED_GETUP,
    PED_STAGGER,
    PED_EVADE_DIVE,
    PED_STATES_CAN_SHOOT, // only put states before here than ped can be shot during
    PED_ENTER_TRAIN, 
    PED_EXIT_TRAIN,
    PED_ARREST_PLAYER,
    PED_DRIVING, // 50
    PED_PASSENGER,
    PED_TAXI_PASSENGER,
    PED_OPEN_DOOR,
    PED_DIE,
    PED_DEAD,
    PED_CARJACK,
    PED_DRAGGED_FROM_CAR,
    PED_ENTER_CAR,
    PED_STEAL_CAR,
    PED_EXIT_CAR,
    PED_HANDS_UP,
    PED_ARRESTED,
    PED_DEPLOY_STINGER,
        
    PED_NUM_STATES
};


class CPed : public CPhysical
{
public:
    //virtual ~CPed();
    virtual void Add(const CRect&);//inherited from CEntity
    virtual void Add(); // inherited from CPhysical
    virtual void Remove(); // inherited from CPhysical
    virtual void SetIsStatic(); // inherited from CEntity
    //virtual void SetModelIndex(uint32_t modelIndex);
    virtual void SetModelIndexNoCreate(uint32_t modelIndex); // inherited from CEntity
    virtual void CreateRwObject(); // inherited from CEntity
    //virtual void DeleteRwObject();
    virtual CRect GetBoundRect(); // inherited from CPhysical
   // virtual void ProcessControl();
    /*
.rdata:0086C384                     dd offset _ZN9CPhysical16ProcessCollisionEv ; CPhysical::ProcessCollision(void)
.rdata:0086C388                     dd offset _ZN9CPhysical12ProcessShiftEv ; CPhysical::ProcessShift(void)
.rdata:0086C38C                     dd offset _ZN9CPhysical13TestCollisionEb ; CPhysical::TestCollision(bool)
.rdata:0086C390                     dd offset _ZN4CPed8TeleportE7CVectorh ; CPed::Teleport(CVector,uchar)
.rdata:0086C394                     dd offset _ZN4CPed30SpecialEntityPreCollisionStuffEP9CPhysicalbRbS2_S2_S2_ ; CPed::SpecialEntityPreCollisionStuff(CPhysical *,bool,bool &,bool &,bool &,bool &)
.rdata:0086C398                     dd offset _ZN4CPed31SpecialEntityCalcCollisionStepsERbS0_ ; CPed::SpecialEntityCalcCollisionSteps(bool &,bool &)
.rdata:0086C39C                     dd offset _ZN4CPed9PreRenderEv ; CPed::PreRender(void)
.rdata:0086C3A0                     dd offset _ZN4CPed6RenderEv ; CPed::Render(void)
.rdata:0086C3A4                     dd offset _ZN4CPed13SetupLightingEv ; CPed::SetupLighting(void)
.rdata:0086C3A8                     dd offset _ZN4CPed14RemoveLightingEb ; CPed::RemoveLighting(bool)
.rdata:0086C3AC                     dd offset _ZN4CPed30FlagToDestroyWhenNextProcessedEv ; CPed::FlagToDestroyWhenNextProcessed(void)
.rdata:0086C3B0                     dd offset _ZN4CPed22ProcessEntityCollisionEP7CEntityP9CColPoint ; CPed::ProcessEntityCollision(CEntity *,CColPoint *)
.rdata:0086C3B4                     dd offset _ZN4CPed11SetMoveAnimEv ; CPed::SetMoveAnim(void)
.rdata:0086C3B8                     dd offset _ZN4CPed4SaveEv ; CPed::Save(void)
.rdata:0086C3BC                     dd offset _ZN4CPed4LoadEv ; CPed::Load(void)
*/
	/* AddGogglesModel(int, bool*)
	* AddWeaponModel(int)
	* AttachPedToBike(CEntity*, CVector, unsigned short, float, float, eWeaponType)
	* AttachPedToEntity(CEntity*, CVector, unsigned short, float, eWeaponType)
	* CPed(unsigned int)
	* CalculateNewOrientation()
	* CalculateNewVelocity()
	* CanBeArrested()
	* CanBeDeleted()
	* CanBeDeletedEvenInVehicle()
	* CanPedHoldConversation()
	* CanSeeEntity(CEntity*, float)
	* CanSetPedState()
	* CanStrafeOrMouseControl()
	* CanThrowEntityThatThisPedIsHolding()
	* CanUseTorsoWhenLooking()
	* CanWeRunAndFireWithWeapon()
	* ClearAimFlag()
	* ClearAll()
	* ClearLook()
	* ClearLookFlag()
	* ClearWeapon(eWeaponType)
	* ClearWeapons()
	* CreateDeadPedMoney()
	* CreateDeadPedPickupCoors(float*, float*, float*)
	* CreateDeadPedWeaponPickups()
	* DeadPedMakesTyresBloody()
	* DeleteRwObject()
	* DettachPedFromEntity()
	* DisablePedSpeech(short)
	* DisablePedSpeechForScriptSpeech(short)
	* DoFootLanded(bool, unsigned char)
	* DoGunFlash(int, bool)
	* DoWeHaveWeaponAvailable(eWeaponType)
	* DoesLOSBulletHitPed(CColPoint&)
	* Dress()
	* DropEntityThatThisPedIsHolding(unsigned char)
	* EnablePedSpeech()
	* EnablePedSpeechForScriptSpeech()
	* FinishJumpCB(CAnimBlendAssociation*, void*)
	* FlagToDestroyWhenNextProcessed()
	* GetBikeRidingSkill()
	* GetBonePosition(RwV3d&, unsigned int, bool)
	* GetEntityThatThisPedIsHolding()
	* GetHeadAndFootPositions(CVector&, CVector&, bool)
	* GetHoldingTask()
	* GetLocalDirection(CVector2D const&)
	* GetPedStateString()
	* GetPedTalking()
	* GetTransformedBonePosition(RwV3d&, unsigned int, bool)
	* GetWalkAnimSpeed()
	* GetWeaponSkill()
	* GetWeaponSkill(eWeaponType)
	* GetWeaponSlot(eWeaponType)
	* GiveDelayedWeapon(eWeaponType, unsigned int)
	* GiveObjectToPedToHold(int, unsigned char)
	* GiveWeapon(eWeaponType, unsigned int, bool)
	* GiveWeaponAtStartOfFight()
	* GiveWeaponWhenJoiningGang()
	* GrantAmmo(eWeaponType, unsigned int)
	void Initialise();
	* IsAlive() const
	* IsPedHeadAbovePos(float)
	* IsPedInControl()
	* IsPedShootable()
	* IsPlayer() const
	* IsPlayingHandSignal()
	* IsPointerValid()
	* KillPedWithCar(CVehicle*, float, bool)
	* Load()
	* Look()
	* MakeTyresMuddySectorList(CPtrList&)
	* OurPedCanSeeThisEntity(CEntity*, bool)
	* PedCanPickUpPickUp()
	* PedIsInvolvedInConversation()
	* PedIsReadyForConversation(bool)
	* PlayFootSteps()
	* PositionAnyPedOutOfCollision()
	* PositionAttachedPed()
	* PositionPedOutOfCollision(int, CVehicle*, bool)
	* PreRender()
	* PreRenderAfterTest()
	* ProcessBuoyancy()
	* ProcessControl()
	* ProcessEntityCollision(CEntity*, CColPoint*)
	* PutOnGoggles()
	* ReleaseCoverPoint()
	* RemoveBodyPart(int, signed char)
	* RemoveGogglesModel()
	* RemoveLighting(bool)
	* RemoveWeaponAnims(int, float)
	* RemoveWeaponForScriptedCutscene()
	* RemoveWeaponModel(int)
	* RemoveWeaponWhenEnteringVehicle(int)
	* Render()
	* ReplaceWeaponForScriptedCutscene()
	* ReplaceWeaponWhenExitingVehicle()
	* RequestDelayedWeapon()
	* ResetGunFlashAlpha()
	* RestartNonPartialAnims()
	* RestoreHeadingRate()
	* RestoreHeadingRateCB(CAnimBlendAssociation*, void*)
	* Save()
	* Say(unsigned short, unsigned int, float, unsigned char, unsigned char, unsigned char)
	* SayScript(int, unsigned char, unsigned char, unsigned char)
	* SetAimFlag(CEntity*)
	* SetAimFlag(float)
	* SetAmmo(eWeaponType, unsigned int)
	* SetCharCreatedBy(unsigned char)
	* SetCurrentWeapon(eWeaponType)
	* SetCurrentWeapon(int)
	* SetGunFlashAlpha(bool)
	* SetIdle()
	* SetLook(CEntity*)
	* SetLook(float)
	* SetLookFlag(CEntity*, bool, bool)
	* SetLookFlag(float, bool, bool)
	* SetLookTimer(unsigned int)
	* SetModelIndex(unsigned int)
	* SetMoveAnim()
	* SetMoveAnimSpeed(CAnimBlendAssociation*)
	void SetMoveState(eMoveState moveState);
	* SetPedDefaultDecisionMaker()
	* SetPedPositionInCar()
	* SetPedState(ePedState)
	* SetPedStats(ePedStats)
	* SetRadioStation()
	* SetWeaponLockOnTarget(CEntity*)
	* SetWeaponSkill(eWeaponType, signed char)
	* SetupLighting()
	* ShoulderBoneRotation(RpClump*)
	* SortPeds(CPed**, int, int)
	* SpawnFlyingComponent(int, signed char)
	* SpecialEntityCalcCollisionSteps(bool&, bool&)
	* SpecialEntityPreCollisionStuff(CPhysical*, bool, bool&, bool&, bool&, bool&)
	* StopNonPartialAnims()
	* StopPlayingHandSignal()
	* TakeOffGoggles()
	* Teleport(CVector, unsigned char)
	* TurnBody()
	* Undress(char*)
	* Update()
	* UpdatePosition()
	* UpdateStatEnteringVehicle()
	* UpdateStatLeavingVehicle()
	* UseGroundColModel()
	* WorkOutHeadingForMovingFirstPerson(float)
	* operator delete(void*)
	* operator delete(void*, int)
	* operator new(unsigned int)
	* operator new(unsigned int, int)
	* ~CPed()
    */
protected:
private:
    	/* m_sGunFlashBlendOutRate
	* m_sGunFlashBlendStart
	* nCloseBlockedLosWaitTime
	* nFarBlockedLosWaitTime
    */
};