#pragma once

#include "gdl/base/sse/abs.h"
#include "gdl/base/sse/crossProduct.h"
#include "gdl/base/sse/determinant.h"
#include "gdl/base/sse/dotProduct.h"
#include "gdl/base/sse/swizzle.h"
#include "gdl/math/solver/solver3.h"
#include "gdl/math/sse/mat3fSSE.h"
#include "gdl/math/sse/vec3fSSE.h"


namespace GDL::Solver
{

Vec3fSSE<true> Cramer(const Mat3fSSE& A, const Vec3fSSE<true>& b)
{
    using namespace GDL::sse;

    const std::array<__m128, 3>& dataA = A.DataSSE();

    __m128 cross12 = CrossProduct(dataA[1], dataA[2]);
    __m128 detA = DotProduct<1, 1, 1, 0>(dataA[0], cross12);

    DEV_EXCEPTION(_mm_cvtss_f32(detA) == ApproxZero<F32>(), "Singular matrix - system not solveable");

    __m128 x = DotProduct<1, 1, 1, 0>(b.DataSSE(), cross12);
    x = Blend<0, 1, 0, 0>(x, DotProduct<1, 1, 1, 0>(dataA[0], CrossProduct(b.DataSSE(), dataA[2])));
    x = Blend<0, 0, 1, 0>(x, DotProduct<1, 1, 1, 0>(dataA[0], CrossProduct(dataA[1], b.DataSSE())));

    return Vec3fSSE<true>(_mmx_div_p(x, detA));
}



Vec3fSSE<true> GaussPartialPivot(const Mat3fSSE& A, const Vec3fSSE<true>& b)
{
    // INFO:
    // -----
    // This algorithm uses a minimal number of instructions to calculate the result vector. Only matrix columns that are
    // needed in subsequent elimination steps are modified. Therefore the internaly used matrix is usually not equal to
    // the identity matrix when the algorithm has finished.

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

    //    std::array<__m128, 3> tmp;
    //    tmp[0] = Abs(*data[0]);
    //    tmp[1] = Broadcast<1>(tmp[0]);
    //    tmp[2] = Broadcast<2>(tmp[0]);
    //    if (_mm_comilt_ss(tmp[0], tmp[1]))
    //        idx = 1;
    //    if (_mm_comilt_ss(tmp[idx], tmp[2]))
    //        idx = 2;

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
    default:
        THROW("Invalid index");
    }


    // First elimination
    const __m128 m1 = _mmx_set1_p<__m128>(-1);
    const __m128 zero = _mmx_setzero_p<__m128>();

    __m128 bc = Broadcast<0>(*data[0]);
    __m128 mult0 = Blend<1, 0, 0, 0>(*data[0], m1);
    __m128 mult1 = _mmx_div_p(mult0, bc);

    for (U32 i = 1; i < 4; ++i)
    {
        bc = Broadcast<0>(*data[i]);
        *data[i] = _mm_fnmadd_ps(mult1, bc, Blend<1, 0, 0, 1>(*data[i], zero));
    }


    // Second pivoting step
    //    _mmx_store_p(colValues, Abs(*data[1]));
    //    if (colValues[1] < colValues[2])
    //        for (U32 i = 1; i < 4; ++i)
    //            *data[i] = Permute<0, 2, 1, 3>(*data[i]);

    __m128 absCol = Abs(*data[1]);
    if (_mm_comilt_ss(absCol, Broadcast<2>(absCol)))
        for (U32 i = 1; i < 4; ++i)
            *data[i] = Permute<0, 2, 1, 3>(*data[i]);


    // Second elimination
    bc = Broadcast<1>(*data[1]);
    mult0 = Blend<0, 1, 0, 0>(*data[1], m1);
    mult1 = _mmx_div_p(mult0, bc);

    for (U32 i = 2; i < 4; ++i)
    {
        bc = Broadcast<1>(*data[i]);
        *data[i] = _mm_fnmadd_ps(mult1, bc, Blend<0, 1, 0, 1>(*data[i], zero));
    }


    // Final elimination
    bc = Broadcast<2>(*data[2]);
    mult0 = Blend<0, 0, 1, 0>(*data[2], m1);
    mult1 = _mmx_div_p(mult0, bc);

    bc = Broadcast<2>(*data[3]);
    *data[3] = _mm_fnmadd_ps(mult1, bc, Blend<0, 0, 1, 1>(*data[3], zero));


    return Vec3fSSE<true>(rhs);
}



// std::cout << Vec3f(row) << std::endl;
// std::cout << rowValues[mapping[1]] << " < " << rowValues[mapping[2]] << std::endl;
// std::cout << mapping[1] << std::endl;
// std::cout << Mat3fSSE(*data[0], *data[1], *data[2]) << std::endl;

} // namespace GDL::Solver
