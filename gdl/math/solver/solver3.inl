#pragma once

#include "gdl/math/solver/solver3.h"

#include "gdl/base/approx.h"
#include "gdl/base/sse/abs.h"
#include "gdl/base/sse/crossProduct.h"
#include "gdl/base/sse/determinant.h"
#include "gdl/base/sse/dotProduct.h"
#include "gdl/base/sse/swizzle.h"
#include "gdl/math/serial/mat3Serial.h"
#include "gdl/math/serial/vec3Serial.h"
#include "gdl/math/sse/mat3fSSE.h"


namespace GDL::Solver
{


// --------------------------------------------------------------------------------------------------------------------

Vec3fSSE<true> Cramer(const Mat3fSSE& A, const Vec3fSSE<true>& b)
{
    using namespace GDL::sse;

    const std::array<__m128, 3>& dataA = A.DataSSE();

    __m128 cross12 = CrossProduct(dataA[1], dataA[2]);
    __m128 detA = DotProduct<1, 1, 1, 0>(dataA[0], cross12);

    DEV_EXCEPTION(_mm_cvtss_f32(detA) == ApproxZero<F32>(10), "Singular matrix - system not solveable");

    __m128 x = DotProduct<1, 1, 1, 0>(b.DataSSE(), cross12);
    x = Blend<0, 1, 0, 0>(x, DotProduct<1, 1, 1, 0>(dataA[0], CrossProduct(b.DataSSE(), dataA[2])));
    x = Blend<0, 0, 1, 0>(x, DotProduct<1, 1, 1, 0>(dataA[0], CrossProduct(dataA[1], b.DataSSE())));

    return Vec3fSSE<true>(_mmx_div_p(x, detA));
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
Vec3Serial<_type, true> GaussPartialPivot(const Mat3Serial<_type>& A, const Vec3Serial<_type, true>& b)
{
    std::array<_type, 9> matrixData = A.Data();
    std::array<_type, 3> vectorData = b.Data();

    U32 idx = 0;

    // Find first pivot
    for (U32 i = 1; i < 3; ++i)
        if (std::abs(matrixData[idx]) < std::abs(matrixData[i]))
            idx = i;

    // First pivoting step
    if (idx != 0)
    {
        for (U32 i = 0; i < 9; i += 3)
            std::swap(matrixData[i], matrixData[i + idx]);
        std::swap(vectorData[0], vectorData[idx]);
    }


    // First elimination step
    Gauss3Serial<_type>::template EliminationStep<0>(matrixData, vectorData);


    // Second pivoting step
    if (std::abs(matrixData[4]) < std::abs(matrixData[5]))
    {
        std::swap(matrixData[4], matrixData[5]);
        std::swap(matrixData[7], matrixData[8]);
        std::swap(vectorData[1], vectorData[2]);
    }


    // Second elimination step
    Gauss3Serial<_type>::template EliminationStep<1>(matrixData, vectorData);

    // Last elimination step
    Gauss3Serial<_type>::template EliminationStep<2>(matrixData, vectorData);


    return Vec3Serial<_type, true>(vectorData);
}



// --------------------------------------------------------------------------------------------------------------------

Vec3fSSE<true> GaussPartialPivot(const Mat3fSSE& A, const Vec3fSSE<true>& b)
{
    using namespace GDL::sse;

    __m128 rhs = b.DataSSE();
    alignas(alignmentBytes<__m128>) std::array<__m128, 3> matrixData = A.DataSSE();
    const std::array<__m128* const, 4> data = {{&matrixData[0], &matrixData[1], &matrixData[2], &rhs}};


    // Find first pivot
    U32 idx = 0;

    alignas(alignmentBytes<__m128>) F32 colValues[4];
    _mmx_store_p(colValues, Abs(*data[0]));
    for (U32 i = 1; i < 3; ++i)
        if (colValues[idx] < colValues[i])
            idx = i;

    // First pivoting step
    switch (idx)
    {
    case 0:
        break;
    case 1:
        for (U32 i = 0; i < 4; ++i)
            *data[i] = Permute<1, 0, 2, 3>(*data[i]);
        break;
    case 2:
        for (U32 i = 0; i < 4; ++i)
            *data[i] = Permute<2, 1, 0, 3>(*data[i]);
        break;
    }


    // First elimination
    Gauss3SSE::EliminationStep<0>(data);


    // Second pivoting step
    __m128 absCol = Abs(*data[1]);
    if (_mm_comilt_ss(Broadcast<1>(absCol), Broadcast<2>(absCol)))
        for (U32 i = 1; i < 4; ++i)
            *data[i] = Permute<0, 2, 1, 3>(*data[i]);


    // Second elimination
    Gauss3SSE::EliminationStep<1>(data);


    // Final elimination
    Gauss3SSE::EliminationStep<2>(data);


    return Vec3fSSE<true>(rhs);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
template <U32 _idx>
inline void Gauss3Serial<_type>::EliminationStep(std::array<_type, 9>& matrixData, std::array<_type, 3>& vectorData)
{
    constexpr U32 colStartIdx = _idx * 3;
    constexpr U32 pivotIdx = colStartIdx + _idx;

    DEV_EXCEPTION(matrixData[pivotIdx] == ApproxZero<_type>(1, 10), "Singular matrix - system not solveable");

    std::array<_type, 3> rowMult;

    // Calculate row multipliers
    _type div = 1 / matrixData[pivotIdx];

    matrixData[colStartIdx + _idx] -= 1;
    for (U32 i = 0; i < 3; ++i)
        rowMult[i] = div * matrixData[colStartIdx + i];

    // Perform elimination for all relevant columns
    for (U32 i = colStartIdx + 3; i < matrixData.size(); i += 3)
    {
        _type pivValue = matrixData[_idx + i];
        for (U32 j = 0; j < 3; ++j)
            matrixData[i + j] -= rowMult[j] * pivValue;
    }

    _type pivValue = vectorData[_idx];
    for (U32 i = 0; i < 3; ++i)
        vectorData[i] -= rowMult[i] * pivValue;
}



// --------------------------------------------------------------------------------------------------------------------


template <U32 _idx>
inline void Gauss3SSE::EliminationStep(const std::array<__m128* const, 4>& data)
{
    using namespace GDL::sse;

    DEV_EXCEPTION(GetValue<_idx>(*data[_idx]) == ApproxZero<F32>(10), "Singular matrix - system not solveable");

    constexpr U32 b0 = (_idx == 0) ? 1 : 0;
    constexpr U32 b1 = (_idx == 1) ? 1 : 0;
    constexpr U32 b2 = (_idx == 2) ? 1 : 0;

    const __m128 m1 = _mmx_set1_p<__m128>(-1);
    const __m128 zero = _mmx_setzero_p<__m128>();

    __m128 bc = Broadcast<_idx>(*data[_idx]);
    __m128 mult0 = Blend<b0, b1, b2, 0>(*data[_idx], m1);
    __m128 mult1 = _mmx_div_p(mult0, bc);

    for (U32 i = _idx + 1; i < 4; ++i)
    {
        bc = Broadcast<_idx>(*data[i]);
        *data[i] = _mmx_fnmadd_p(mult1, bc, Blend<b0, b1, b2, 1>(*data[i], zero));
    }
}


// std::cout << Vec3f(row) << std::endl;
// std::cout << rowValues[mapping[1]] << " < " << rowValues[mapping[2]] << std::endl;
// std::cout << mapping[1] << std::endl;
// std::cout << Mat3fSSE(*data[0], *data[1], *data[2]) << std::endl;

} // namespace GDL::Solver
