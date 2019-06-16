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
#include "gdl/math/sse/mat4fAVX.h"
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

    const std::array<__m128, 4>& matrixData = A.DataSSE();
    const __m128& col0 = matrixData[0];
    const __m128& col1 = matrixData[1];
    const __m128& col2 = matrixData[2];
    const __m128& col3 = matrixData[3];


    // Calculate first 4 terms of det(A)

    __m128 col0P1230 = Permute<1, 2, 3, 0>(col0);
    __m128 col1P1230 = Permute<1, 2, 3, 0>(col1);
    __m128 col2P1230 = Permute<1, 2, 3, 0>(col2);
    __m128 col3P1230 = Permute<1, 2, 3, 0>(col3);

    __m128 tmp0 = _mmx_fmsub_p(col0, col1P1230, _mm_mul_ps(col0P1230, col1));
    __m128 tmp1 = _mmx_fmsub_p(col2, col3P1230, _mm_mul_ps(col2P1230, col3));

    __m128 tmp1P2301 = Permute<2, 3, 0, 1>(tmp1);
    __m128 tmp1P2301N = Negate<0, 1, 0, 1>(tmp1P2301);

    __m128 prodSum03 = DotProduct(tmp0, tmp1P2301N);


    // Calculate last 2 terms of det(A)

    __m128 b0 = Blend<0, 0, 1, 1>(col0, col2);
    __m128 b1 = Blend<1, 1, 0, 0>(col0, col2);
    __m128 b2 = Blend<0, 0, 1, 1>(col1, col3);
    __m128 b3 = Blend<1, 1, 0, 0>(col1, col3);

    __m128 b1P2301 = Permute<2, 3, 0, 1>(b1);
    __m128 b3P2301 = Permute<2, 3, 0, 1>(b3);

    __m128 tmp4 = _mmx_fmsub_p(b0, b3P2301, _mm_mul_ps(b1P2301, b2));
    __m128 tmp4P3210 = Permute<3, 2, 1, 0>(tmp4);

    __m128 tmp5 = _mm_mul_ps(tmp4, tmp4P3210);
    __m128 prodSum45 = _mm_add_ps(tmp5, Permute<1, 0, 3, 2>(tmp5));


    // Calculate det(A)

    __m128 detA = _mmx_add_p(prodSum03, prodSum45);

    DEV_EXCEPTION(_mm_cvtss_f32(detA) == ApproxZero<F32>(10), "Singular matrix - system not solveable");


    // Calculate first 4 terms of modified determinants

    const __m128& v = b.DataSSE();
    __m128 vP1230 = Permute<1, 2, 3, 0>(v);

    __m128 tmp0V0 = _mmx_fmsub_p(v, col1P1230, _mm_mul_ps(vP1230, col1));
    __m128 tmp0V1 = _mmx_fmsub_p(col0, vP1230, _mm_mul_ps(col0P1230, v));
    __m128 tmp1V2 = _mmx_fmsub_p(v, col3P1230, _mm_mul_ps(vP1230, col3));
    __m128 tmp1V3 = _mmx_fmsub_p(col2, vP1230, _mm_mul_ps(col2P1230, v));

    __m128 tmp1V2P2301 = Permute<2, 3, 0, 1>(tmp1V2);
    __m128 tmp1V3P2301 = Permute<2, 3, 0, 1>(tmp1V3);

    __m128 tmp1V2P2301N = Negate<0, 1, 0, 1>(tmp1V2P2301);
    __m128 tmp1V3P2301N = Negate<0, 1, 0, 1>(tmp1V3P2301);


    // Calculate last 2 terms of modified determinants

    __m128 prod03V0 = _mmx_mul_p(tmp0V0, tmp1P2301N);
    __m128 prod03V1 = _mmx_mul_p(tmp0V1, tmp1P2301N);
    __m128 prod03V2 = _mmx_mul_p(tmp0, tmp1V2P2301N);
    __m128 prod03V3 = _mmx_mul_p(tmp0, tmp1V3P2301N);

    __m128 vP2301 = Permute<2, 3, 0, 1>(v);

    __m128 tmp4V02 = _mmx_fmsub_p(v, b3P2301, _mm_mul_ps(vP2301, b2));
    __m128 tmp4V13 = _mmx_fmsub_p(b0, vP2301, _mm_mul_ps(b1P2301, v));


    // Calculate modified determinants

    __m128 prod03V02b0 = Blend<0, 0, 1, 1>(prod03V0, prod03V2);
    __m128 prod03V02b1 = Blend<1, 1, 0, 0>(prod03V0, prod03V2);
    __m128 prod03V13b0 = Blend<0, 0, 1, 1>(prod03V1, prod03V3);
    __m128 prod03V13b1 = Blend<1, 1, 0, 0>(prod03V1, prod03V3);

    __m128 prod03V02b1P2301 = Permute<2, 3, 0, 1>(prod03V02b1);
    __m128 prod03V13b1P2301 = Permute<2, 3, 0, 1>(prod03V13b1);

    __m128 prodSumV02 = _mmx_fmadd_p(tmp4V02, tmp4P3210, _mmx_add_p(prod03V02b0, prod03V02b1P2301));
    __m128 prodSumV13 = _mmx_fmadd_p(tmp4V13, tmp4P3210, _mmx_add_p(prod03V13b0, prod03V13b1P2301));

    __m128 prodSumB0 = Blend<0, 1, 0, 1>(prodSumV02, prodSumV13);
    __m128 prodSumB1 = Blend<1, 0, 1, 0>(prodSumV02, prodSumV13);

    __m128 prodSumB1P1032 = Permute<1, 0, 3, 2>(prodSumB1);

    __m128 determinants = _mmx_add_p(prodSumB0, prodSumB1P1032);


    // Calculate result

    __m128 result = _mmx_div_p(determinants, detA);
    return Vec4fSSE<true>(result);
}



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

