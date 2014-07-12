#include "StdInc.h"

CBaseModelInfo::CBaseModelInfo()
{
    m_usRefCount = 0;
    m_usTextureDictionary = -1;
}

void CBaseModelInfo::SetAnimFile(char const *szName)
{
    m_usTextureDictionary = CTxdStore::FindTxdSlot(szName);
    if(m_usTextureDictionary != -1)
    {
        m_usTextureDictionary = CTxdStore::AddTxdSlot(szName);
    }
}

void CBaseModelInfo::ClearTexDictionary()
{
    m_usTextureDictionary = -1;
}

void CBaseModelInfo::AddTexDictionaryRef()
{
    CTxdStore::AddRef(m_usTextureDictionary);
}

void CBaseModelInfo::RemoveTexDictionaryRef()
{
    CTxdStore::RemoveRef(m_usTextureDictionary);
}

void CBaseModelInfo::AddRef()
{
    m_usRefCount++;
    AddTexDictionaryRef();
}

void CBaseModelInfo::RemoveRef()
{
    m_usRefCount--;
    RemoveTexDictionaryRef();
}

size_t CBaseModelInfo::GetAnimFileIndex()
{
    return -1;
}