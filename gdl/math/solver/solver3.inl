#pragma once

#include "gdl/math/solver/solver3.h"

#include "gdl/base/approx.h"
#include "gdl/base/sse/abs.h"
#include "gdl/base/sse/crossProduct.h"
#include "gdl/base/sse/determinant.h"
#include "gdl/base/sse/directAccess.h"
#include "gdl/base/sse/dotProduct.h"
#include "gdl/base/sse/swizzle.h"
#include "gdl/math/solver/internal/GaussDenseSmall.h"
#include "gdl/math/serial/mat3Serial.h"
#include "gdl/math/serial/vec3Serial.h"
#include "gdl/math/sse/mat3fSSE.h"


namespace GDL::Solver
{

// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
inline Vec3Serial<_type, true> Cramer(const Mat3Serial<_type>& matA, const Vec3Serial<_type, true>& vecRhs)
{
    std::array<_type, 9> a = matA.Data();
    std::array<_type, 3> r = vecRhs.Data();

    std::array<_type, 3> crossbc = {{a[4] * a[8] - a[5] * a[7], a[5] * a[6] - a[3] * a[8], a[3] * a[7] - a[4] * a[6]}};

    std::array<_type, 3> crossrc = {{r[1] * a[8] - r[2] * a[7], r[2] * a[6] - r[0] * a[8], r[0] * a[7] - r[1] * a[6]}};

    std::array<_type, 3> crossbr = {{a[4] * r[2] - a[5] * r[1], a[5] * r[0] - a[3] * r[2], a[3] * r[1] - a[4] * r[0]}};

    _type detA = a[0] * crossbc[0] + a[1] * crossbc[1] + a[2] * crossbc[2];

    DEV_EXCEPTION(detA == ApproxZero<F32>(10), "Singular matrix - system not solveable");

    std::array<_type, 3> result = {{(r[0] * crossbc[0] + r[1] * crossbc[1] + r[2] * crossbc[2]) / detA,
                                    (a[0] * crossrc[0] + a[1] * crossrc[1] + a[2] * crossrc[2]) / detA,
                                    (a[0] * crossbr[0] + a[1] * crossbr[1] + a[2] * crossbr[2]) / detA}};

    return Vec3Serial<_type, true>(result);
}



// --------------------------------------------------------------------------------------------------------------------

inline Vec3fSSE<true> Cramer(const Mat3fSSE& matA, const Vec3fSSE<true>& vecRhs)
{
    using namespace GDL::sse;

    const std::array<__m128, 3>& dataA = matA.DataSSE();
    const __m128& a = dataA[0];
    const __m128& b = dataA[1];
    const __m128& c = dataA[2];

    const __m128& r = vecRhs.DataSSE();

    __m128 crossbc = CrossProduct(b, c);
    __m128 detA = DotProduct<1, 1, 1, 0>(a, crossbc);

    DEV_EXCEPTION(_mm_cvtss_f32(detA) == ApproxZero<F32>(10), "Singular matrix - system not solveable");

    __m128 crossrc = CrossProduct(r, c);
    __m128 crossbr = CrossProduct(b, r);

    __m128 determinants = DotProduct<1, 1, 1, 0>(r, crossbc);
    determinants = Blend<0, 1, 0, 0>(determinants, DotProduct<1, 1, 1, 0>(a, crossrc));
    determinants = Blend<0, 0, 1, 0>(determinants, DotProduct<1, 1, 1, 0>(a, crossbr));

    __m128 result = _mmx_div_p(determinants, detA);

    return Vec3fSSE<true>(result);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
inline Vec3Serial<_type, true> GaussPartialPivot(const Mat3Serial<_type>& A, const Vec3Serial<_type, true>& b)
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
    GaussDenseSmallSerial<_type, 3>::template EliminationStep<0>(matrixData, vectorData);


    // Second pivoting step
    if (std::abs(matrixData[4]) < std::abs(matrixData[5]))
    {
        std::swap(matrixData[4], matrixData[5]);
        std::swap(matrixData[7], matrixData[8]);
        std::swap(vectorData[1], vectorData[2]);
    }


    // Second elimination step
    GaussDenseSmallSerial<_type, 3>::template EliminationStep<1>(matrixData, vectorData);

    // Last elimination step
    GaussDenseSmallSerial<_type, 3>::template EliminationStep<2>(matrixData, vectorData);


    return Vec3Serial<_type, true>(vectorData);
}



// --------------------------------------------------------------------------------------------------------------------

inline Vec3fSSE<true> GaussPartialPivot(const Mat3fSSE& A, const Vec3fSSE<true>& b)
{
    using namespace GDL::sse;

    alignas(alignmentBytes<__m128>) std::array<__m128, 3> matrixData = A.DataSSE();
    __m128 rhs = b.DataSSE();
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
    GaussDenseSmallSSE<3>::EliminationStep<0>(data);


    // Second pivoting step
    __m128 absCol = Abs(*data[1]);
    if (_mm_comilt_ss(Broadcast<1>(absCol), Broadcast<2>(absCol)))
        for (U32 i = 1; i < 4; ++i)
            *data[i] = Permute<0, 2, 1, 3>(*data[i]);


    // Second elimination
    GaussDenseSmallSSE<3>::EliminationStep<1>(data);


    // Final elimination
    GaussDenseSmallSSE<3>::EliminationStep<2>(data);


    return Vec3fSSE<true>(rhs);
}



} // namespace GDL::Solver
