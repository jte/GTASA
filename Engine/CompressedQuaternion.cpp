#include "StdInc.h"

void DecompressQuaternion(CQuaternion& full, const CompressedQuaternion& compressed)
{
    full.pos.x = static_cast<float>(compressed.x / 4096.0f);
    full.pos.y = static_cast<float>(compressed.y / 4096.0f);
    full.pos.z = static_cast<float>(compressed.z / 4096.0f);
    full.w = static_cast<float>(compressed.w / 4096.0f);
}

void CompressQuaternion(const CQuaternion& full, CompressedQuaternion& compressed)
{
    compressed.x = static_cast<uint16_t>(full.pos.x * 4096.0f);
    compressed.y = static_cast<uint16_t>(full.pos.y * 4096.0f);
    compressed.z = static_cast<uint16_t>(full.pos.z * 4096.0f);
    compressed.w = static_cast<uint16_t>(full.w * 4096.0f);
}