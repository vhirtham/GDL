#pragma once

#include "gdl/math/solver/solver4.h"

#include "gdl/base/sse/abs.h"
#include "gdl/base/sse/crossProduct.h"
#include "gdl/base/sse/determinant.h"
#include "gdl/base/sse/dotProduct.h"
#include "gdl/base/sse/swizzle.h"
#include "gdl/math/solver/internal/GaussDenseSmall.h"
#include "gdl/math/serial/mat4Serial.h"
#include "gdl/math/serial/vec4Serial.h"
#include "gdl/math/sse/mat4fSSE.h"
#include "gdl/math/sse/vec4fSSE.h"


namespace GDL::Solver
{



template <typename _type>
inline Vec4Serial<_type, true> Cramer(const Mat4Serial<_type>& A, const Vec4Serial<_type, true>& b)
{

    _type detA = A.Det();
    DEV_EXCEPTION(detA == ApproxZero<F32>(10), "Singular matrix - system not solveable");

    std::array<_type, 4> detTmp;
    std::array<_type, 16> matrixData = A.Data();
    matrixData[0] = b[0];
    matrixData[1] = b[1];
    matrixData[2] = b[2];
    matrixData[3] = b[3];
    detTmp[0] = Mat4Serial<_type>::Det(matrixData);

    matrixData[0] = A(0, 0);
    matrixData[1] = A(1, 0);
    matrixData[2] = A(2, 0);
    matrixData[3] = A(3, 0);
    matrixData[4] = b[0];
    matrixData[5] = b[1];
    matrixData[6] = b[2];
    matrixData[7] = b[3];
    detTmp[1] = Mat4Serial<_type>::Det(matrixData);

    matrixData[4] = A(0, 1);
    matrixData[5] = A(1, 1);
    matrixData[6] = A(2, 1);
    matrixData[7] = A(3, 1);
    matrixData[8] = b[0];
    matrixData[9] = b[1];
    matrixData[10] = b[2];
    matrixData[11] = b[3];
    detTmp[2] = Mat4Serial<_type>::Det(matrixData);

    matrixData[8] = A(0, 2);
    matrixData[9] = A(1, 2);
    matrixData[10] = A(2, 2);
    matrixData[11] = A(3, 2);
    matrixData[12] = b[0];
    matrixData[13] = b[1];
    matrixData[14] = b[2];
    matrixData[15] = b[3];
    detTmp[3] = Mat4Serial<_type>::Det(matrixData);

    detTmp[0] /= detA;
    detTmp[1] /= detA;
    detTmp[2] /= detA;
    detTmp[3] /= detA;

    return Vec4Serial<_type, true>(detTmp);
}



// --------------------------------------------------------------------------------------------------------------------

inline Vec4fSSE<true> Cramer(const Mat4fSSE& A, const Vec4fSSE<true>& b)
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

template <typename _type>
inline Vec4Serial<_type, true> GaussNoPivot(const Mat4Serial<_type>& A, const Vec4Serial<_type, true>& b)
{
    std::array<_type, 16> matrixData = A.Data();
    std::array<_type, 4> vectorData = b.Data();

    // First elimination step
    GaussDenseSmallSerial<_type, 4>::template EliminationStep<0>(matrixData, vectorData);

    // Second elimination step
    GaussDenseSmallSerial<_type, 4>::template EliminationStep<1>(matrixData, vectorData);

    // Third elimination step
    GaussDenseSmallSerial<_type, 4>::template EliminationStep<2>(matrixData, vectorData);

    // Last elimination step
    GaussDenseSmallSerial<_type, 4>::template EliminationStep<3>(matrixData, vectorData);

    return Vec4Serial<_type, true>(vectorData);
}



// --------------------------------------------------------------------------------------------------------------------

inline Vec4fSSE<true> GaussNoPivot(const Mat4fSSE& A, const Vec4fSSE<true>& b)
{
    using namespace GDL::sse;

    alignas(alignmentBytes<__m128>) std::array<__m128, 4> matrixData = A.DataSSE();
    __m128 rhs = b.DataSSE();
    const std::array<__m128* const, 5> data = {{&matrixData[0], &matrixData[1], &matrixData[2], &matrixData[3], &rhs}};


    // First elimination
    GaussDenseSmallSSE<4>::EliminationStep<0>(data);

    // Second elimination
    GaussDenseSmallSSE<4>::EliminationStep<1>(data);

    // Third elimination
    GaussDenseSmallSSE<4>::EliminationStep<2>(data);

    // Final elimination
    GaussDenseSmallSSE<4>::EliminationStep<3>(data);

    return Vec4fSSE<true>(rhs);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
inline Vec4Serial<_type, true> GaussPartialPivot(const Mat4Serial<_type>& A, const Vec4Serial<_type, true>& b)
{
    std::array<_type, 16> matrixData = A.Data();
    std::array<_type, 4> vectorData = b.Data();


    // Find first pivot
    U32 idx = 0;
    for (U32 i = 1; i < 4; ++i)
        if (std::abs(matrixData[idx]) < std::abs(matrixData[i]))
            idx = i;

    // First pivoting step
    if (idx != 0)
    {
        for (U32 i = 0; i < 16; i += 4)
            std::swap(matrixData[i], matrixData[i + idx]);
        std::swap(vectorData[0], vectorData[idx]);
    }

    // First elimination step
    GaussDenseSmallSerial<_type, 4>::template EliminationStep<0>(matrixData, vectorData);



    // Find second pivot
    idx = 5;
    for (U32 i = 5; i < 7; ++i)
        if (std::abs(matrixData[idx]) < std::abs(matrixData[i]))
            idx = i;

    // Second pivoting step
    idx -= 5;
    if (idx != 0)
    {
        for (U32 i = 5; i < 16; i += 4)
            std::swap(matrixData[i], matrixData[i + idx]);
        std::swap(vectorData[1], vectorData[idx + 1]);
    }

    // Second elimination step
    GaussDenseSmallSerial<_type, 4>::template EliminationStep<1>(matrixData, vectorData);


    // Third pivoting step
    if (std::abs(matrixData[10]) < std::abs(matrixData[11]))
    {
        std::swap(matrixData[10], matrixData[11]);
        std::swap(matrixData[14], matrixData[15]);
        std::swap(vectorData[2], vectorData[3]);
    }


    // Third elimination step
    GaussDenseSmallSerial<_type, 4>::template EliminationStep<2>(matrixData, vectorData);

    // Last elimination step
    GaussDenseSmallSerial<_type, 4>::template EliminationStep<3>(matrixData, vectorData);

    return Vec4Serial<_type, true>(vectorData);
}



// --------------------------------------------------------------------------------------------------------------------

inline Vec4fSSE<true> GaussPartialPivot(const Mat4fSSE& A, const Vec4fSSE<true>& b)
{
    using namespace GDL::sse;

    alignas(alignmentBytes<__m128>) std::array<__m128, 4> matrixData = A.DataSSE();
    __m128 rhs = b.DataSSE();
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
    GaussDenseSmallSSE<4>::EliminationStep<0>(data);



    // Find second pivot
    idx = 1;

    _mmx_store_p(colValues, Abs(*data[1]));
    for (U32 i = 2; i < 4; ++i)
        if (colValues[idx] < colValues[i])
            idx = i;

    // Second pivoting step
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
    GaussDenseSmallSSE<4>::EliminationStep<1>(data);



    // Third pivoting step
    __m128 absCol = Abs(*data[2]);
    if (_mm_comilt_ss(Broadcast<2>(absCol), Broadcast<3>(absCol)))
        for (U32 i = 2; i < 5; ++i)
            *data[i] = Permute<0, 1, 3, 2>(*data[i]);


    // Third elimination
    GaussDenseSmallSSE<4>::EliminationStep<2>(data);


    // Final elimination
    GaussDenseSmallSSE<4>::EliminationStep<3>(data);

    return Vec4fSSE<true>(rhs);
}


} // namespace GDL::Solver
