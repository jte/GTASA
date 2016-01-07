#include "StdInc.h"

CWeaponInfo aWeaponInfo[80];

const char CWeaponInfo::ms_aWeaponNames[][16] = 
{
    "UNARMED",
    "BRASSKNUCKLE",
    "GOLFCLUB",
    "NIGHTSTICK",
    "KNIFE",
    "BASEBALLBAT",
    "SHOVEL",
    "POOLCUE",
    "KATANA",
    "CHAINSAW",
    "DILDO1",
    "DILDO2",
    "VIBE1",
    "VIBE2",
    "FLOWERS",
    "CANE",
    "GRENADE",
    "TEARGAS",
    "MOLOTOV",
    "ROCKET",
    "ROCKET_HS",
    "FREEFALL_BOMB",
    "PISTOL",
    "PISTOL_SILENCED",
    "DESERT_EAGLE",
    "SHOTGUN",
    "SAWNOFF",
    "SPAS12",
    "MICRO_UZI",
    "MP5",
    "AK47",
    "M4",
    "TEC9",
    "COUNTRYRIFLE",
    "SNIPERRIFLE",
    "RLAUNCHER",
    "RLAUNCHER_HS",
    "FTHROWER",
    "MINIGUN",
    "SATCHEL_CHARGE",
    "DETONATOR",
    "SPRAYCAN",
    "EXTINGUISHER",
    "CAMERA",
    "NIGHTVISION",
    "INFRARED",
    "PARACHUTE",
    "",
    "ARMOUR"
};

