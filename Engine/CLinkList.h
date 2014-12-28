#pragma once

template<typename _T>
class CLink
{
public:
    _T data;
    CLink* next;
    CLink* prev;
    CLink* head;
    CLink* tail;
};

template<typename _T>
class CLinkList
{
public:
    void Init(size_t numElems)
    {
        m_links = new CLink<_T>[numElems];

        m_tail.head = &m_head;
        m_head.tail = &m_tail;
    
        CLink<_T>* sorted_tail = &m_sorted_tail;
        CLink<_T>* sorted_head = &m_sorted_head;
    
        m_sorted_head.tail = sorted_tail;
        m_sorted_tail.head = sorted_head;
    
        for (size_t i = 0; i < numElems; i++)
        {
            CLink<_T> link = m_links[i];
            link.tail = sorted_tail;
            sorted_tail = &link;
            m_sorted_head.tail->head = &link;
            link.head = sorted_head;
            m_sorted_head.tail = &link;
        }
    }

    void Clear()
    {
        for (CLink<_T>* link = m_head.tail; link != &m_tail; link = m_head.tail)
        {
            link->tail->head = link->head;
            link->head->tail = link->tail;

            link->tail = m_sorted_head.tail;
            m_sorted_head.tail->head = link;
            link->head = &m_sorted_head;
            m_sorted_head.tail = link;
        }
    }

    void Shutdown()
    {
        if (m_links)
        {
            delete [] m_links;
        }
        m_links = NULL;
    }

    CLink<_T>* Insert(const _T& item)
    {
        CLink<_T>* link = m_tail.head;
        if (link == &m_tail)
        {
            return 0;
        }
        link->data = item;
        link->prev->next = link->next;
        link->next->prev = link->prev;
        link->prev = m_head.prev;
        m_head.prev->next = link;
        link->next = &m_head;
        m_head.prev = link;
        return link;
    }

    CLink<_T>* InsertSorted(const _T& item)
    {
        CLink<_T>* unsorted;
        for (CLink<_T>* unsorted = m_head.tail; unsorted != &m_tail; unsorted = unsorted->tail)
        {
            if (unsorted->data.alpha >= item.alpha)
                break;
        }
        
        CLink<_T>* sorted = m_sorted_head.tail;
        if (sorted == &m_sorted_tail)
        {
            return 0;
        }
        else
        {
            sorted->data.object = item.object;
            sorted->data.callback = item.callback;
            sorted->data.alpha = item.alpha;
            sorted->tail->head = sorted->head;
            sorted->head->tail = sorted->tail;
            sorted->tail = unsorted->head->tail;
            unsorted->head->tail->head = sorted;
            sorted->head = unsorted->head;
            unsorted->head->tail = sorted;
        }
        return sorted;
    }
private:
    CLink<_T> m_head;
    CLink<_T> m_tail;
    CLink<_T> m_sorted_head;
    CLink<_T> m_sorted_tail;
    CLink<_T>* m_links;
};
