#pragma once

struct CompressedQuaternion
{
    uint16_t x;
    uint16_t y;
    uint16_t z;
    uint16_t w; 
};

void DecompressQuaternion(CQuaternion& full, const CompressedQuaternion& compressed);
void CompressQuaternion(const CQuaternion& full, CompressedQuaternion& compressed);