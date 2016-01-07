#pragma once

#include "CompressedVector.h"
#include "Maths\CMatrix.h"

template<_T>
class CCompressedMatrixNotAligned
{
public:
    //* CompressFromFullMatrix(CMatrix&)
    void DecompressIntoFullMatrix(CMatrix& fullMatrix)
    {
        DecompressVector<_T>(fullMatrix.right, right);
        DecompressVector<_T>(fullMatrix.up, up);
        DecompressVector<_T>(fullMatrix.at, at);
        DecompressVector<_T>(fullMatrix.pos, pos);
    }
public:
    CompressedVector right;
    CompressedVector up;
    CompressedVector at;
    CompressedVector pos;
};