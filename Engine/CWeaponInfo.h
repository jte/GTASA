#pragma once

struct WeaponAimOffset_t
{
    float aimX;
    float aimZ;
    float duckX;
    float duckZ;
    uint16_t RLoadA;
    uint16_t RLoadB;
    uint16_t CrouchRLoadA;
    uint16_t CrouchRLoadB;
};

enum eWeaponFireType
{
	WEAPON_FIRE_TYPE_MELEE,
	WEAPON_FIRE_TYPE_INSTANT_HIT,
	WEAPON_FIRE_TYPE_PROJECTILE,
	WEAPON_FIRE_TYPE_AREA_EFFECT,
	WEAPON_FIRE_TYPE_CAMERA,
	WEAPON_FIRE_TYPE_USE
};

enum eWeaponType
{
	WEAPON_UNARMED = 0x0,
	WEAPON_BRASSKNUCKLE = 0x1,
	WEAPON_GOLFCLUB = 0x2,
	WEAPON_NIGHTSTICK = 0x3,
	WEAPON_KNIFE = 0x4,
	WEAPON_BASEBALLBAT = 0x5,
	WEAPON_SHOVEL = 0x6,
	WEAPON_POOLCUE = 0x7,
	WEAPON_KATANA = 0x8,
	WEAPON_CHAINSAW = 0x9,
	WEAPON_DILDO1 = 0xA,
	WEAPON_DILDO2 = 0xB,
	WEAPON_VIBE1 = 0xC,
	WEAPON_VIBE2 = 0xD,
	WEAPON_FLOWERS = 0xE,
	WEAPON_CANE = 0xF,
	WEAPON_GRENADE = 0x10,
	WEAPON_TEARGAS = 0x11,
	WEAPON_MOLOTOV = 0x12,
	WEAPON_ROCKET = 0x13,
	WEAPON_ROCKET_HS = 0x14,
	WEAPON_FREEFALL_BOMB = 0x15,
	WEAPON_PISTOL = 0x16,
	WEAPON_PISTOL_SILENCED = 0x17,
	WEAPON_DESERT_EAGLE = 0x18,
	WEAPON_SHOTGUN = 0x19,
	WEAPON_SAWNOFF = 0x1A,
	WEAPON_SPAS12 = 0x1B,
	WEAPON_MICRO_UZI = 0x1C,
	WEAPON_MP5 = 0x1D,
	WEAPON_AK47 = 0x1E,
	WEAPON_M4 = 0x1F,
	WEAPON_TEC9 = 0x20,
	WEAPON_COUNTRYRIFLE = 0x21,
	WEAPON_SNIPERRIFLE = 0x22,
	WEAPON_RLAUNCHER = 0x23,
	WEAPON_RLAUNCHER_HS = 0x24,
	WEAPON_FTHROWER = 0x25,
	WEAPON_MINIGUN = 0x26,
	WEAPON_SATCHEL_CHARGE = 0x27,
	WEAPON_DETONATOR = 0x28,
	WEAPON_SPRAYCAN = 0x29,
	WEAPON_EXTINGUISHER = 0x2A,
	WEAPON_CAMERA = 0x2B,
	WEAPON_NIGHTVISION = 0x2C,
	WEAPON_INFRARED = 0x2D,
	WEAPON_PARACHUTE = 0x2E,
	WEAPON_UNUSED = 0x2F,
	WEAPON_ARMOUR = 0x30
};

enum eWeaponSlot
{
    WEAPONSLOT_TYPE_UNARMED,
    WEAPONSLOT_TYPE_MELEE,
    WEAPONSLOT_TYPE_HANDGUN,
    WEAPONSLOT_TYPE_SHOTGUN,
    WEAPONSLOT_TYPE_SMG,
    WEAPONSLOT_TYPE_MG, 
    WEAPONSLOT_TYPE_RIFLE,
    WEAPONSLOT_TYPE_HEAVY,
    WEAPONSLOT_TYPE_THROWN,
    WEAPONSLOT_TYPE_SPECIAL,
    WEAPONSLOT_TYPE_GIFT,
    WEAPONSLOT_TYPE_PARACHUTE,
    WEAPONSLOT_TYPE_DETONATOR,

  //  WEAPONSLOT_MAX
};

enum eWeaponSkill
{
    WEAPONSKILL_POOR = 0,
    WEAPONSKILL_STD,
    WEAPONSKILL_PRO,
    WEAPONSKILL_SPECIAL,    // for cops using pistols differently for example
    //WEAPONSKILL_MAX_NUMBER
};

class CWeaponInfo
{
public:
    static eWeaponFireType FindWeaponFireType(char const* fireTypeStr);
    static eWeaponType FindWeaponType(const char* weaponTypeStr);
	static int32_t GetSkillStatIndex(eWeaponType weaponType);
    static CWeaponInfo* GetWeaponInfo(eWeaponType type, eWeaponSkill skill);
	static void Initialise();
    static void LoadWeaponData();
	static void Shutdown();
private:
    static WeaponAimOffset_t ms_aWeaponAimOffsets[21];
	static const char ms_aWeaponNames[][16];
public:
    float GetTargetHeadRange();
    size_t GetWeaponReloadTime();
private:
    eWeaponFireType m_fireType;
    float m_targetRange;
    float m_weaponRange;
    int32_t m_modelId1;
    int32_t m_modelId2;
    eWeaponSlot m_weaponSlot;
    union
    {
        uint32_t m_flags;
        struct
        {
            uint32_t m_canAim : 1;
            uint32_t m_aimWithArm : 1;
            uint32_t m_1stPerson : 1;
            uint32_t m_onlyFreeAim : 1;
            uint32_t m_moveAim : 1;
            uint32_t m_moveFire : 1;
            uint32_t unused : 2;
            uint32_t m_throw : 1;
            uint32_t m_heavy : 1;
            uint32_t m_continuousFire : 1;
            uint32_t m_twinPistol : 1;
            uint32_t m_reloadable : 1;
            uint32_t m_crouchFire : 1;
            uint32_t m_reload2Start : 1;
            uint32_t m_longReload : 1;
            uint32_t m_slowdown : 1;
            uint32_t m_randSpeed : 1;
            uint32_t m_expands : 1;
        };
    };
    AssocGroupId m_animGroup;
    uint16_t m_ammo;
    uint16_t m_damage;
    CVector m_fireOffset;
    eWeaponSkill m_skillLevel;
    int m_reqStatLevel;
    float m_accuracy;
    float m_moveSpeed;
    float m_animLoopStart;
    float m_animLoopEnd;
    float m_animLoopFireTime;
    float m_anim2LoopStart;
    float m_anim2LoopEnd;
    float m_anim2LoopFireTime;
    float m_animBreakoutTime;
    float m_speed;
    float m_radius;
    float m_lifeSpan;
    float m_spread;
    uint16_t m_aimOffsetIndex;
    uint8_t m_defaultCombo;
    uint8_t m_numCombosAvailable;
};

extern CWeaponInfo aWeaponInfo[80];