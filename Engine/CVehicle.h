#pragma once

#include "Physics\CPhysical.h"

enum eCarType
{
};

class CVehicle : public CPhysical
{
public:
	// virtual methods
	// virtual ~CVehicle();
	// virtual void SetModelIndex(uint32_t modelIndex);
	virtual void DeleteRwObject();
	// virtual void SpecialEntityCalcCollisionSteps(bool&, bool&);
	// virtual void SpecialEntityPreCollisionStuff(class CPhysical*, bool, bool&, bool&, bool&, bool&);
	virtual void PreRender();
    virtual void Render();
	/*
.rdata:00871ECC                     dd offset _ZN8CVehicle13SetupLightingEv ; CVehicle::SetupLighting(void)
.rdata:00871ED0                     dd offset _ZN8CVehicle14RemoveLightingEb ; CVehicle::RemoveLighting(bool)
.rdata:00871ED4                     dd offset _ZN8CVehicle30FlagToDestroyWhenNextProcessedEv ; ;

.rdata:00871ED8                     dd offset _ZN9CPhysical22ProcessEntityCollisionEP7CEntityP9CColPoint ; CPhysical::ProcessEntityCollision(CEntity *,CColPoint *)
.rdata:00871EDC                     dd offset _ZN8CVehicle28ProcessControlCollisionCheckEb ; ;
.rdata:00871EE0                     dd offset _ZN8CVehicle20ProcessControlInputsEh ; ;
.rdata:00871EE4                     dd offset _ZN8CVehicle25GetComponentWorldPositionEiR7CVector ; CVehicle::GetComponentWorldPosition(int,CVector &)
.rdata:00871EE8                     dd offset _ZNK8CVehicle18IsComponentPresentEi ; return false;
.rdata:00871EEC                     dd offset _ZN8CVehicle8OpenDoorEP4CPedi6eDoorsfb ; CVehicle::OpenDoor(CPed *,int,eDoors,float,bool)
.rdata:00871EF0                     dd offset _ZN8CVehicle15ProcessOpenDoorEP4CPedjjjf ; CVehicle::ProcessOpenDoor(CPed *,uint,uint,uint,float)
.rdata:00871EF4                     dd offset CVehicle__GetDoorAngleOpenRatio ; return 0.0f;
.rdata:00871EF8                     dd offset CVehicle__GetDoorAngleOpenRatio2 ; return 0.0;
.rdata:00871EFC                     dd offset CVehicle__IsDoorReady ; return false;
.rdata:00871F00                     dd offset CVehicle__IsDoorReady2 ; return false;
.rdata:00871F04                     dd offset CVehicle__IsDoorFullyOpen ; return false;
.rdata:00871F08                     dd offset CVehicle__IsDoorFullyOpen2 ; return false;
.rdata:00871F0C                     dd offset CVehicle__IsDoorClosed ; return false;
.rdata:00871F10                     dd offset CVehicle__IsDoorClosed2 ; return false;
.rdata:00871F14                     dd offset CVehicle__IsDoorMissing ; return false;
.rdata:00871F18                     dd offset CVehicle__IsDoorMissing2 ; return false;
.rdata:00871F1C                     dd offset CVehicle__IsOpenTopCar ; return false;
.rdata:00871F20                     dd offset CVehicle__RemoveRefsToVehicle
.rdata:00871F24                     dd offset CVehicle__blowUp ; ;
.rdata:00871F28                     dd offset CVehicle__BlowUpCarCutSceneNoExtras ; ;
.rdata:00871F2C                     dd offset CVehicle__SetUpWheelColModel ; return false;
.rdata:00871F30                     dd offset CVehicle__deflateTire ; return false;
.rdata:00871F34                     dd offset CVehicle__IsRoomForPedToLeaveCar ; return false;
.rdata:00871F38                     dd offset _ZN8CVehicle19ProcessDrivingAnimsEP4CPedh ; CVehicle::ProcessDrivingAnims(CPed *,uchar)
.rdata:00871F3C                     dd offset _ZN8CVehicle15GetRideAnimDataEv ; return NULL;
.rdata:00871F40                     dd offset CVehicle__SetupSuspensionLines ; ;
.rdata:00871F44                     dd offset CVehicle__AddMovingCollisionSpeed ; _a0->_f0 = _a0->_f4 = _a0->_f8 = 0;
.rdata:00871F48                     dd offset CVehicle__Fix ; ;
.rdata:00871F4C                     dd offset CVehicle__SetupDamageAfterLoad ; ;
.rdata:00871F50                     dd offset CVehicle__DoBurstAndSoftGroundRatios ; ;
.rdata:00871F54                     dd offset CVehicle__GetHeightAboveRoad
.rdata:00871F58                     dd offset CVehicle__PlayCarHorn ; ;
.rdata:00871F5C                     dd offset CVehicle__GetNumContactWheels ; return 4;
.rdata:00871F60                     dd offset CVehicle__VehicleDamage ; ;
.rdata:00871F64                     dd offset _ZNK8CVehicle16CanPedStepOutCarEb ; CVehicle::CanPedStepOutCar(bool)
.rdata:00871F68                     dd offset _ZNK8CVehicle16CanPedJumpOutCarEP4CPed ; CVehicle::CanPedJumpOutCar(CPed *)
.rdata:00871F6C                     dd offset CVehicle__GetTowHitchPos
.rdata:00871F70                     dd offset CVehicle__GetTowbarPos
.rdata:00871F74                     dd offset CVehicle__AttachTrailer ; return false;
.rdata:00871F78                     dd offset CVehicle__DetachTrailer ; return false;
.rdata:00871F7C                     dd offset CVehicle__FindWheelWidth ; return 0.25f;
.rdata:00871F80                     dd offset _ZN8CVehicle4SaveEv ; CVehicle::Save(void)
.rdata:00871F84                     dd offset _ZN8CVehicle4LoadEv ; CVehicle::Load(void)
*/
    /*
	* ActivateBomb()
	* ActivateBombWhenEntered()
	* AddDamagedVehicleParticles()
	* AddExhaustParticles()
	* AddMovingCollisionSpeed(CVector&)
	* AddPassenger(CPed*)
	* AddPassenger(CPed*, unsigned char)
	* AddReplacementUpgrade(int, int)
	* AddSingleWheelParticles(tWheelState, unsigned int, float, float, CColPoint*, CVector*, float, int, unsigned int, bool*, unsigned int)
	* AddUpgrade(int, int)
	* AddVehicleUpgrade(int)
	* AddWaterSplashParticles()
	* AddWheelDirtAndWater(CColPoint&, unsigned int, unsigned char, unsigned char)
	* ApplyBoatWaterResistance(tBoatHandlingData*, float)
	* BladeColSectorList(CPtrList&, CColModel&, CMatrix&, short, float)
	* BlowUpCar(CEntity*, unsigned char)
	* BlowUpCarCutSceneNoExtras(bool, bool, bool, bool)
	* BreakTowLink()
	* BurstTyre(unsigned char, bool)
	* CVehicle(unsigned char)
	* CalculateLightingFromCollision()
	* CanBeDeleted() const
	* CanBeDriven() const
	* CanDoorsBeDamaged() const
	* CanPedEnterCar() const
	* CanPedJumpOutCar(CPed*) const
	* CanPedLeanOut(CPed*) const
	* CanPedOpenLocks(CPed const*) const
	* CanPedStepOutCar(bool) const
	* CanUseCameraHeightSetting()
	* CanVehicleBeDamaged(CEntity*, eWeaponType, unsigned char*)
	* CarHasRoof() const
	* ChangeLawEnforcerState(unsigned char)
	* ClearGettingInFlags(unsigned char)
	* ClearGettingOutFlags(unsigned char)
	* ClearVehicleUpgradeFlags(int, int)
	* ClearWindowOpenFlag(unsigned char)
	* CreateReplacementAtomic(CBaseModelInfo*, RwFrame*, int, bool, bool)
	* CreateUpgradeAtomic(CBaseModelInfo*, UpgradePosnDesc const*, RwFrame*, bool)
	*/ 
	bool CustomCarPlate_TextureCreate(CVehicleModelInfo* modelInfo);
	/* CustomCarPlate_TextureDestroy()
	* DeleteRwObject()
	* DoBladeCollision(CVector, CMatrix&, short, float, float)
	* DoBoatSplashes(float)
	* DoBurstAndSoftGroundRatios()
	* DoDriveByShootings()
	* DoFixedMachineGuns()
	* DoHeadLightBeam(int, CMatrix&, unsigned char)
	* DoHeadLightEffect(int, CMatrix&, unsigned char, unsigned char)
	* DoHeadLightReflection(CMatrix&, unsigned int, unsigned char, unsigned char)
	* DoHeadLightReflectionSingle(CMatrix&, unsigned char)
	* DoHeadLightReflectionTwin(CMatrix&)
	* DoPlaneGunFireFX(CWeapon*, CVector&, CVector&, int)
	* DoReverseLightEffect(int, CMatrix&, unsigned char, unsigned char, unsigned int, unsigned char)
	* DoSunGlare()
	* DoTailLightEffect(int, CMatrix&, unsigned char, unsigned char, unsigned int, unsigned char)
	* DoVehicleLights(CMatrix&, unsigned int)
	* ExtinguishCarFire()
	* FillVehicleWithPeds(bool)
	* FindTyreNearestPoint(float, float)
	* FindWheelWidth(bool)
	* FireFixedMachineGuns()
	* FireHeatSeakingMissile(CEntity*, eOrdnanceType, bool)
	* FirePlaneGuns()
	* FireUnguidedMissile(eOrdnanceType, bool)
	* Fix()
	* FlagToDestroyWhenNextProcessed()
	* FlyingControl(eFlightModel, float, float, float, float)
	* GetComponentWorldPosition(int, CVector&)
	* GetDooorAngleOpenRatio(eDoors) const
	* GetDooorAngleOpenRatio(unsigned int) const
	* GetFiringRateMultiplier()
	* GetGasTankPosition()
	* GetHasDualExhausts()
	* GetHeightAboveRoad() const
	* GetNewSteeringAmt()
	* GetNumContactWheels()
	* GetPlaneGunsAutoAimAngle()
	* GetPlaneGunsPosition(int)
	* GetPlaneGunsRateOfFire()
	* GetPlaneNumGuns()
	* GetPlaneOrdnancePosition(eOrdnanceType)
	* GetPlaneOrdnanceRateOfFire(eOrdnanceType)
	* GetPlaneWeaponFiringStatus(bool&, eOrdnanceType&)
	* GetRemapIndex()
	* GetReplacementUpgrade(int)
	* GetRideAnimData()
	* GetRopeHeightForHeli()
	* GetSpareHasslePosId()
	* GetSpecialColModel()
	* GetTowBarPos(CVector&, bool, CVehicle*)
	* GetTowHitchPos(CVector&, bool, CVehicle*)
	* GetUpgrade(int)
	* GetVehicleAppearance() const
	* GetVehicleLightsStatus()
	* HeightAboveCeiling(float, eFlightModel)
	* InflictDamage(CEntity*, eWeaponType, float, CVector)
	* InitWinch(int)
	* IsComponentPresent(int) const
	* IsDoorClosed(eDoors) const
	* IsDoorClosed(unsigned int) const
	* IsDoorFullyOpen(eDoors) const
	* IsDoorFullyOpen(unsigned int) const
	* IsDoorMissing(eDoors) const
	* IsDoorMissing(unsigned int) const
	* IsDoorReady(eDoors) const
	* IsDoorReady(unsigned int) const
	* IsDriver(CPed const*) const
	* IsDriver(int) const
	* IsLawEnforcementVehicle() const
	* IsOnItsSide() const
	* IsOpenTopCar() const
	* IsPassenger(CPed const*) const
	* IsPassenger(int) const
	* IsRoomForPedToLeaveCar(unsigned int, CVector*)
	* IsSphereTouchingVehicle(float, float, float, float)
	* IsUpsideDown() const
	* IsVehicleNormal() const
	* KillPedsGettingInVehicle()
	* KillPedsInVehicle()
	* Load()
	* MakeDirty(CColPoint&)
	* OpenDoor(CPed*, int, eDoors, float, bool)
	* PickRandomPassenger()
	* PickUpEntityWithWinch(CEntity*)
	* PlayCarHorn()
	* PossiblyDropFreeFallBombForPlayer(eOrdnanceType, bool)
	* PreRender()
	* PreRenderDriverAndPassengers()
	* ProcessBikeWheel(CVector&, CVector&, CVector&, CVector&, int, float, float, float, float, char, float*, tWheelState*, eBikeWheelSpecial, unsigned short)
	* ProcessBoatControl(tBoatHandlingData*, float&, bool, bool)
	* ProcessCarAlarm()
	* ProcessControlCollisionCheck(bool)
	* ProcessControlInputs(unsigned char)
	* ProcessDelayedExplosion()
	* ProcessDrivingAnims(CPed*, unsigned char)
	* ProcessOpenDoor(CPed*, unsigned int, unsigned int, unsigned int, float)
	* ProcessSirenAndHorn(bool)
	* ProcessWeapons()
	* ProcessWheel(CVector&, CVector&, CVector&, CVector&, int, float, float, float, char, float*, tWheelState*, unsigned short)
	* ProcessWheelRotation(tWheelState, CVector const&, CVector const&, float)
	* QueryPickedUpEntityWithWinch()
	* ReactToVehicleDamage(CPed*)
	* ReduceVehicleDamage(float&)
	* ReleasePickedUpEntityWithWinch()
	* RemoveAllUpgrades()
	* RemoveDriver(bool)
	* RemoveLighting(bool)
	* RemovePassenger(CPed*)
	* RemoveRefsToVehicle(CEntity*)
	* RemoveReplacementUpgrade(int)
	* RemoveUpgrade(int)
	* RemoveVehicleUpgrade(int)
	* RemoveWinch()
	* Render()
	* RenderDriverAndPassengers()
	*/ 
	void ResetAfterRender();
	/* Save()
	* ScanAndMarkTargetForHeatSeekingMissile(CEntity*)
	* SelectPlaneWeapon(bool, eOrdnanceType)
	* SetCollisionLighting(unsigned char)
	* SetComponentAtomicAlpha(RpAtomic*, int)
	* SetComponentRotation(RwFrame*, int, float, bool)
	* SetComponentVisibility(RwFrame*, unsigned int)
	* SetDriver(CPed*)
	* SetFiringRateMultiplier(float)
	* SetGettingInFlags(unsigned char)
	* SetGettingOutFlags(unsigned char)
	* SetHasslePosId(int, bool)
	* SetModelIndex(unsigned int)
	* SetRemap(int)
	* SetRemapTexDictionary(char const*)
	* SetRopeHeightForHeli(float)
	* SetTappedGasTankVehicle(CEntity*)
	* SetTowLink(CVehicle*, bool)
	* SetTransmissionRotation(RwFrame*, float, float, CVector, bool)
	* SetUpDriver(int, bool, bool)
	* SetUpWheelColModel(CColModel*)
	* SetVehicleCreatedBy(int, bool)
	* SetVehicleUpgradeFlags(int, int, int&)
	* SetWindowOpenFlag(unsigned char)
	* SetupDamageAfterLoad()
	* SetupLighting()
	* SetupPassenger(int, int, bool, bool)
	* SetupRender()
	* SetupSuspensionLines()
	* SetupUpgradesAfterLoad()
	* ShufflePassengersToMakeSpace()
	* Shutdown()
	* SpecialEntityCalcCollisionSteps(bool&, bool&)
	* SpecialEntityPreCollisionStuff(CPhysical*, bool, bool&, bool&, bool&, bool&)
	* UpdateClumpAlpha()
	* UpdateLightingFromStoredPolys()
	* UpdatePassengerList()
	* UpdateTractorLink(bool, bool)
	* UpdateTrailerLink(bool, bool)
	* UpdateWinch()
	* UsesSiren() const
	* VehicleDamage(float, unsigned short, CEntity*, CVector*, CVector*, eWeaponType)
	* operator delete(void*)
	* operator delete(void*, int)
	* operator new(unsigned int)
	* operator new(unsigned int, int)
	* ~CVehicle()
    */
protected:
private:
	// static members
	/*
	* WHEELSPIN_FALL_RATE
	* WHEELSPIN_INAIR_TARGET_RATE
	* WHEELSPIN_RISE_RATE
	* WHEELSPIN_TARGET_RATE
	* bDisableRemoteDetonation
	* bDisableRemoteDetonationOnContact
	* m_aSpecialColModel
	* m_aSpecialColVehicle
	* m_aSpecialHydraulicData
	* m_bEnableMouseFlying
	* m_bEnableMouseSteering
	* m_fAirResistanceMult
	* m_nLastControlInput
	* m_pTappedGasTankVehicle
	* ms_fRailTrackResistance
	* ms_fRailTrackResistanceDefault
	* ms_forceVehicleLightsOff
	* s_bPlaneGunsEjectShellCasings
	*/
};
