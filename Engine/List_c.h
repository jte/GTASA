#pragma once

class ListItem_c;

class List_c
{
public:
    List_c();
    void AddItem(ListItem_c* item);
    void RemoveItem(ListItem_c* item);
    ListItem_c* RemoveHead();
    void RemoveAll();
    uint32_t GetNumItems();
    void AppendItem(ListItem_c* item);
    void InsertAfterItem(ListItem_c* insertItem, ListItem_c* afterItem);
    ListItem_c* RemoveTail();
    ListItem_c* GetTail();
    static ListItem_c* GetPrev(ListItem_c* item);
    ListItem_c* GetItemOffset(bool startFromHead, int pos);
	void InsertBeforeItem(ListItem_c* insertItem, ListItem_c* beforeItem);
//private:
    ListItem_c* head;
    ListItem_c* tail;
    uint32_t numItems;
};