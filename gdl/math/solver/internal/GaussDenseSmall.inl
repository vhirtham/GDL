#pragma once

#include "gdl/math/solver/internal/GaussDenseSmall.h"

#include "gdl/base/approx.h"
#include "gdl/base/exception.h"
#include "gdl/base/sse/directAccess.h"
#include "gdl/base/sse/swizzle.h"
#include "gdl/math/solver/internal/pivotDenseSmall.h"


#include <iostream>

namespace GDL::Solver
{


// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <U32 _idx, Pivot _pivot>
inline void GaussDenseSmallSerial<_type, _size>::GaussStep(std::array<_type, _size * _size>& matrixData,
                                                           std::array<_type, _size>& vectorData)
{
    if constexpr (_pivot == Pivot::PARTIAL && _idx + 1 < _size)
        PivotDenseSmallSerial<_type, _size>::template Partial<_idx, _idx>(matrixData, vectorData);
    GaussDenseSmallSerial<_type, _size>::template EliminationStep<_idx>(matrixData, vectorData);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <U32 _idx>
inline void GaussDenseSmallSerial<_type, _size>::EliminationStep(std::array<_type, _size * _size>& matrixData,
                                                                 std::array<_type, _size>& vectorData)
{
    constexpr U32 colStartIdx = _idx * _size;
    constexpr U32 pivotIdx = colStartIdx + _idx;

    DEV_EXCEPTION(matrixData[pivotIdx] == ApproxZero<_type>(1, 10), "Singular matrix - system not solveable");

    std::array<_type, _size> rowMult;

    // Calculate row multipliers
    _type div = 1 / matrixData[pivotIdx];

    matrixData[colStartIdx + _idx] -= 1;
    for (U32 i = 0; i < _size; ++i)
        rowMult[i] = div * matrixData[colStartIdx + i];

    // Perform elimination for all relevant columns
    for (U32 i = colStartIdx + _size; i < matrixData.size(); i += _size)
    {
        _type pivValue = matrixData[_idx + i];
        for (U32 j = 0; j < _size; ++j)
            matrixData[i + j] -= rowMult[j] * pivValue;
    }

    _type pivValue = vectorData[_idx];
    for (U32 i = 0; i < _size; ++i)
        vectorData[i] -= rowMult[i] * pivValue;
}



// --------------------------------------------------------------------------------------------------------------------


template <U32 _size>
template <U32 _idx>
inline void GaussDenseSmallSSE<_size>::EliminationStep(const std::array<__m128* const, _size + 1>& data)
{
    // INFO: This version is slightly faster than the one with the "-1". Reason: The "-1" version needs more
    // "expensive" operations in front of the for-loop. For small systems the additional blends in the loop are cheaper
    // than those extra operations.
    using namespace GDL::sse;

    DEV_EXCEPTION(GetValue<_idx>(*data[_idx]) == ApproxZero<F32>(10), "Singular matrix - system not solveable");

    const __m128 m1 = _mmx_set1_p<__m128>(-1);
    const __m128 zero = _mmx_setzero_p<__m128>();

    __m128 bc = Broadcast<_idx>(*data[_idx]);
    const __m128 rowMult = _mmx_div_p(BlendIndex<_idx>(*data[_idx], m1), bc);

    for (U32 i = _idx + 1; i < _size + 1; ++i)
    {
        bc = Broadcast<_idx>(*data[i]);
        *data[i] = _mmx_fnmadd_p(rowMult, bc, BlendIndex<_idx>(*data[i], zero));
    }
}



// --------------------------------------------------------------------------------------------------------------------

//#ifdef __AVX2__

// template <U32 _idx>
// inline void GaussDenseSmallAVX::EliminationStep(const std::array<__m256* const, 3>& data)
//{
//    // INFO: This version is slightly faster than the one with the "-1". Reason: The "-1" version needs more
//    // "expensive" operations in front of the for-loop. For small systems the additional blends in the loop are
//    cheaper
//    // than those extra operations.
//    using namespace GDL::sse;

//    constexpr U32 regIdx = _idx / 2;
//    constexpr U32 laneIdx = _idx % 2;
//    constexpr U32 valueIdx = _idx + laneIdx * 4;

//    DEV_EXCEPTION(GetValue<valueIdx>(*data[regIdx]) == ApproxZero<F32>(10), "Singular matrix - system not solveable");

//    const __m256 m1 = _mmx_set1_p<__m256>(-1);
//    const __m256 zero = _mmx_setzero_p<__m256>();

//    __m256 bc = Broadcast<_idx>(*data[regIdx]);
//    const __m256 rowMult = Permute2F128<laneIdx, laneIdx>(_mmx_div_p(BlendIndex<valueIdx>(*data[regIdx], m1), bc));



//    constexpr U32 b0 = (_idx == 0) ? 1 : 0;
//    constexpr U32 b1 = (_idx == 1) ? 1 : 0;
//    constexpr U32 b2 = (_idx == 2) ? 1 : 0;
//    constexpr U32 b3 = (_idx == 3) ? 1 : 0;

//    if constexpr (laneIdx == 0)
//        *data[2] = Blend<0, 0, 0, 0, 1, 1, 1, 1>(*data[2], *data[regIdx]);
//    for (U32 i = regIdx + 1; i < 3; ++i)
//    {
//        bc = Broadcast<_idx>(*data[i]);
//        *data[i] = _mmx_fnmadd_p(rowMult, bc, Blend<b0, b1, b2, b3, b0, b1, b2, b3>(*data[i], zero));
//    }
//    if constexpr (laneIdx == 0)
//        *data[regIdx] = *data[2];
//}

//#endif // __AVX2__



} // namespace GDL::Solver
