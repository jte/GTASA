#pragma once

#include "../Entity/CEntity.h"
#include "../CColPoint.h"

class CRealTimeShadow;

class CPhysical : public CEntity
{
public:
    /*
    * Add()
	* AddCollisionRecord(CEntity*)
	* AddToMovingList()
	* ApplyAirResistance()
	*/ 
    bool ApplyCollision(CEntity*, CColPoint&, float&);
	/* ApplyCollision(CPhysical*, CColPoint&, float&, float&)
	* ApplyCollisionAlt(CEntity*, CColPoint&, float&, CVector&, CVector&)
	*/ 
    void ApplyForce(CVector force, CVector point, bool applyTurnForce);
	/* ApplyFriction()
	* ApplyFriction(CPhysical*, float, CColPoint&)
	*/ 
	bool ApplyFriction(float, CColPoint& point);
	/* ApplyFrictionForce(CVector, CVector)
	*/ 
    void ApplyFrictionMoveForce(CVector force);
	void ApplyFrictionTurnForce(CVector force, CVector point);
    void ApplyGravity();
    void ApplyMoveForce(CVector force);
	void ApplyMoveSpeed();
	/* ApplyScriptCollision(CVector, float, float, CVector*)
	* ApplySoftCollision(CEntity*, CColPoint&, float&)
	* ApplySoftCollision(CPhysical*, CColPoint&, float&, float&)
	* ApplySpeed()
	*/ 
    void ApplySpringCollision(float, CVector&, CVector&, float, float, float&);
	void ApplySpringCollisionAlt(float, CVector&, CVector&, float, float, CVector&, float&);
	void ApplySpringDampening(float, float, CVector&, CVector&, CVector&);
	/* ApplySpringDampeningOld(float, float, CVector&, CVector&, CVector&)
	*/ 
    void ApplyTurnForce(CVector force, CVector point);
	void ApplyTurnSpeed();
	/* AttachEntityToEntity(CEntity*, CVector*, RtQuat*)
	* AttachEntityToEntity(CEntity*, CVector, CVector)
	* CPhysical()
	* CanPhysicalBeDamaged(eWeaponType, unsigned char*)
	* CheckCollision()
	* CheckCollision_SimpleCar()
	* DettachAutoAttachedEntity()
	* DettachEntityFromEntity(float, float, float, bool)
	* GetBoundRect() const
	*/ 
    bool GetHasCollidedWith(CEntity* entity);
	bool GetHasCollidedWithAnyObject();
	/* GetLightingFromCol(bool) const
	* GetLightingTotal() const
	*/ 
    CVector GetSpeed(CVector point);
	/* PlacePhysicalRelativeToOtherPhysical(CPhysical*, CPhysical*, CVector)
	* PositionAttachedEntity()
	* ProcessCollision()
	* ProcessCollisionSectorList(int, int)
	* ProcessCollisionSectorList_SimpleCar(CRepeatSector*)
	*/ 
    void ProcessControl();
	bool ProcessEntityCollision(CEntity* otherEntity, CColPoint* colPoint);
	/* ProcessShift()
	* ProcessShiftSectorList(int, int)
	* Remove()
	* RemoveAndAdd()
	* RemoveFromMovingList()
	*/ 
    void RemoveRefsToEntity(CEntity* entity);
	/* SetDamagedPieceRecord(float, CEntity*, CColPoint&, float)
	*/ 
    void SkipPhysics();
	/* TestCollision(bool)
	*/ 
    void UnsetIsInSafePosition();
	/* ~CPhysical()
    */
private:
    int m_pad1;
    float m_last_collision_check;

    uint32_t m_0x01 : 1; // 0x01
    uint32_t m_applyGravity : 1; // 0x02
    uint32_t m_disableCollisionForce : 1; // 0x04
    uint32_t m_collidable : 1; // 0x08
    uint32_t m_disableTurnForce : 1; // 0x10
    uint32_t m_disableMoveForce : 1; // 0x20
    uint32_t m_infiniteMass : 1; // 0x40
    uint32_t m_disableZ : 1; // 0x80
    uint32_t m_submerged_in_water : 1; // 0x100
    uint32_t m_on_solid_surface : 1; // 0x200
    uint32_t m_broken : 1; // 0x400
    uint32_t m_0x800 : 1;
    uint32_t m_0x1000 : 1;
    uint32_t m_dontApplySpeed : 1; // 0x2000
    uint32_t m_0x4000 : 1;
    uint32_t m_0x8000 : 1;
    uint32_t m_0x10000 : 1;
    uint32_t m_0x20000 : 1;
    uint32_t m_bulletproof : 1;
    uint32_t m_fireproof : 1;
    uint32_t m_collisionproof : 1;
    uint32_t m_meeleproof : 1;
    uint32_t m_invulnerable : 1;
    uint32_t m_explosionproof : 1;
    uint32_t m_0x1000000 : 1;
    uint32_t m_attached_to_entity : 1;
    uint32_t m_0x4000000 : 1;
    uint32_t m_touching_water : 1;
    uint32_t m_canBeCollidedWith : 1;
    uint32_t m_destroyed : 1;
    uint32_t m_0x40000000 : 1;
    uint32_t m_0x80000000 : 1;

    CVector m_linearVelocity;
    CVector m_angularVelocity;
    CVector m_frictionLinearVelocity;
    CVector m_frictionAngularVelocity;
    CVector m_vecUnk1;
    CVector m_vecUnk2;

    float m_mass;
    float m_turnMass;
    float m_pad2;
    float m_airResistance;
    float m_elasticity;
    float m_buoyancyConstant;
    CVector m_centerOfMass;
    class CEntryInfoList* m_collisionList;
    class CPtrNodeDoubleLink* m_movingList;
	uint8_t m_colFlag1;
	uint8_t m_numCollisions;
	uint8_t m_bCollideType;
	uint8_t m_colFlag4;
	CEntity* m_contactedEntities[4];
    int m_pad3;
    int m_pad4;
    int m_pad5;
    float m_collisionDamage;
    CEntity* m_collisionedEntity;
    CVector m_collision_impact_velocity;
    CVector m_collision_last_pos;
    uint16_t m_pieceType;
    uint16_t m_pad6;
	CEntity* m_attachedEntity;
	CVector m_attached_entity_offset;
    CVector m_attached_entity_pos;
    CQuaternion m_attached_entity_rotation;
    CEntity **m_PPWRONGattached_entity;
    float m_brightness1;
    float m_brightness2;
	CRealTimeShadow *m_pRealTimeShadow;
};

