#include "StdInc.h"

void CMatrixLinkList::Init(int count)
{
    m_links.resize(count);
    m_list1_head.next = &m_list1_tail;
    m_list1_tail.prev = &m_list1_head;    
    m_list2_head.next = &m_list2_tail;
    m_list2_tail.prev = &m_list2_head;
    m_freelist_head.next = &m_freelist_tail;
    m_freelist_tail.prev = &m_freelist_head;
    std::vector<CMatrixLink*>::iterator it = m_links.begin();
    for(; it != m_links.end(); ++it)
    {
        (*it)->next = m_freelist_head.next;
        (*it)->prev = &m_freelist_head;
        m_freelist_head.next->prev = *it;
        m_freelist_head.next = *it;
    }
}

CMatrixLink *CMatrixLinkList::AddToList1()
{
    CMatrixLink *link = m_freelist_head.next;
    if(link == &m_freelist_tail)
    {
        return NULL;
    }
    // unlink from freelist
    link->next->prev = link->prev;
    link->prev->next = link->next;
    // link to list1_head
    link->next = m_list1_head.next;
    m_list1_head.next->prev = link;
    link->prev = &m_list1_head;
    m_list1_head.next = link;
    return link;
}

CMatrixLink *CMatrixLinkList::AddToList2()
{
    CMatrixLink *link = m_freelist_head.next;
    if(link == &m_freelist_tail)
    {
        return NULL;
    }
    // unlink from freelist
    link->next->prev = link->prev;
    link->prev->next = link->next;
    // link to list2_head
    link->next = m_list2_head.next;
    m_list2_head.next->prev = link;
    link->prev = &m_list2_head;
    m_list2_head.next = link;
    return link;
}

void CMatrixLinkList::MoveToList2(CMatrixLink *pLink)
{
    // unlink from current list
    pLink->next->prev = pLink->prev;
    pLink->prev->next = pLink->next;
    // link into list 2
    pLink->next = m_list2_head.next;
    m_list2_head.next->prev = pLink;
    pLink->prev = &m_list2_head;
    m_list2_head.next = pLink;
}