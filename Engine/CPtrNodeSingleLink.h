#pragma once

#include "CPools.h"

class CPtrNodeSingleLink 
{
public:
    void* operator new(size_t)
    {
        return CPools::GetPtrNodeSingleLinkPool()->New();
    }
    void operator delete(void* p)
    {
        CPools::GetPtrNodeSingleLinkPool()->Delete(p);
    }
//private:
    void* ptr;
    CPtrNodeSingleLink* next;
};

class CPtrListSingleLink
{
public:
    void Add(void* ptr)
    {
        CPtrNodeSingleLink* node = new CPtrNodeSingleLink;
        if (node)
        {
            node->ptr = ptr;
            node->next = head;
            head = node;
        }
    }
//private:
    CPtrNodeSingleLink* head;
};