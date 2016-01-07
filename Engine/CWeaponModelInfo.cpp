#include "StdInc.h"

void CWeaponModelInfo::SetClump(RpClump *clump)
{
    CClumpModelInfo::SetClump(clump);
    RpClumpForAllAtomics(clump, CClumpModelInfo::SetAtomicRendererCB, CVisibilityPlugins::RenderWeaponCB);
    RwFrame* flash = GetFrameFromName(clump, "gunflash");
    if (flash)
    {
        RpAtomic* atomic = (RpAtomic*)GetFirstObject(flash);
        if (atomic)
        {
            CVehicle::SetComponentAtomicAlpha(atomic, 0);
            atomic->object.object.flags = 0;
            atomic->geometry->matList.materials[0]->surfaceProps.ambient = 16.0f;
        }
    }
}

void CWeaponModelInfo::Init()
{
    CClumpModelInfo::Init();
    m_weaponType = WEAPON_UNARMED;
}

size_t CWeaponModelInfo::GetModelType()
{
    return 4;
}

void CWeaponModelInfo::SetWeaponType(eWeaponType weaponType)
{
    m_weaponType = weaponType;
}

eWeaponType CWeaponModelInfo::GetWeaponType() const
{
    return m_weaponType;
}