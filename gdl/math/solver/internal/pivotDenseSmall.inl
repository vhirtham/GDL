#pragma once

#include "gdl/math/solver/internal/pivotDenseSmall.h"

#include "gdl/base/exception.h"
#include "gdl/base/simd/swizzle.h"


#include <iostream>
#include "gdl/math/simd/mat4fSSE.h"

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



// --------------------------------------------------------------------------------------------------------------------
template <typename _type, U32 _size>
template <Pivot _pivot, U32 _idx, U32 _idxColStart>
inline void PivotDenseSmallSerial<_type, _size>::PivotStep(std::array<_type, _size * _size>& matrixData,
                                                           std::array<_type, _size>& vectorData)
{
    static_assert(_pivot != Pivot::NONE, "Unneccessary function call");

    static_assert(_pivot == Pivot::PARTIAL, "Unsupported pivoting strategy");

    if constexpr (_pivot == Pivot::PARTIAL)
        Partial<_idx, _idxColStart>(matrixData, vectorData);
}



// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// SSE Version
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size>
inline U32 PivotDenseSmallSSE<_size>::CreatePermutationHash(__m128 permutation)
{
    alignas(simd::alignmentBytes<__m128>) F32 P[4];

    _mm_store(P, permutation);

    return (((static_cast<U32>(P[0])) << 6) | ((static_cast<U32>(P[1])) << 4) | ((static_cast<U32>(P[2])) << 2) |
            (static_cast<U32>(P[3])));
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size>
template <U32 _idx, U32 _idxColStart>
inline void PivotDenseSmallSSE<_size>::Partial(std::array<__m128, _size>& matrixData, __m128& vectorData)
{
    using namespace GDL::simd;

    static_assert(_idx + 1 < _size, "Unnecessary function call.");

    if constexpr (_idx + 2 < _size)
    {
        U32 idx = _idx;
        alignas(alignmentBytes<__m128>) F32 colValues[4];

        _mm_store(colValues, Abs(matrixData[_idx]));
        for (U32 i = _idx + 1; i < _size; ++i)
            if (colValues[idx] < colValues[i])
                idx = i;


        switch (idx)
        {
        case 0:
            if constexpr (_idx == 0)
                break;
        case 1:
            if constexpr (_idx == 1)
                break;
            else
            {
                for (U32 i = _idxColStart; i < _size; ++i)
                    matrixData[i] = Swap<_idx, 1>(matrixData[i]);
                vectorData = Swap<_idx, 1>(vectorData);
                break;
            }
        case 2:
            if constexpr (_idx == 2)
                break;
            else
            {
                for (U32 i = _idxColStart; i < _size; ++i)
                    matrixData[i] = Swap<_idx, 2>(matrixData[i]);
                vectorData = Swap<_idx, 2>(vectorData);
                break;
            }
        case 3:
            if constexpr (_idx == 3)
                break;
            else
            {
                for (U32 i = _idxColStart; i < _size; ++i)
                    matrixData[i] = Swap<_idx, 3>(matrixData[i]);
                vectorData = Swap<_idx, 3>(vectorData);
            }
        }
    }
    else
    {
        __m128 absCol = Abs(matrixData[_idx]);
        if (_mm_comilt_ss(Broadcast<_idx>(absCol), Broadcast<_idx + 1>(absCol)))
        {
            for (U32 i = _idxColStart; i < _size; ++i)
                matrixData[i] = Swap<_idx, _idx + 1>(matrixData[i]);
            vectorData = Swap<_idx, _idx + 1>(vectorData);
        }
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size>
inline __m128 PivotDenseSmallSSE<_size>::PermuteVector(const __m128& vec, U32 permutationHash)
{
    using namespace GDL::simd;

    switch (permutationHash)
    {
    case (((0) << 6) | (1) << 4) | ((2) << 2) | (3):
        return vec;
    case (((0) << 6) | (1) << 4) | ((3) << 2) | (2):
        return Permute<0, 1, 3, 2>(vec);
    case (((0) << 6) | (2) << 4) | ((1) << 2) | (3):
        return Permute<0, 2, 1, 3>(vec);
    case (((0) << 6) | (2) << 4) | ((3) << 2) | (1):
        return Permute<0, 2, 3, 1>(vec);
    case (((0) << 6) | (3) << 4) | ((2) << 2) | (1):
        return Permute<0, 3, 2, 1>(vec);
    case (((0) << 6) | (3) << 4) | ((1) << 2) | (2):
        return Permute<0, 3, 1, 2>(vec);

    case (((1) << 6) | (0) << 4) | ((2) << 2) | (3):
        return Permute<1, 0, 2, 3>(vec);
    case (((1) << 6) | (0) << 4) | ((3) << 2) | (2):
        return Permute<1, 0, 3, 2>(vec);
    case (((1) << 6) | (2) << 4) | ((0) << 2) | (3):
        return Permute<1, 2, 0, 3>(vec);
    case (((1) << 6) | (2) << 4) | ((3) << 2) | (0):
        return Permute<1, 2, 3, 0>(vec);
    case (((1) << 6) | (3) << 4) | ((2) << 2) | (0):
        return Permute<1, 3, 2, 0>(vec);
    case (((1) << 6) | (3) << 4) | ((0) << 2) | (2):
        return Permute<1, 3, 0, 2>(vec);

    case (((2) << 6) | (1) << 4) | ((0) << 2) | (3):
        return Permute<2, 1, 0, 3>(vec);
    case (((2) << 6) | (1) << 4) | ((3) << 2) | (0):
        return Permute<2, 1, 3, 0>(vec);
    case (((2) << 6) | (0) << 4) | ((1) << 2) | (3):
        return Permute<2, 0, 1, 3>(vec);
    case (((2) << 6) | (0) << 4) | ((3) << 2) | (1):
        return Permute<2, 0, 3, 1>(vec);
    case (((2) << 6) | (3) << 4) | ((0) << 2) | (1):
        return Permute<2, 3, 0, 1>(vec);
    case (((2) << 6) | (3) << 4) | ((1) << 2) | (0):
        return Permute<2, 3, 1, 0>(vec);

    case (((3) << 6) | (1) << 4) | ((2) << 2) | (0):
        return Permute<3, 1, 2, 0>(vec);
    case (((3) << 6) | (1) << 4) | ((0) << 2) | (2):
        return Permute<3, 1, 0, 2>(vec);
    case (((3) << 6) | (2) << 4) | ((1) << 2) | (0):
        return Permute<3, 2, 1, 0>(vec);
    case (((3) << 6) | (2) << 4) | ((0) << 2) | (1):
        return Permute<3, 2, 0, 1>(vec);
    case (((3) << 6) | (0) << 4) | ((2) << 2) | (1):
        return Permute<3, 0, 2, 1>(vec);
    case (((3) << 6) | (0) << 4) | ((1) << 2) | (2):
        return Permute<3, 0, 1, 2>(vec);
        // LCOV_EXCL_START
    default:
        THROW("Invalid permutation hash");
        // LCOV_EXCL_STOP
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size>
template <Pivot _pivot, U32 _idx, U32 _idxColStart>
inline void PivotDenseSmallSSE<_size>::PivotStep(std::array<__m128, _size>& matrixData, __m128& vectorData)
{
    static_assert(_pivot != Pivot::NONE, "Unneccessary function call");

    static_assert(_pivot == Pivot::PARTIAL, "Unsupported pivoting strategy");

    if constexpr (_pivot == Pivot::PARTIAL)
        Partial<_idx, _idxColStart>(matrixData, vectorData);
}


} // namespace GDL::Solver