inline Vec4fSSE<true> Cramer(const Mat4fAVX& A, const Vec4fSSE<true>& b)
{
    using namespace GDL::sse;

    // Calculate determinant of A

    const std::array<__m256, 2>& dataA = A.DataAVX();
    const __m256& col01 = dataA[0];
    const __m256& col23 = dataA[1];

    __m256 col01P1230 = Permute<1, 2, 3, 0>(col01);
    __m256 col23P1230 = Permute<1, 2, 3, 0>(col23);
    __m256 tmp0 = _mmx_fmsub_p(col01, col23P1230, _mmx_mul_p(col23, col01P1230));
    tmp0 = Negate<0, 0, 0, 0, 1, 0, 1, 0>(tmp0);

    __m256 col01P2323 = Permute<2, 3, 2, 3>(col01);
    __m256 col23P2323 = Permute<2, 3, 2, 3>(col23);
    __m256 tmp1 = _mmx_fmsub_p(col01, col23P2323, _mmx_mul_p(col23, col01P2323));

    __m256 tmp2 = Blend<0, 0, 0, 0, 1, 1, 1, 1>(tmp0, tmp1);
    __m256 tmp3 = Permute2F128<0, 1, 1, 0>(tmp0, tmp1);
    tmp3 = Permute<2, 3, 0, 1, 1, 0, 2, 3>(tmp3);

    __m256 tmp4 = DotProduct(tmp2, tmp3);

    __m256 detA = _mmx_add_p(tmp4, Permute2F128<1, 0>(tmp4));

    DEV_EXCEPTION(_mm256_cvtss_f32(detA) == ApproxZero<F32>(10), "Singular matrix - system not solveable");

    const __m256 v = _mm256_insertf128_ps(_mm256_castps128_ps256(b.DataSSE()), b.DataSSE(), 1);
    __m256 vP1230 = Permute<1, 2, 3, 0>(v);


    // Calculate first 4 products of all modified matrix determinants

    __m256 tmp0V01 = _mmx_fmsub_p(v, col23P1230, _mmx_mul_p(vP1230, col23));
    tmp0V01 = Negate<0, 0, 0, 0, 1, 0, 1, 0>(tmp0V01);
    __m256 tmp0V23 = _mmx_fmsub_p(col01, vP1230, _mmx_mul_p(v, col01P1230));
    tmp0V23 = Negate<0, 0, 0, 0, 1, 0, 1, 0>(tmp0V23);

    __m256 tmp0SL = Permute2F128<1, 0>(tmp0);
    __m256 tmp0SLP2301 = Permute<2, 3, 0, 1>(tmp0SL);

    __m256 prod03V01 = _mmx_mul_p(tmp0V01, tmp0SLP2301);
    __m256 prod03V23 = _mmx_mul_p(tmp0V23, tmp0SLP2301);
    __m256 prod03B0 = Blend<0, 0, 1, 1, 0, 0, 1, 1>(prod03V01, prod03V23);
    __m256 prod03B1 = Blend<1, 1, 0, 0, 1, 1, 0, 0>(prod03V01, prod03V23);
    __m256 prod03B1P2301 = Permute<2, 3, 0, 1>(prod03B1);

    __m256 prodSum03 = _mmx_add_p(prod03B0, prod03B1P2301);


    // Calculate last 2 products of all modified matrix determinants

    __m256 blend1 = Blend<1, 1, 0, 0, 1, 1, 0, 0>(col01, col23);
    __m256 blend0 = Blend<0, 0, 1, 1, 0, 0, 1, 1>(col01, col23);
    __m256 blend0P2301 = Permute<2, 3, 0, 1>(blend0);

    __m256 vP2301 = Permute<2, 3, 0, 1>(v);

    __m256 tmp1V = _mmx_fmsub_p(blend0P2301, v, _mmx_mul_p(vP2301, blend1));
    __m256 tmp1SL = Permute2F128<1, 0>(tmp1);
    __m256 tmp1SLP1010 = Permute<1, 0, 1, 0>(tmp1SL);

    __m256 prod45V = _mmx_mul_p(tmp1V, tmp1SLP1010);


    // Calculate result

    __m256 prodSum05 = _mmx_add_p(prodSum03, prod45V);
    __m256 determinants = _mmx_add_p(prodSum05, Permute<1, 0, 3, 2>(prodSum05));
    __m256 determinantsSL = Permute2F128<1, 0>(determinants);
    __m256 determinants0123 = Blend<0, 1, 0, 1, 1, 0, 1, 0>(determinants, determinantsSL);
    __m256 result = _mmx_div_p(determinants0123, detA);

    return Vec4fSSE<true>(_mm256_castps256_ps128(result));
}

#endif // __AVX2__



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