void CWeaponInfo::LoadWeaponData()
{
    int file = CFileMgr::OpenFile("DATA\\WEAPON.DAT", "rb");
    for (const char* i = CFileLoader::LoadLine(file); i; i = CFileLoader::LoadLine(file))
    {
        if (!*i || *i == '#')
        {
            continue;
        }
        else if (*i == '£')
        {
            char weaponTypeStr[32], weaponFireTypeStr[32], comboTypeStr[8], marker[8], animGroupName[32];
            float targetRange, weaponRange;
            eWeaponSlot weaponSlot;
            size_t modelId1, modelId2, comboAvailable, flags;
            sscanf(i,"%s %s %s %f %f %d %d %d %s %d %x %s",
                marker, weaponTypeStr, weaponFireTypeStr,
                &targetRange, &weaponRange, &modelId1, &modelId2,
                &weaponSlot, comboTypeStr, &comboAvailable, &flags, animGroupName);
            eWeaponType weaponType = CWeaponInfo::FindWeaponType(weaponTypeStr);
            aWeaponInfo[weaponType].m_targetRange = targetRange;
            aWeaponInfo[weaponType].m_fireType = CWeaponInfo::FindWeaponFireType(weaponFireTypeStr);
            aWeaponInfo[weaponType].m_modelId2 = modelId2;
            aWeaponInfo[weaponType].m_weaponRange = weaponRange;
            aWeaponInfo[weaponType].m_modelId1 = modelId1;
            aWeaponInfo[weaponType].m_weaponSlot = weaponSlot;
            aWeaponInfo[weaponType].m_numCombosAvailable = comboAvailable;
            aWeaponInfo[weaponType].m_defaultCombo = CTaskSimpleFight::GetComboType(comboTypeStr);
            aWeaponInfo[weaponType].m_flags = flags;
            if (strncmp(animGroupName, "null", 4u) != 0)
            {
                for (size_t j = 0; j < CAnimManager::ms_numAnimAssocDefinitions; j++)
                {
                    if (!strcmp(animGroupName, CAnimManager::GetAnimGroupName(j)->szGroupName))
                    {
                        aWeaponInfo[weaponType].m_animGroup = j;
                        break;
                    }
                }
            }
            if (modelId1 > 0)
            {
                CWeaponModelInfo *weapModel = (CWeaponModelInfo*)CModelInfo::GetModelInfo(modelId1);
                weapModel->SetWeaponType(weaponType);
            }
        }
        else if (*i == '$')
        {
            char weaponTypeStr[32], weaponFireTypeStr[32], comboTypeStr[8], marker[8], animGroupName[32];
            float spread = 0, lifespan = 0, radius = 0, speed = 0;
            float accuracy, moveSpeed, animLoopStart, animLoopEnd, animLoopFire;
            float animLoop2Start, animLoop2End, animLoop2Fire, breakoutTime;
            size_t flags, ammoClip, damage, skillLevel, statLevel, modelId1, modelId2, weaponSlot;
            float fireOffsetX, fireOffsetY, fireOffsetZ, targetRange, weaponRange;
            sscanf(i, "%s %s %s %f %f %d %d %d %s %d %d %f %f %f %d %d %f %f %d %d %d %d %d %d %d %x %f %f %f %f",
              marker, weaponTypeStr, weaponFireTypeStr, &targetRange, &weaponRange, &modelId1, &modelId2, &weaponSlot, &animGroupName,
              &ammoClip,  &damage, &fireOffsetX, &fireOffsetY, &fireOffsetZ, &skillLevel, &statLevel, &accuracy,
              &moveSpeed, &animLoopStart, &animLoopEnd, &animLoopFire, &animLoop2Start, &animLoop2End, &animLoop2Fire, &breakoutTime,
              &flags, &speed, &radius, &lifespan, &spread);
            eWeaponType weaponType = CWeaponInfo::FindWeaponType(weaponTypeStr);
            size_t weapIdx = weaponType;
            if (weaponType < WEAPON_PISTOL || weaponType > WEAPON_TEC9)
            {
                skillLevel = WEAPONSKILL_STD;
            }
            else if (skillLevel == WEAPONSKILL_POOR)
            {
                weapIdx = weaponType + 25;
            }
            else if (skillLevel == WEAPONSKILL_PRO)
            {
                weapIdx = weaponType + 36;
            }
            else if (skillLevel == WEAPONSKILL_SPECIAL)
            {
                weapIdx = weaponType + 47;
            }
            aWeaponInfo[weapIdx].m_fireType = CWeaponInfo::FindWeaponFireType(weaponFireTypeStr);
            aWeaponInfo[weapIdx].m_targetRange = targetRange;
            aWeaponInfo[weapIdx].m_weaponRange = weaponRange;
            aWeaponInfo[weapIdx].m_animLoopStart = animLoopStart / 30.0f;
            aWeaponInfo[weapIdx].m_modelId1 = modelId1;
            aWeaponInfo[weapIdx].m_modelId2 = modelId2;
            aWeaponInfo[weapIdx].m_weaponSlot = (eWeaponSlot)weaponSlot;
            aWeaponInfo[weapIdx].m_ammo = ammoClip;
            aWeaponInfo[weapIdx].m_animLoopEnd = animLoopEnd / 30.0f;
            aWeaponInfo[weapIdx].m_damage = damage;
            aWeaponInfo[weapIdx].m_fireOffset.x = fireOffsetX;
            aWeaponInfo[weapIdx].m_fireOffset.y = fireOffsetY;
            aWeaponInfo[weapIdx].m_fireOffset.z = fireOffsetZ;
            aWeaponInfo[weapIdx].m_animLoopFireTime = animLoopFire / 30.0f;
            aWeaponInfo[weapIdx].m_moveSpeed = moveSpeed;
            aWeaponInfo[weapIdx].m_reqStatLevel = statLevel;
            aWeaponInfo[weapIdx].m_anim2LoopStart = animLoop2Start / 30.0f;
            aWeaponInfo[weapIdx].m_accuracy = accuracy;
            aWeaponInfo[weapIdx].m_radius = radius;
            aWeaponInfo[weapIdx].m_flags = flags;
            aWeaponInfo[weapIdx].m_anim2LoopEnd = animLoop2End / 30.0f;
            aWeaponInfo[weapIdx].m_speed = speed;
            aWeaponInfo[weapIdx].m_skillLevel = (eWeaponSkill)skillLevel;
            aWeaponInfo[weapIdx].m_lifeSpan = lifespan;
            aWeaponInfo[weapIdx].m_anim2LoopFireTime = animLoop2Fire / 30.0f;
            aWeaponInfo[weapIdx].m_spread = spread;
            aWeaponInfo[weapIdx].m_animBreakoutTime = breakoutTime / 30.0f;
            if (strncmp(animGroupName, "null", 4) != 0)
            {
                for (size_t j = 0; j < CAnimManager::ms_numAnimAssocDefinitions; j++)
                {
                    if (!strcmp(animGroupName, CAnimManager::GetAnimGroupName(j)->szGroupName))
                    {
                        aWeaponInfo[weapIdx].m_animGroup = j;
                        break;
                    }
                }
            }

            if (aWeaponInfo[weapIdx].m_animGroup >= 11 && aWeaponInfo[weapIdx].m_animGroup <= 31)
            {
                aWeaponInfo[weapIdx].m_aimOffsetIndex = aWeaponInfo[weapIdx].m_animGroup - 11;
            }
            aWeaponInfo[weapIdx].m_animLoopEnd = floor((aWeaponInfo[weapIdx].m_animLoopEnd - aWeaponInfo[weapIdx].m_animLoopStart) * 50.0 + 0.1) * 0.02 - 0.006 + aWeaponInfo[weapIdx].m_animLoopStart;
            aWeaponInfo[weapIdx].m_anim2LoopEnd = floor((aWeaponInfo[weapIdx].m_anim2LoopEnd - aWeaponInfo[weapIdx].m_anim2LoopStart) * 50.0 + 0.1) * 0.02 - 0.006 + aWeaponInfo[weapIdx].m_anim2LoopStart;
            if (skillLevel == WEAPONSKILL_STD && weaponType != WEAPON_DETONATOR && modelId1 > 0)
            {
                CWeaponModelInfo* weapInfo = (CWeaponModelInfo*)CModelInfo::GetModelInfo(modelId1);
                weapInfo->SetWeaponType(weaponType);
            }
        }
        else if (*i == '%')
        {
            char marker[8], animGroupName[32];
            float aimX, aimZ, duckX, duckZ;
            uint16_t RLoadA, RLoadB, CrouchRLoadA, CrouchRLoadB;
            sscanf(i, "%s %s %f %f %f %f %d %d %d %d",
                marker, animGroupName,
                &aimX, &aimZ, &duckX, &duckZ,
                &RLoadA, &RLoadB, &CrouchRLoadA, &CrouchRLoadB);
            size_t j;
            for (j = 0; j < CAnimManager::ms_numAnimAssocDefinitions; j++)
            {
                if (!strcmp(animGroupName, CAnimManager::GetAnimGroupName(j)->szGroupName))
                {
                    break;
                }
            }
            size_t aimIndex = j - 11;
            CWeaponInfo::ms_aWeaponAimOffsets[aimIndex].aimX = aimX;
            CWeaponInfo::ms_aWeaponAimOffsets[aimIndex].aimZ = aimZ;
            CWeaponInfo::ms_aWeaponAimOffsets[aimIndex].duckX = duckX;
            CWeaponInfo::ms_aWeaponAimOffsets[aimIndex].duckZ = duckZ;
            CWeaponInfo::ms_aWeaponAimOffsets[aimIndex].RLoadA = RLoadA;
            CWeaponInfo::ms_aWeaponAimOffsets[aimIndex].RLoadB = RLoadB;
            CWeaponInfo::ms_aWeaponAimOffsets[aimIndex].CrouchRLoadA = CrouchRLoadA;
            CWeaponInfo::ms_aWeaponAimOffsets[aimIndex].CrouchRLoadB = CrouchRLoadB;
        }
        else
        {
            char marker[32];
            sscanf(i, "%s", marker);
            if (!strncmp(marker, "ENDWEAPONDATA", 13))
            {
                CFileMgr::CloseFile(file);
                return;
            }
        }
    }
    CFileMgr::CloseFile(file);
}

