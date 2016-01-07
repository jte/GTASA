#pragma once

class CAnimBlock
{
public:
    const char* GetName();
    bool IsLoaded();
    uint16_t GetReferenceCount();
    int GetIdOffset();
    size_t GetAnimationCount();
    AssocGroupId GetFirstAssocGroup();

    void SetAnimationCount(size_t numAnims);
    void SetName(const char* name);
    void SetIdOffset(int idOffset);
    void SetFirstAssocGroup(AssocGroupId assocGroup);
    void SetLoaded(bool loaded);
    void SetReferenceCount(uint16_t refs);
private:
    const char* m_name;
    bool m_loaded;
    char pad1;
    uint16_t m_refs;
    int m_idOffset;
    size_t m_numAnims;
    AssocGroupId m_assocGroup;
};
