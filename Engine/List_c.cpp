#include "StdInc.h"

List_c::List_c()
{
    head = NULL;
    tail = NULL;
    numItems = 0;
}

void List_c::AddItem(ListItem_c* item)
{
    if (!head)
    {
        item->prev = 0;
        item->next = 0;
        tail = item;
    }
    else
    {
        item->next = head;
        item->prev = item;
    }
    head = item;
    numItems++;
}

void List_c::RemoveItem(ListItem_c* item)
{
    if (item->next)
    {
        item->next->prev = item->prev;
    }
    else
    {
        tail = item->prev;
    }
    if (item->prev)
    {
        item->prev->next = item->next;
    }
    else
    {
        head = item->next;
    }
    numItems--;
}

ListItem_c* List_c::RemoveHead()
{
    if (!head)
    {
        return NULL;
    }
    ListItem_c* hd = head;
    if (head == tail)
    {
        tail = NULL;
        head = NULL;
        numItems--;
    }
    else
    {
        head = head->next;
        if (head)
        {
            head->prev = NULL;
        }
        numItems--;
    }
    return hd;
}

void List_c::RemoveAll()
{
    head = NULL;
    tail = NULL;
    numItems = 0;
}

uint32_t List_c::GetNumItems()
{
    return numItems;
}

void List_c::AppendItem(ListItem_c* item)
{
    if (tail)
    {
        item->prev = tail;
        tail->next = item;
    }
    else
    {
        item->prev = 0;
        item->next = 0;
        head = item;
    }
    tail = item;
    numItems++;
}

void List_c::InsertAfterItem(ListItem_c* insertItem, ListItem_c* insertAfter)
{
    if (head)
    {
        ListItem_c* item = head;
        while (item != insertAfter)
        {
            item = item->next;
            if (!item)
            {
                break;
            }
        }
        if (item)
        {
            insertItem->prev = item;
            if (item->next)
            {
                insertItem->next = item->next;
                item->next->prev = insertItem;
            }
            else
            {
                insertItem->next = 0;
                tail = insertItem;
            }
            item->next = insertItem;
        }
    }
    numItems++;
}

ListItem_c* List_c::RemoveTail()
{
    if (!tail)
    {
        return NULL;
    }
    ListItem_c* tl = tail;
    tail = tail->prev;
    numItems--;
    return tl;
}

ListItem_c* List_c::GetTail()
{
    return tail;
}

ListItem_c* List_c::GetPrev(ListItem_c* item)
{
    return item->prev;
}

ListItem_c* List_c::GetItemOffset(bool startFromHead, int pos)
{
    if ( startFromHead )
    {
        if (pos <= 0)
        {
            return head;
        }
        ListItem_c* item = head;
        for (size_t i = 0; item;)
        {
            item = item->next;
            i++;
            if (i >= pos)
            {
                return item;
            }
        }
    }
    else
    {
        if (pos <= 0)
        {
            return tail;
        }
        ListItem_c* item = tail;
        for (size_t i = 0; item;)
        {
            item = item->prev;
            i++;
            if (i >= pos)
            {
                return item;
            }
        }
    }
    return 0;
}

void List_c::InsertBeforeItem(ListItem_c* insertItem, ListItem_c* beforeItem)
{
    if (head)
    {
        ListItem_c* item = head;
        while (item != beforeItem)
        {
            item = item->next;
            if (!item)
            {
                break;
            }
        }
        if (item)
        {
            if (item->prev)
            {
                insertItem->prev = item->prev;
                item->prev->next = insertItem;
            }
            else
            {
                insertItem->prev = NULL;
                head = insertItem;
            }
            item->prev = insertItem;
            insertItem->next = item;
        }
    }
    numItems++;
}