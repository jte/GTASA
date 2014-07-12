#pragma once

class CReference
{
public:
    CReference *pNext;
    CEntity *pEntity;
};

class CReferences
{
public:
    static void Init();
    static size_t ListSize(CReference *pStart);
    static void PruneAllReferencesInWorld();
    static void RemoveReferencesToPlayer();
protected:
private:
    static CReference aRefs[3000];
    static CReference *pEmptyList;
};