#include "StdInc.h"

CTimeInfo* CTimeInfo::FindOtherTimeModel(char *modelName)
{
    char name[24];
    strcpy(name, modelName);
    char *suffix = strstr(name, "_nt");
    /* invert suffix _dy -> _nt or _nt -> _dy */
    if (suffix)
    {
        strncpy(suffix, "_dy", 4);
    }
    else
    {
        suffix = strstr(name, "_dy");
        if (!suffix)
        {
            return NULL;
        }
        strncpy(suffix, "_nt", 4);
    }
    /* find invert suffixed model */
    size_t newHash = CKeyGen::GetUppercaseKey(name);
    for (size_t i = 0; i < 20000; i++)
    {
        CBaseModelInfo *info = CModelInfo::GetModelInfo(i);
        if (info)
        {
            CTimeInfo *timeInfo = info->GetTimeInfo();
            if (timeInfo && info->GetHashKey() == newHash)
            {
               m_objectId = i;
               return timeInfo;
            }
        }
    }
    return NULL;
}