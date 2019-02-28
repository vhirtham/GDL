#pragma once

#include "gdl/math/solver/solver3.h"

#include "gdl/base/sse/abs.h"
#include "gdl/base/sse/crossProduct.h"
#include "gdl/base/sse/determinant.h"
#include "gdl/base/sse/dotProduct.h"
#include "gdl/base/sse/swizzle.h"
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

    DEV_EXCEPTION(_mm_cvtss_f32(detA) == ApproxZero<F32>(10), "Singular matrix - system not solveable");

    __m128 x = DotProduct<1, 1, 1, 0>(b.DataSSE(), cross12);
    x = Blend<0, 1, 0, 0>(x, DotProduct<1, 1, 1, 0>(dataA[0], CrossProduct(b.DataSSE(), dataA[2])));
    x = Blend<0, 0, 1, 0>(x, DotProduct<1, 1, 1, 0>(dataA[0], CrossProduct(dataA[1], b.DataSSE())));

    return Vec3fSSE<true>(_mmx_div_p(x, detA));
}


class GaussInternals
{
    friend Vec3fSSE<true> GaussPartialPivot(const Mat3fSSE&, const Vec3fSSE<true>&);

    //! @brief Performs the gauss elimination step starting with the specified matrix element on the main diagonal. Rows
    //! above and to the right of the specified element are not modified, since the elemination process is expected to
    //! work from left to right and top to bottom.
    //! @tparam _idx: Index of the element on the main diagonal of the matrix from which the elimination process is
    //! started
    //! @param data: Data of the linear system (matrix columns with appended rhs vector)
    //! @remark Since the current column is not needed in subsequent elimination steps, it will not be modified to
    //! maximize performance. Therefore the matrix is not equal to the identity matrix when all elimination steps are
    //! done.
    template <U32 _idx>
    static inline void EliminationStep(const std::array<__m128* const, 4>& data)
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
            *data[i] = _mm_fnmadd_ps(mult1, bc, Blend<b0, b1, b2, 1>(*data[i], zero));
        }
    }
};



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
    default:
        THROW("Invalid index");
    }


    // First elimination
    GaussInternals::EliminationStep<0>(data);


    // Second pivoting step
    __m128 absCol = Abs(*data[1]);
    if (_mm_comilt_ss(Broadcast<1>(absCol), Broadcast<2>(absCol)))
        for (U32 i = 1; i < 4; ++i)
            *data[i] = Permute<0, 2, 1, 3>(*data[i]);


    // Second elimination
    GaussInternals::EliminationStep<1>(data);


    // Final elimination
    GaussInternals::EliminationStep<2>(data);

    return Vec3fSSE<true>(rhs);
}



// std::cout << Vec3f(row) << std::endl;
// std::cout << rowValues[mapping[1]] << " < " << rowValues[mapping[2]] << std::endl;
// std::cout << mapping[1] << std::endl;
// std::cout << Mat3fSSE(*data[0], *data[1], *data[2]) << std::endl;

} // namespace GDL::Solver
