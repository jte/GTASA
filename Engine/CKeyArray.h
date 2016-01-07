#pragma once

class CKeyEntry
{
public:
    uint16_t* data;
    uint32_t keyHash;
};

class CKeyArray
{
public:
    CKeyArray();
    ~CKeyArray();
    CKeyEntry* BinarySearch(uint32_t key, CKeyEntry* array, uint16_t firstIndex, uint16_t lastIndex);
	bool Load(uint32_t size, int file, uint32_t* offset, bool skipRead);
	uint16_t* Search(const char* key, uint8_t* found);
	void Unload();
	void Update(uint16_t* p);
private:
    uint8_t* m_array;
    uint32_t m_size;
};