eWeaponFireType CWeaponInfo::FindWeaponFireType(char const* fireTypeStr)
{
    struct 
    {
        char* typeStr;
        eWeaponFireType type;
    } map[] = 
    { 
        {"USE", WEAPON_FIRE_TYPE_USE},
        {"CAMERA", WEAPON_FIRE_TYPE_CAMERA},
        {"AREA_EFFECT", WEAPON_FIRE_TYPE_AREA_EFFECT},
        {"PROJECTILE", WEAPON_FIRE_TYPE_PROJECTILE},
        {"INSTANT_HIT", WEAPON_FIRE_TYPE_INSTANT_HIT},
        {"MELEE", WEAPON_FIRE_TYPE_MELEE}
    };
    for (size_t i = 0; i < ELEMS_COUNT(map); i++)
    {
        if (!strcmp(fireTypeStr, map[i].typeStr))
        {
            return map[i].type;
        }
    }
    return WEAPON_FIRE_TYPE_INSTANT_HIT;
}

eWeaponType CWeaponInfo::FindWeaponType(const char* weaponTypeStr)
{
    for (size_t i = 0; i < ELEMS_COUNT(ms_aWeaponNames); i++)
    {
        if (!stricmp(weaponTypeStr, ms_aWeaponNames[i]))
        {
            return (eWeaponType)i;
        }
    }
    return WEAPON_UNARMED;
}

