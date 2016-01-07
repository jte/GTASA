#pragma once

// class, subclass
template<typename _C, typename _S = _C>
class CPool
{
public:
    CPool(size_t count)
    {
        m_size = count * sizeof(_S);
        m_flags = new uint8_t[m_size];
        m_objects = new _S[m_size];
        m_initialized = true;
        m_top = 0;
        for (size_t i = 0; i < m_size; i++)
        {
            m_flags[i] |= 0x80;
        }
    }
    virtual _S* New()
    {
        while (m_top != m_size)
        {
            if (m_flags[m_top] & 0x80)
            {
                m_flags[m_top] &= 0x7f;
                m_flags[m_top] ^= (m_flags[m_top] ^ (m_flags[m_top] + 1)) & 0x7f;
                return m_objects[m_top];
            }
            m_top++;
        }
        return NULL;
    }
    virtual void Delete(_S* object)
    {
        size_t pos = (object - m_objects) / sizeof(_S);
        m_flags[pos] |= 0x80;
        if (pos < m_top)
        {
            m_top = pos;
        }
    }
    virtual void Delete(size_t id)
    {
        _S* object = m_flags[id] >= 0 ? m_objects[id] : NULL;
        Delete(object);
    }
    virtual _S* Get(size_t id) const
    {
        if(!(m_flags[id] & 0x80))
        {
            return m_objects[id];
        }
    }
    virtual size_t GetSize() const
    {
        return m_size;
    }
    virtual void Flush()
    {
        if(m_size > 0)
        {
            if(m_initialized)
            {
                delete [] m_objects;
                delete [] m_flags;
            }
            m_objects = NULL;
            m_flags = NULL;
            m_size = 0;
            m_top = 0;
        }
    }
    virtual ~CPool()
    {
        Flush();
    }
public:
    uint8_t* m_flags;
    _S *m_objects;
    uint32_t m_size;
    int32_t m_top;
    bool m_initialized;
protected:
private:
};