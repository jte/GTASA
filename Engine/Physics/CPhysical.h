#pragma once

class CPhysical : public CEntity
{
public:
	//* Add();
	void AddCollisionRecord(CEntity *pEntity);
	void AddToMovingList();
	void ApplyAirResistance();
	//* ApplyCollision(CEntity*, CColPoint&, float&)
	//* ApplyCollision(CPhysical*, CColPoint&, float&, float&)
	void ApplyCollisionAlt(CEntity*, CColPoint&, float&, CVector&, CVector&);
	//* ApplyForce(CVector, CVector, bool)
	//* ApplyFriction()
	//* ApplyFriction(CPhysical*, float, CColPoint&)
	bool ApplyFriction(float friction, CColPoint &point);
	void ApplyFrictionForce(CVector force, CVector point);
	void ApplyFrictionMoveForce(CVector force);
	//* ApplyFrictionTurnForce(CVector, CVector)
	void ApplyGravity();
	void ApplyMoveForce(CVector force);
	void ApplyMoveSpeed();
	//* ApplyScriptCollision(CVector, float, float, CVector*)
	//* ApplySoftCollision(CEntity*, CColPoint&, float&)
	//* ApplySoftCollision(CPhysical*, CColPoint&, float&, float&)
	//* ApplySpeed()
	void ApplySpringCollision(float k, CVector&, CVector&, float, float, float&);
	//* ApplySpringCollisionAlt(float, CVector&, CVector&, float, float, CVector&, float&)
	//* ApplySpringDampening(float, float, CVector&, CVector&, CVector&)
	//* ApplySpringDampeningOld(float, float, CVector&, CVector&, CVector&)
	void ApplyTurnForce(CVector, CVector);
	void ApplyTurnSpeed();
	void AttachEntityToEntity(CEntity *entity, CVector *offset, RtQuat *rotation);
	void AttachEntityToEntity(CEntity *entity, CVector offset, CVector position);
	CPhysical();
	bool CanPhysicalBeDamaged(eWeaponType weapon, unsigned char *fire_related);
	//* CheckCollision()
	//* CheckCollision_SimpleCar()
	//* DettachAutoAttachedEntity()
	//* DettachEntityFromEntity(float x, float y, float z, bool apply_rotation);
	CRect *GetBoundRect() const;
	bool GetHasCollidedWith(CEntity *entity);
	bool GetHasCollidedWithAnyObject();
	float GetLightingFromCol(bool bIsInInterior) const;
	float GetLightingTotal() const;
	void GetSpeed(CVector &speed, CVector point);
	static void PlacePhysicalRelativeToOtherPhysical(CPhysical *first, CPhysical *second, CVector offset);
	//* PositionAttachedEntity()
	//* ProcessCollision()
	//* ProcessCollisionSectorList(int, int)
	//* ProcessCollisionSectorList_SimpleCar(CRepeatSector*)
	//* ProcessControl()
	void ProcessEntityCollision(CEntity *pOther, CColPoint *pColPoint);
	//* ProcessShift()
	//* ProcessShiftSectorList(int, int)
	//* Remove();
	//* RemoveAndAdd();
	void RemoveFromMovingList();
	void RemoveRefsToEntity(CEntity *entity);
	//* SetDamagedPieceRecord(float, CEntity*, CColPoint&, float)
	//* SkipPhysics()
	//* TestCollision(bool)
	//* UnsetIsInSafePosition()
	~CPhysical();
	protected:
	private:
        int m_pad1;
        float m_last_collision_check;
        uint32_t m_0x01 : 1;
        uint32_t m_apply_gravity : 1;
        uint32_t m_disable_friction : 1;
        uint32_t m_collidable : 1;
        uint32_t m_disable_turn_force : 1;
        uint32_t m_disable_z_component : 1;
        uint32_t m_infinite_mass : 1;
        uint32_t m_0x80 : 1;
        uint32_t m_submerged_in_water : 1;
        uint32_t m_on_solid_surface : 1;
        uint32_t m_broken : 1;
        uint32_t m_0x800 : 1;
        uint32_t m_0x1000 : 1;
        uint32_t m_0x2000 : 1;
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
        uint32_t m_enable_collision : 1;
        uint32_t m_destroyed : 1;
        uint32_t m_0x40000000 : 1;
        uint32_t m_0x80000000 : 1;
        CVector m_linear_velocity;
        CVector m_angular_velocity;
        CVector m_friction_move_force;
        CVector m_friction_turn_force;
        CVector m_vecUnk1;
        CVector m_vecUnk2;
        float m_mass;
        float m_turn_mass;
        float m_pad2;
        float m_air_resistance;
        float m_elasticity;
        float m_buoyancy_constant;
        CVector m_center_of_mass;
        CEntryInfoList *m_pCollisionList;
        CPtrNodeDouble *m_pMovingList;
		uint8_t m_colFlag1;
		uint8_t m_bCollidesNum;
		uint8_t m_bCollideType;
		uint8_t m_colFlag4;
		CEntity *m_last_contacted_entity[4];
        int m_pad3;
        int m_pad4;
        int m_pad5;
        float m_collision_damage;
        CEntity *m_collisioned_entity;
        CVector m_collision_impact_velocity;
        CVector m_collision_last_pos;
        uint16_t m_piece_type;
        uint16_t m_pad6;
		CEntity *m_attached_entity;
		CVector m_attached_entity_offset;
        CVector m_attached_entity_pos;
        RtQuat m_attached_entity_rotation;
        CEntity **m_PPWRONGattached_entity;
        float m_brightness1;
        float m_brightness2;
		CShadowData *m_pRealTimeShadow;
};

