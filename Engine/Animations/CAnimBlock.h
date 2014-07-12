#pragma once

class CAnimBlock
{
public:
    char* GetName() { return m_szName; }
    bool IsLoaded() const { return m_bLoaded; }
    uint16_t GetReferenceCount() const { return usRefs; }
    int GetIdOffset() const { return m_iIdOffset; }
    size_t GetAnimationCount() const { return iNumAnimations; }
    AssocGroupId GetFirstAssocGroup() const { return m_FirstAssocGroup; }

    void SetAnimationCount(size_t iAnimsCount) { iNumAnimations = iAnimsCount; }
    void SetName(const char szName[16]) { strcpy(m_szName, szName); }
    void SetIdOffset(int iIdOffset) { m_iIdOffset = iIdOffset; }
    void SetFirstAssocGroup(AssocGroupId FirstAssocGroup) { m_FirstAssocGroup = FirstAssocGroup; }
    void SetLoaded(bool bLoaded) { m_bLoaded = bLoaded; }
protected:
private:
    char m_szName[16];
    bool m_bLoaded;
    char pad1;
    uint16_t usRefs;
    int m_iIdOffset;
    size_t iNumAnimations;
    AssocGroupId m_FirstAssocGroup;
};