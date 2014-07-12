#pragma once

struct CompressedVector
{
    uint16_t x;
    uint16_t y;
    uint16_t z;
};

template<int _C/* = 128*/>
void DecompressVector(CVector& full, const CompressedVector& compressed)
{
    full.x = static_cast<float>(compressed.x / _C);
    full.y = static_cast<float>(compressed.y / _C);
    full.z = static_cast<float>(compressed.z / _C);
}

template<int _C/* = 128*/>
void CompressVector(const CVector& full, CompressedVector& compressed)
{
    compressed.x = static_cast<uint16_t>(full.x * _C);
    compressed.y = static_cast<uint16_t>(full.y * _C);
    compressed.z = static_cast<uint16_t>(full.z * _C);
}