#include "StdInc.h"

const char* CIplStore::GetIplName(size_t id)
{
    return ms_pPool.Get(id)->name;
}

void CIplStore::SetIsInterior(size_t id, bool isInterior)
{
    ms_pPool.Get(id)->isInterior = isInterior;
}

size_t CIplStore::FindIplSlot(const char* name)
{
    if (ms_pPool.GetSize() == 0)
    {
        return -1;
    }
    for (size_t i = 0; i < ms_pPool.GetSize(); i++)
    {
        IplDef* def = ms_pPool.Get(i);
        if(def && !stricmp(name, def->name))
        {
            return i;
        }
    }
    return -1;
}