CWeaponInfo* CWeaponInfo::GetWeaponInfo(eWeaponType type, eWeaponSkill skill)
{
    switch (skill)
    {
        case WEAPONSKILL_POOR:
            return &aWeaponInfo[type + 25];
        break;
        case WEAPONSKILL_STD:
            return &aWeaponInfo[type];
        break;
        case WEAPONSKILL_PRO:
            return &aWeaponInfo[type + 36];
        break;
        case WEAPONSKILL_SPECIAL:
            return &aWeaponInfo[type + 47];
        break;
        default:
            return &aWeaponInfo[47];
        break;
    }
}

int32_t CWeaponInfo::GetSkillStatIndex(eWeaponType weaponType)
{
    if (weaponType < WEAPON_PISTOL || weaponType > WEAPON_TEC9)
    {
        return -1;
    }
    if (weaponType <= WEAPON_M4)
    {
        return weaponType - 22 + 69;
    }
    else if (weaponType == WEAPON_TEC9)
    {
        return 75;
    }
    else if (weaponType == WEAPON_COUNTRYRIFLE)
    {
        return 81;
    }
    else
    {
        return weaponType + 69;
    }
}

float CWeaponInfo::GetTargetHeadRange()
{
    return (m_skillLevel + 2) * m_weaponRange / 25.0f;
}

size_t CWeaponInfo::GetWeaponReloadTime()
{
    if (m_reloadable)
    {
        return m_twinPistol ? 2000 : 1000;
    }
    if (m_longReload)
    {
        return 1000;
    }
    size_t RLoadA = ms_aWeaponAimOffsets[m_aimOffsetIndex].RLoadA + 100;
    if (RLoadA > 400)
    {
        return RLoadA;
    }
    size_t CrouchRLoadA = ms_aWeaponAimOffsets[m_aimOffsetIndex].CrouchRLoadA + 100;
    if (CrouchRLoadA > 400)
    {
        return CrouchRLoadA;
    }
    size_t RLoadB = ms_aWeaponAimOffsets[m_aimOffsetIndex].RLoadB + 100;
    if (RLoadB > 400)
    {
        return RLoadB;
    }
    size_t crouchRLoadB = ms_aWeaponAimOffsets[m_aimOffsetIndex].CrouchRLoadB + 100;
    if (crouchRLoadB > 400)
    {
        return crouchRLoadB;
    }
    return 400;
}

void CWeaponInfo::Shutdown()
{
    //empty
}

void CWeaponInfo::Initialise()
{
    memset(aWeaponInfo, 0, sizeof(aWeaponInfo));
    for (size_t i = 0; i < ELEMS_COUNT(aWeaponInfo); i++)
    {
        aWeaponInfo[i].m_modelId1 = -1;
        aWeaponInfo[i].m_modelId2 = -1;
        aWeaponInfo[i].m_weaponSlot = (eWeaponSlot)-1;
        aWeaponInfo[i].m_skillLevel = WEAPONSKILL_STD;
        aWeaponInfo[i].m_accuracy = 1.0f;
        aWeaponInfo[i].m_moveSpeed = 1.0f;
        aWeaponInfo[i].m_defaultCombo = 4;
        aWeaponInfo[i].m_numCombosAvailable = 1;
    }
    memset(ms_aWeaponAimOffsets, 0, sizeof(ms_aWeaponAimOffsets));
    LoadWeaponData();
}