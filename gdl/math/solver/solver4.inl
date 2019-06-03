#pragma once

#include "gdl/math/solver/solver4.h"

#include "gdl/base/sse/abs.h"
#include "gdl/base/sse/crossProduct.h"
#include "gdl/base/sse/determinant.h"
#include "gdl/base/sse/dotProduct.h"
#include "gdl/base/sse/swizzle.h"
#include "gdl/math/sse/mat4fSSE.h"
#include "gdl/math/sse/vec4fSSE.h"


namespace GDL::Solver
{

// --------------------------------------------------------------------------------------------------------------------

Vec4fSSE<true> Cramer(const Mat4fSSE& A, const Vec4fSSE<true>& b)
{
    using namespace GDL::sse;

    F32 detA = A.Det();
    DEV_EXCEPTION(detA == ApproxZero<F32>(10), "Singular matrix - system not solveable");

    const std::array<__m128, 4>& dataA = A.DataSSE();
    const __m128 dataB = b.DataSSE();

    alignas(alignmentBytes<__m128>) std::array<F32, 4> detTmp;

    detTmp[0] = Determinant4x4(dataB, dataA[1], dataA[2], dataA[3]);
    detTmp[1] = Determinant4x4(dataA[0], dataB, dataA[2], dataA[3]);
    detTmp[2] = Determinant4x4(dataA[0], dataA[1], dataB, dataA[3]);
    detTmp[3] = Determinant4x4(dataA[0], dataA[1], dataA[2], dataB);

    return Vec4fSSE<true>(_mmx_div_p(_mmx_load_p<__m128>(detTmp.data()), _mmx_set1_p<__m128>(detA)));
}



// --------------------------------------------------------------------------------------------------------------------

Vec4fSSE<true> GaussPartialPivot(const Mat4fSSE& A, const Vec4fSSE<true>& b)
{
    using namespace GDL::sse;

    __m128 rhs = b.DataSSE();
    alignas(alignmentBytes<__m128>) std::array<__m128, 4> matrixData = A.DataSSE();
    const std::array<__m128* const, 5> data = {{&matrixData[0], &matrixData[1], &matrixData[2], &matrixData[3], &rhs}};


    // Find first pivot
    U32 idx = 0;
    alignas(alignmentBytes<__m128>) F32 colValues[4];

    _mmx_store_p(colValues, Abs(*data[0]));
    for (U32 i = 1; i < 4; ++i)
        if (colValues[idx] < colValues[i])
            idx = i;


    // First pivoting step
    switch (idx)
    {
    case 0:
        break;
    case 1:
        for (U32 i = 0; i < 5; ++i)
            *data[i] = Permute<1, 0, 2, 3>(*data[i]);
        break;
    case 2:
        for (U32 i = 0; i < 5; ++i)
            *data[i] = Permute<2, 1, 0, 3>(*data[i]);
        break;
    case 3:
        for (U32 i = 0; i < 5; ++i)
            *data[i] = Permute<3, 1, 2, 0>(*data[i]);
        break;
    }


    // First elimination
    Gauss4SSE::EliminationStep<0>(data);



    // Find second pivot
    idx = 1;

    _mmx_store_p(colValues, Abs(*data[1]));
    for (U32 i = 2; i < 4; ++i)
        if (colValues[idx] < colValues[i])
            idx = i;

    // First pivoting step
    switch (idx)
    {
    case 1:
        break;
    case 2:
        for (U32 i = 1; i < 5; ++i)
            *data[i] = Permute<0, 2, 1, 3>(*data[i]);
        break;
    case 3:
        for (U32 i = 1; i < 5; ++i)
            *data[i] = Permute<0, 3, 2, 1>(*data[i]);
        break;
    }


    // Second elimination
    Gauss4SSE::EliminationStep<1>(data);



    // Third pivoting step
    __m128 absCol = Abs(*data[2]);
    if (_mm_comilt_ss(Broadcast<2>(absCol), Broadcast<3>(absCol)))
        for (U32 i = 2; i < 5; ++i)
            *data[i] = Permute<0, 1, 3, 2>(*data[i]);


    // Third elimination
    Gauss4SSE::EliminationStep<2>(data);


    // Final elimination
    Gauss4SSE::EliminationStep<3>(data);

    return Vec4fSSE<true>(rhs);
}



// --------------------------------------------------------------------------------------------------------------------

Vec4fSSE<true> GaussNoPivot(const Mat4fSSE& A, const Vec4fSSE<true>& b)
{
    using namespace GDL::sse;

    __m128 rhs = b.DataSSE();
    alignas(alignmentBytes<__m128>) std::array<__m128, 4> matrixData = A.DataSSE();
    const std::array<__m128* const, 5> data = {{&matrixData[0], &matrixData[1], &matrixData[2], &matrixData[3], &rhs}};


    // First elimination
    Gauss4SSE::EliminationStep<0>(data);

    // Second elimination
    Gauss4SSE::EliminationStep<1>(data);

    // Third elimination
    Gauss4SSE::EliminationStep<2>(data);

    // Final elimination
    Gauss4SSE::EliminationStep<3>(data);

    return Vec4fSSE<true>(rhs);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _idx>
inline void Gauss4SSE::EliminationStep(const std::array<__m128* const, 5>& data)
{
    // INFO: This version is slightly faster than the one with the "-1". Reason: The "-1" version needs more
    // "expensive" operations in front of the for loop. For small systems the additional blends in the loop are cheaper
    // than those extra operations.
    using namespace GDL::sse;

    DEV_EXCEPTION(GetValue<_idx>(*data[_idx]) == ApproxZero<F32>(10), "Singular matrix - system not solveable");

    const __m128 m1 = _mmx_set1_p<__m128>(-1);
    const __m128 zero = _mmx_setzero_p<__m128>();

    __m128 bc = Broadcast<_idx>(*data[_idx]);
    const __m128 rowMult = _mmx_div_p(BlendIndex<_idx>(*data[_idx], m1), bc);

    for (U32 i = _idx + 1; i < 5; ++i)
    {
        bc = Broadcast<_idx>(*data[i]);
        *data[i] = _mmx_fnmadd_p(rowMult, bc, BlendIndex<_idx>(*data[i], zero));
    }
}

} // namespace GDL::Solver
