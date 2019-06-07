#pragma once

#include "gdl/math/solver/solver3.h"

#include "gdl/base/approx.h"
#include "gdl/base/sse/abs.h"
#include "gdl/base/sse/crossProduct.h"
#include "gdl/base/sse/determinant.h"
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
inline Vec3Serial<_type, true> Cramer(const Mat3Serial<_type>& A, const Vec3Serial<_type, true>& b)
{
    std::array<_type, 9> matrixData = A.Data();
    std::array<_type, 3> vectorData = b.Data();

    std::array<_type, 3> cross12 = {{matrixData[4] * matrixData[8] - matrixData[5] * matrixData[7],
                                     matrixData[5] * matrixData[6] - matrixData[3] * matrixData[8],
                                     matrixData[3] * matrixData[7] - matrixData[4] * matrixData[6]}};

    std::array<_type, 3> crossV2 = {{vectorData[1] * matrixData[8] - vectorData[2] * matrixData[7],
                                     vectorData[2] * matrixData[6] - vectorData[0] * matrixData[8],
                                     vectorData[0] * matrixData[7] - vectorData[1] * matrixData[6]}};

    std::array<_type, 3> cross1V = {{matrixData[4] * vectorData[2] - matrixData[5] * vectorData[1],
                                     matrixData[5] * vectorData[0] - matrixData[3] * vectorData[2],
                                     matrixData[3] * vectorData[1] - matrixData[4] * vectorData[0]}};

    _type detA = matrixData[0] * cross12[0] + matrixData[1] * cross12[1] + matrixData[2] * cross12[2];

    DEV_EXCEPTION(detA == ApproxZero<F32>(10), "Singular matrix - system not solveable");

    std::array<_type, 3> result = {
            {(vectorData[0] * cross12[0] + vectorData[1] * cross12[1] + vectorData[2] * cross12[2]) / detA,
             (matrixData[0] * crossV2[0] + matrixData[1] * crossV2[1] + matrixData[2] * crossV2[2]) / detA,
             (matrixData[0] * cross1V[0] + matrixData[1] * cross1V[1] + matrixData[2] * cross1V[2]) / detA}};

    return Vec3Serial<_type, true>(result);
}



// --------------------------------------------------------------------------------------------------------------------

inline Vec3fSSE<true> Cramer(const Mat3fSSE& A, const Vec3fSSE<true>& b)
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
