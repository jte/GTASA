#include "StdInc.h"

CAnimBlendStaticAssociation::CAnimBlendStaticAssociation()
{
}

CAnimBlendStaticAssociation::CAnimBlendStaticAssociation(RpClump*, CAnimBlendHierarchy*)
{

}

CAnimBlendStaticAssociation::~CAnimBlendStaticAssociation()
{
    FreeSequenceArray();
}

void CAnimBlendStaticAssociation::AllocateSequenceArray(int iCount)
{
    ppSequences = CMemoryMgr::Calloc(4 * iCount, NULL);
}

void CAnimBlendStaticAssociation::FreeSequenceArray()
{
    if(ppSequences)
    {
        CMemoryMgr::Free(ppSequences);
    }
}