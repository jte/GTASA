#pragma once

class FxMemoryPool_c
{
public:
    bool Init();
    void Exit();
    void Reset();
    void* GetMem(int size, int align);
    void Optimise();
private:
    char* m_ptr;
    size_t m_size;
    size_t m_position;
};