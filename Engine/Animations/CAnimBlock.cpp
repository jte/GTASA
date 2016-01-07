#include "StdInc.h"

const char* CAnimBlock::GetName()
{
    return m_name;
}

bool CAnimBlock::IsLoaded()
{
    return m_loaded;
}

uint16_t CAnimBlock::GetReferenceCount()
{
    return m_refs;
}

int CAnimBlock::GetIdOffset()
{
    return m_idOffset;
}

size_t CAnimBlock::GetAnimationCount()
{
    return m_numAnims;
}

AssocGroupId CAnimBlock::GetFirstAssocGroup()
{
    return m_assocGroup;
}

void CAnimBlock::SetAnimationCount(size_t numAnims)
{
    m_numAnims = numAnims;
}

void CAnimBlock::SetName(const char* name)
{
    m_name = name;
}

void CAnimBlock::SetIdOffset(int idOffset)
{
    m_idOffset = idOffset;
}

void CAnimBlock::SetFirstAssocGroup(AssocGroupId assocGroup)
{
    m_assocGroup = assocGroup;
}

void CAnimBlock::SetLoaded(bool loaded)
{
    m_loaded = loaded;
}

void CAnimBlock::SetReferenceCount(uint16_t refs)
{
    m_refs = refs;
}