#pragma once

#include "gdl/math/solver/internal/pivotDenseSmall.h"

#include "gdl/base/exception.h"
#include "gdl/base/sse/swizzle.h"


#include <iostream>
#include "gdl/math/sse/mat4fSSE.h"

namespace GDL::Solver
{

// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <U32 _idx, U32 _idxColStart>
inline void PivotDenseSmallSerial<_type, _size>::Partial(std::array<_type, _size * _size>& matrixData,
                                                         std::array<_type, _size>& vectorData)
{
    static_assert(_idx + 1 < _size, "Unnecessary function call.");

    constexpr U32 pivIdx = _idx * (1 + _size);

    if constexpr (_idx + 2 < _size)
    {
        // Find pivot
        U32 rowIdx = pivIdx;

        for (U32 i = pivIdx + 1; i < _size * (_idx + 1); ++i)
            if (std::abs(matrixData[rowIdx]) < std::abs(matrixData[i]))
                rowIdx = i;


        // Pivoting step
        U32 rowDiff = rowIdx - _idx * pivIdx;
        if (rowDiff > 0)
        {
            for (U32 i = _idx + _idxColStart * _size; i < _size * _size; i += _size)
                std::swap(matrixData[i], matrixData[i + rowDiff]);
            std::swap(vectorData[_idx], vectorData[_idx + rowDiff]);
        }
    }
    else
    {
        if (std::abs(matrixData[pivIdx]) < std::abs(matrixData[pivIdx + 1]))
        {
            for (U32 i = _idx + _idxColStart * _size; i < _size * _size; i += _size)
                std::swap(matrixData[i], matrixData[i + 1]);
            std::swap(vectorData[_idx], vectorData[_idx + 1]);
        }
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
inline std::array<_type, _size>
PivotDenseSmallSerial<_type, _size>::PermuteVector(const std::array<_type, _size>& vec,
                                                   const std::array<_type, _size>& permutation)
{
    std::array<_type, _size> rPermute;
    for (U32 i = 0; i < _size; ++i)
        rPermute[i] = vec[permutation[i]];

    return rPermute;
}


} // namespace GDL::Solver
