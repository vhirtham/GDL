#pragma once

#include "gdl/math/solver/solver4.h"


#include "gdl/base/sse/abs.h"
#include "gdl/base/sse/crossProduct.h"
#include "gdl/base/sse/compareAll.h"
#include "gdl/base/sse/determinant.h"
#include "gdl/base/sse/dotProduct.h"
#include "gdl/base/sse/swizzle.h"
#include "gdl/math/solver/internal/GaussDenseSmall.h"
#include "gdl/math/solver/internal/luDenseSmall.h"
#include "gdl/math/serial/mat4Serial.h"
#include "gdl/math/serial/vec4Serial.h"
#include "gdl/math/sse/mat4fAVX.h"
#include "gdl/math/sse/mat4fSSE.h"
#include "gdl/math/sse/vec4fSSE.h"


namespace GDL::Solver
{



// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
inline Vec4Serial<_type, true> Cramer(const Mat4Serial<_type>& matA, const Vec4Serial<_type, true>& vecRhs)
{
    const std::array<_type, 16>& A = matA.Data();


    // Calculate detrminant of A
    F32 ab01 = A[0] * A[5] - A[1] * A[4];
    F32 cd23 = A[10] * A[15] - A[11] * A[14];

    F32 ab12 = A[1] * A[6] - A[2] * A[5];
    F32 cd03 = A[8] * A[15] - A[11] * A[12];

    F32 ab23 = A[2] * A[7] - A[3] * A[6];
    F32 cd01 = A[8] * A[13] - A[9] * A[12];

    F32 ab30 = A[3] * A[4] - A[0] * A[7];
    F32 cd21 = A[10] * A[13] - A[9] * A[14];

    F32 ab02 = A[0] * A[6] - A[2] * A[4];
    F32 cd31 = A[11] * A[13] - A[9] * A[15];

    F32 ab13 = A[1] * A[7] - A[3] * A[5];
    F32 cd20 = A[10] * A[12] - A[8] * A[14];

    _type detA = ab01 * cd23 + ab12 * cd03 + ab23 * cd01 + ab30 * cd21 + ab02 * cd31 + ab13 * cd20;


    DEV_EXCEPTION(detA == ApproxZero<F32>(10), "Singular matrix - system not solveable");


    const std::array<_type, 4>& r = vecRhs.Data();


    // Calculate solution with modified determinants
    F32 rb01 = r[0] * A[5] - r[1] * A[4];
    F32 rb12 = r[1] * A[6] - r[2] * A[5];
    F32 rb23 = r[2] * A[7] - r[3] * A[6];
    F32 rb30 = r[3] * A[4] - r[0] * A[7];
    F32 rb02 = r[0] * A[6] - r[2] * A[4];
    F32 rb13 = r[1] * A[7] - r[3] * A[5];

    std::array<_type, 4> solution;
    solution[0] = (rb01 * cd23 + rb12 * cd03 + rb23 * cd01 + rb30 * cd21 + rb02 * cd31 + rb13 * cd20) / detA;


    F32 ar01 = A[0] * r[1] - A[1] * r[0];
    F32 ar12 = A[1] * r[2] - A[2] * r[1];
    F32 ar23 = A[2] * r[3] - A[3] * r[2];
    F32 ar30 = A[3] * r[0] - A[0] * r[3];
    F32 ar02 = A[0] * r[2] - A[2] * r[0];
    F32 ar13 = A[1] * r[3] - A[3] * r[1];

    solution[1] = (ar01 * cd23 + ar12 * cd03 + ar23 * cd01 + ar30 * cd21 + ar02 * cd31 + ar13 * cd20) / detA;


    F32 rd23 = r[2] * A[15] - r[3] * A[14];
    F32 rd03 = r[0] * A[15] - r[3] * A[12];
    F32 rd01 = r[0] * A[13] - r[1] * A[12];
    F32 rd21 = r[2] * A[13] - r[1] * A[14];
    F32 rd31 = r[3] * A[13] - r[1] * A[15];
    F32 rd20 = r[2] * A[12] - r[0] * A[14];

    solution[2] = (ab01 * rd23 + ab12 * rd03 + ab23 * rd01 + ab30 * rd21 + ab02 * rd31 + ab13 * rd20) / detA;


    F32 cr23 = A[10] * r[3] - A[11] * r[2];
    F32 cr03 = A[8] * r[3] - A[11] * r[0];
    F32 cr01 = A[8] * r[1] - A[9] * r[0];
    F32 cr21 = A[10] * r[1] - A[9] * r[2];
    F32 cr31 = A[11] * r[1] - A[9] * r[3];
    F32 cr20 = A[10] * r[0] - A[8] * r[2];

    solution[3] = (ab01 * cr23 + ab12 * cr03 + ab23 * cr01 + ab30 * cr21 + ab02 * cr31 + ab13 * cr20) / detA;


    return Vec4Serial<_type, true>(solution);
}



// --------------------------------------------------------------------------------------------------------------------

inline Vec4fSSE<true> Cramer(const Mat4fSSE& matA, const Vec4fSSE<true>& vecRhs)
{
    using namespace GDL::sse;

    const std::array<__m128, 4>& matrixData = matA.DataSSE();
    const __m128& a = matrixData[0];
    const __m128& b = matrixData[1];
    const __m128& c = matrixData[2];
    const __m128& d = matrixData[3];


    // Calculate sum of first 4 terms of det(A)
    __m128 aP1230 = Permute<1, 2, 3, 0>(a);
    __m128 bP1230 = Permute<1, 2, 3, 0>(b);
    __m128 cP1230 = Permute<1, 2, 3, 0>(c);
    __m128 dP1230 = Permute<1, 2, 3, 0>(d);

    __m128 ab03 = _mmx_fmsub_p(a, bP1230, _mm_mul_ps(aP1230, b));
    __m128 cd03 = _mmx_fmsub_p(c, dP1230, _mm_mul_ps(cP1230, d));

    __m128 cd03P2301 = Permute<2, 3, 0, 1>(cd03);
    __m128 cd03P2301N = Negate<0, 1, 0, 1>(cd03P2301);

    __m128 sum03 = DotProduct(ab03, cd03P2301N);


    // Calculate sum of last 2 terms of det(A)
    __m128 acB0011 = Blend<0, 0, 1, 1>(a, c);
    __m128 acB1100 = Blend<1, 1, 0, 0>(a, c);
    __m128 bdB0011 = Blend<0, 0, 1, 1>(b, d);
    __m128 bdB1100 = Blend<1, 1, 0, 0>(b, d);

    __m128 acB1100P2301 = Permute<2, 3, 0, 1>(acB1100);
    __m128 bdB1100P2301 = Permute<2, 3, 0, 1>(bdB1100);

    __m128 abcd45 = _mmx_fmsub_p(acB0011, bdB1100P2301, _mm_mul_ps(acB1100P2301, bdB0011));
    __m128 abcd45P3210 = Permute<3, 2, 1, 0>(abcd45);

    __m128 products45 = _mm_mul_ps(abcd45, abcd45P3210);
    __m128 sum45 = _mm_add_ps(products45, Permute<1, 0, 3, 2>(products45));


    // Calculate det(A)
    __m128 detA = _mmx_add_p(sum03, sum45);


    DEV_EXCEPTION(_mm_cvtss_f32(detA) == ApproxZero<F32>(10), "Singular matrix - system not solveable");


    // Calculate the first 4 terms of the modified matrix determinants
    const __m128& r = vecRhs.DataSSE();
    __m128 rP1230 = Permute<1, 2, 3, 0>(r);

    __m128 rb03 = _mmx_fmsub_p(r, bP1230, _mm_mul_ps(rP1230, b));
    __m128 ar03 = _mmx_fmsub_p(a, rP1230, _mm_mul_ps(aP1230, r));
    __m128 rd03 = _mmx_fmsub_p(r, dP1230, _mm_mul_ps(rP1230, d));
    __m128 cr03 = _mmx_fmsub_p(c, rP1230, _mm_mul_ps(cP1230, r));

    __m128 rd03P2301 = Permute<2, 3, 0, 1>(rd03);
    __m128 cr03P2301 = Permute<2, 3, 0, 1>(cr03);

    __m128 rd03P2301N = Negate<0, 1, 0, 1>(rd03P2301);
    __m128 cr03P2301N = Negate<0, 1, 0, 1>(cr03P2301);

    __m128 products03M0 = _mmx_mul_p(rb03, cd03P2301N);
    __m128 products03M1 = _mmx_mul_p(ar03, cd03P2301N);
    __m128 products03M2 = _mmx_mul_p(ab03, rd03P2301N);
    __m128 products03M3 = _mmx_mul_p(ab03, cr03P2301N);


    // Calculate last 2 terms of the modified determinants
    __m128 rP2301 = Permute<2, 3, 0, 1>(r);

    __m128 rbrd45 = _mmx_fmsub_p(r, bdB1100P2301, _mm_mul_ps(rP2301, bdB0011));
    __m128 arcr45 = _mmx_fmsub_p(acB0011, rP2301, _mm_mul_ps(acB1100P2301, r));


    // Calculate modified determinants
    __m128 products03M0M2B0011 = Blend<0, 0, 1, 1>(products03M0, products03M2);
    __m128 products03M0M2B1100 = Blend<1, 1, 0, 0>(products03M0, products03M2);
    __m128 products03M1M3B0011 = Blend<0, 0, 1, 1>(products03M1, products03M3);
    __m128 products03M1M3B1100 = Blend<1, 1, 0, 0>(products03M1, products03M3);

    __m128 products03M0M2B1100P2301 = Permute<2, 3, 0, 1>(products03M0M2B1100);
    __m128 products03M1M3B1100P2301 = Permute<2, 3, 0, 1>(products03M1M3B1100);

    __m128 sumsM0M2 = _mmx_fmadd_p(rbrd45, abcd45P3210, _mmx_add_p(products03M0M2B0011, products03M0M2B1100P2301));
    __m128 sumsM1M3 = _mmx_fmadd_p(arcr45, abcd45P3210, _mmx_add_p(products03M1M3B0011, products03M1M3B1100P2301));

    __m128 sumsB0101 = Blend<0, 1, 0, 1>(sumsM0M2, sumsM1M3);
    __m128 sumsB1010 = Blend<1, 0, 1, 0>(sumsM0M2, sumsM1M3);

    __m128 sumsB1010P1032 = Permute<1, 0, 3, 2>(sumsB1010);

    __m128 determinants = _mmx_add_p(sumsB0101, sumsB1010P1032);


    // Calculate and return solution
    __m128 solution = _mmx_div_p(determinants, detA);

    return Vec4fSSE<true>(solution);
}



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

inline Vec4fSSE<true> Cramer(const Mat4fAVX& matA, const Vec4fSSE<true>& vecRhs)
{
    using namespace GDL::sse;

    const std::array<__m256, 2>& dataA = matA.DataAVX();
    const __m256& ab = dataA[0];
    const __m256& cd = dataA[1];


    // Calculate the components of the first 4 terms of det(a)
    __m256 abP1230 = Permute<1, 2, 3, 0>(ab);
    __m256 cdP1230 = Permute<1, 2, 3, 0>(cd);

    __m256 acbd03 = _mmx_fmsub_p(ab, cdP1230, _mmx_mul_p(cd, abP1230));
    __m256 acbd03N = Negate<0, 0, 0, 0, 1, 0, 1, 0>(acbd03);


    // Calculate the components of the last 2 terms of det(a)
    __m256 abP2323 = Permute<2, 3, 2, 3>(ab);
    __m256 cdP2323 = Permute<2, 3, 2, 3>(cd);
    __m256 acbd45 = _mmx_fmsub_p(ab, cdP2323, _mmx_mul_p(cd, abP2323));


    // Calculate determinant of A
    __m256 ac03bd45 = Blend<0, 0, 0, 0, 1, 1, 1, 1>(acbd03N, acbd45);
    __m256 bd03ac45 = Permute2F128<0, 1, 1, 0>(acbd03N, acbd45);
    __m256 bd03ac45P = Permute<2, 3, 0, 1, 1, 0, 2, 3>(bd03ac45);

    __m256 sums = DotProduct(ac03bd45, bd03ac45P);

    __m256 detA = _mmx_add_p(sums, Permute2F128<1, 0>(sums));


    DEV_EXCEPTION(_mm256_cvtss_f32(detA) == ApproxZero<F32>(10), "Singular matrix - system not solveable");

    const __m256 r = _mm256_insertf128_ps(_mm256_castps128_ps256(vecRhs.DataSSE()), vecRhs.DataSSE(), 1);


    // Calculate first 4 products of all modified matrix determinants
    __m256 rP1230 = Permute<1, 2, 3, 0>(r);

    __m256 rcrd03 = _mmx_fmsub_p(r, cdP1230, _mmx_mul_p(rP1230, cd));
    __m256 arbr03 = _mmx_fmsub_p(ab, rP1230, _mmx_mul_p(r, abP1230));

    __m256 rcrd03N = Negate<0, 0, 0, 0, 1, 0, 1, 0>(rcrd03);
    __m256 arbr03N = Negate<0, 0, 0, 0, 1, 0, 1, 0>(arbr03);

    __m256 bdac03 = Permute2F128<1, 0>(acbd03N);
    __m256 bdac03P2301 = Permute<2, 3, 0, 1>(bdac03);

    __m256 products03M0M1 = _mmx_mul_p(rcrd03N, bdac03P2301);
    __m256 products03M2M3 = _mmx_mul_p(arbr03N, bdac03P2301);
    __m256 products03B0011 = Blend<0, 0, 1, 1, 0, 0, 1, 1>(products03M0M1, products03M2M3);
    __m256 products03B1100 = Blend<1, 1, 0, 0, 1, 1, 0, 0>(products03M0M1, products03M2M3);
    __m256 products03B1100P2301 = Permute<2, 3, 0, 1>(products03B1100);

    __m256 sum03 = _mmx_add_p(products03B0011, products03B1100P2301);


    // Calculate last 2 products of all modified matrix determinants
    __m256 acbdB1100 = Blend<1, 1, 0, 0, 1, 1, 0, 0>(ab, cd);
    __m256 acbdB0011 = Blend<0, 0, 1, 1, 0, 0, 1, 1>(ab, cd);
    __m256 acbdB0011P2301 = Permute<2, 3, 0, 1>(acbdB0011);

    __m256 rP2301 = Permute<2, 3, 0, 1>(r);

    __m256 components45 = _mmx_fmsub_p(acbdB0011P2301, r, _mmx_mul_p(rP2301, acbdB1100));
    __m256 bdac45 = Permute2F128<1, 0>(acbd45);
    __m256 bdac45P1010 = Permute<1, 0, 1, 0>(bdac45);

    __m256 products45M = _mmx_mul_p(components45, bdac45P1010);


    // Calculate and return solution
    __m256 sumsM = _mmx_add_p(sum03, products45M);
    __m256 determinants00221133 = _mmx_add_p(sumsM, Permute<1, 0, 3, 2>(sumsM));
    __m256 determinants11330022 = Permute2F128<1, 0>(determinants00221133);
    __m256 determinants = Blend<0, 1, 0, 1, 1, 0, 1, 0>(determinants00221133, determinants11330022);

    __m256 solution = _mmx_div_p(determinants, detA);

    return Vec4fSSE<true>(_mm256_castps256_ps128(solution));
}

#endif // __AVX2__



// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
inline Vec4Serial<_type, true> GaussNoPivot(const Mat4Serial<_type>& matA, const Vec4Serial<_type, true>& vecRhs)
{
    std::array<_type, 16> matrixData = matA.Data();
    std::array<_type, 4> vectorData = vecRhs.Data();

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

inline Vec4fSSE<true> GaussNoPivot(const Mat4fSSE& matA, const Vec4fSSE<true>& vecRhs)
{
    using namespace GDL::sse;

    alignas(alignmentBytes<__m128>) std::array<__m128, 4> matrixData = matA.DataSSE();
    __m128 rhs = vecRhs.DataSSE();
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

#ifdef __AVX2__

[[nodiscard]] inline Vec4fSSE<true> GaussNoPivot(const Mat4fAVX& A, const Vec4fSSE<true>& b)
{
    // INFO: There is no specialized AVX version of this function. During the elimination step, the row multipliers must
    // be transferred across lane boundaries which results in a performance penalty when compared to the SSE version.
    // Therefore, the AVX matrix uses the same function as the SSE matrix.

    static_assert(sizeof(Mat4fAVX) == sizeof(Mat4fSSE), "Internal error - Matrix types have different sizes");
    return GaussNoPivot(*reinterpret_cast<const Mat4fSSE*>(&A), b);
}

#endif // __AVX2__



// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
inline Vec4Serial<_type, true> GaussPartialPivot(const Mat4Serial<_type>& matA, const Vec4Serial<_type, true>& vecRhs)
{
    std::array<_type, 16> matrixData = matA.Data();
    std::array<_type, 4> vectorData = vecRhs.Data();


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

inline Vec4fSSE<true> GaussPartialPivot(const Mat4fSSE& matA, const Vec4fSSE<true>& vecRhs)
{
    using namespace GDL::sse;

    alignas(alignmentBytes<__m128>) std::array<__m128, 4> matrixData = matA.DataSSE();
    __m128 rhs = vecRhs.DataSSE();
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



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

[[nodiscard]] inline Vec4fSSE<true> GaussPartialPivot(const Mat4fAVX& matA, const Vec4fSSE<true>& vecRhs)
{
    // INFO: There is no specialized AVX version of this function. During the elimination step, the row multipliers must
    // be transferred across lane boundaries which results in a performance penalty when compared to the SSE version.
    // Therefore, the AVX matrix uses the same function as the SSE matrix.

    static_assert(sizeof(Mat4fAVX) == sizeof(Mat4fSSE), "Internal error - Matrix types have different sizes");
    return GaussPartialPivot(*reinterpret_cast<const Mat4fSSE*>(&matA), vecRhs);
}

#endif // __AVX2__



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot, typename _type>
[[nodiscard]] inline Vec4Serial<_type, true> LU(const Mat4Serial<_type>& matA, const Vec4Serial<_type, true>& vecRhs)
{
    using LUSolver = LUDenseSmallSerial<_type, 4, _pivot>;

    typename LUSolver::Factorization lu = LUSolver::Factorize(matA.Data());

    return Vec4Serial<_type>(LUSolver::Solve(lu, vecRhs.Data()));
}



// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline Vec4fSSE<true> LUNoPivot(const Mat4fSSE& matA, const Vec4fSSE<true>& vecRhs)
{
    using namespace GDL::sse;

    // Factorize
    alignas(alignmentBytes<__m128>) std::array<__m128, 4> LU = matA.DataSSE();


    DEV_EXCEPTION(GetValue<0>(LU[0]) == ApproxZero<F32>(1, 10), "Singular matrix - system not solveable");


    const __m128 zero = _mmx_setzero_p<__m128>();

    __m128 rowMult = BlendIndex<0>(_mmx_div_p(LU[0], Broadcast<0>(LU[0])), zero);
    LU[0] = BlendIndex<0>(rowMult, LU[0]);
    for (U32 i = 1; i < 4; ++i)
    {
        __m128 bc = Broadcast<0>(LU[i]);
        LU[i] = _mmx_fnmadd_p(rowMult, bc, LU[i]);
    }

    //    std::cout << "SSE" << std::endl;
    //    std::cout << Mat4fSSE(LU[0], LU[1], LU[2], LU[3]) << std::endl;

    DEV_EXCEPTION(GetValue<1>(LU[1]) == ApproxZero<F32>(1, 10), "Singular matrix - system not solveable");

    __m128 rowMult1 = BlendAboveIndex<2>(_mmx_div_p(LU[1], Broadcast<1>(LU[1])), zero);
    LU[1] = BlendAboveIndex<2>(rowMult1, LU[1]);
    for (U32 i = 2; i < 4; ++i)
    {
        __m128 bc = Broadcast<1>(LU[i]);
        LU[i] = _mmx_fnmadd_p(rowMult1, bc, LU[i]);
    }

    DEV_EXCEPTION(GetValue<2>(LU[2]) == ApproxZero<F32>(1, 10), "Singular matrix - system not solveable");

    __m128 rowMult2 = BlendAboveIndex<3>(_mmx_div_p(LU[2], Broadcast<2>(LU[2])), zero);
    LU[2] = BlendAboveIndex<3>(rowMult2, LU[2]);
    for (U32 i = 3; i < 4; ++i)
    {
        __m128 bc = Broadcast<2>(LU[i]);
        LU[i] = _mmx_fnmadd_p(rowMult2, bc, LU[i]);
    }


    DEV_EXCEPTION(GetValue<3>(LU[3]) == ApproxZero<F32>(1, 10), "Singular matrix - system not solveable");



    // Solve
    __m128 r = vecRhs.DataSSE();

    // Ly=r
    r = _mmx_fnmadd_p(BlendBelowIndex<0>(zero, LU[0]), Broadcast<0>(r), r);
    r = _mmx_fnmadd_p(BlendBelowIndex<1>(zero, LU[1]), Broadcast<1>(r), r);
    r = _mmx_fnmadd_p(BlendBelowIndex<2>(zero, LU[2]), Broadcast<2>(r), r);


    // Ux=y
    r = BlendIndex<3>(r, _mmx_div_p(r, LU[3]));
    r = _mmx_fnmadd_p(BlendAboveIndex<3>(zero, LU[3]), Broadcast<3>(r), r);
    r = BlendIndex<2>(r, _mmx_div_p(r, LU[2]));
    r = _mmx_fnmadd_p(BlendAboveIndex<2>(zero, LU[2]), Broadcast<2>(r), r);
    r = BlendIndex<1>(r, _mmx_div_p(r, LU[1]));
    r = _mmx_fnmadd_p(BlendAboveIndex<1>(zero, LU[1]), Broadcast<1>(r), r);
    r = BlendIndex<0>(r, _mmx_div_p(r, LU[0]));


    return Vec4fSSE<true>(r);



    //    using namespace GDL::sse;

    //    // Factorize
    //    alignas(alignmentBytes<__m128>) std::array<__m128, 4> LU = matA.DataSSE();


    //    DEV_EXCEPTION(GetValue<0>(LU[0]) == ApproxZero<F32>(1, 10), "Singular matrix - system not solveable");


    //    const __m128 one = _mmx_set1_p<__m128>(1);
    //    const __m128 m1 = _mmx_set1_p<__m128>(-1);
    //    const __m128 zero = _mmx_setzero_p<__m128>();


    //    __m128 bc = Broadcast<0>(LU[0]);
    //    const __m128 rowMult = _mmx_div_p(BlendIndex<0>(LU[0], m1), bc);
    //    LU[0] = BlendIndex<0>(LU[0], Negate(rowMult));

    //    for (U32 i = 1; i < 4; ++i)
    //    {
    //        bc = Broadcast<0>(LU[i]);
    //        LU[i] = _mmx_fnmadd_p(rowMult, bc, BlendIndex<0>(LU[i], zero));
    //    }

    //    DEV_EXCEPTION(GetValue<1>(LU[1]) == ApproxZero<F32>(1, 10), "Singular matrix - system not solveable");

    //    __m128 bc1 = Broadcast<1>(LU[1]);
    //    const __m128 rowMult1 = _mmx_div_p(BlendBelowIndex<1>(_mmx_setr_p<__m128>(0, -1, 0, 0), LU[1]), bc1);
    //    LU[1] = BlendIndex<1>(LU[1], Negate(rowMult1));

    //    for (U32 i = 2; i < 4; ++i)
    //    {
    //        bc1 = Broadcast<1>(LU[i]);
    //        LU[i] = _mmx_fnmadd_p(rowMult1, bc1, BlendIndex<1>(LU[i], zero));
    //    }

    //    DEV_EXCEPTION(GetValue<2>(LU[2]) == ApproxZero<F32>(1, 10), "Singular matrix - system not solveable");

    //    __m128 bc2 = Broadcast<2>(LU[2]);
    //    const __m128 rowMult2 = _mmx_div_p(BlendBelowIndex<2>(_mmx_setr_p<__m128>(0, 0, -1, 0), LU[2]), bc2);
    //    LU[2] = BlendIndex<2>(LU[2], Negate(rowMult2));

    //    for (U32 i = 3; i < 4; ++i)
    //    {
    //        bc2 = Broadcast<2>(LU[i]);
    //        LU[i] = _mmx_fnmadd_p(rowMult2, bc2, BlendIndex<2>(LU[i], zero));
    //    }

    //    DEV_EXCEPTION(GetValue<3>(LU[3]) == ApproxZero<F32>(1, 10), "Singular matrix - system not solveable");

    //    LU[3] = BlendIndex<3>(LU[3], _mmx_div_p(one, LU[3]));



    //    __m128 r = vecRhs.DataSSE();

    //    // Ly=r
    //    //    r[0] =    0  --LU[0] * LU[0] *r[0];
    //    //    r[1] = (r[1] - LU[1] * LU[0] *r[0]) * LU[5];
    //    //    r[2] = (r[2] - LU[2] * LU[0] *r[0] - LU[6] * r[1]) * LU[10];
    //    //    r[3] = (r[3] - LU[3] * LU[0] *r[0] - LU[7] * r[1] - LU[11] * r[2]) * LU[15];

    //    r = BlendIndex<0>(r, _mmx_mul_p(LU[0], r));
    //    r = BlendBelowIndex<0>(r, _mmx_fnmadd_p(LU[0], Broadcast<0>(r), r));

    //    r = BlendIndex<1>(r, _mmx_mul_p(LU[1], r));
    //    r = BlendBelowIndex<1>(r, _mmx_fnmadd_p(LU[1], Broadcast<1>(r), r));

    //    r = BlendIndex<2>(r, _mmx_mul_p(LU[2], r));
    //    r = BlendBelowIndex<2>(r, _mmx_fnmadd_p(LU[2], Broadcast<2>(r), r));

    //    r = BlendIndex<3>(r, _mmx_mul_p(LU[3], r));



    //    // Ux=y
    //    //    r[2] = (r[2] - r[3] * LU[14]);
    //    //    r[1] = (r[1] - r[3] * LU[13] - r[2] * LU[9]);
    //    //    r[0] = (r[0] - r[3] * LU[12] - r[2] * LU[8] - r[1] * LU[4]);

    //    r = BlendAboveIndex<3>(r, _mmx_fnmadd_p(LU[3], Broadcast<3>(r), r));
    //    r = BlendAboveIndex<2>(r, _mmx_fnmadd_p(LU[2], Broadcast<2>(r), r));
    //    r = BlendAboveIndex<1>(r, _mmx_fnmadd_p(LU[1], Broadcast<1>(r), r));

    //    return Vec4fSSE<true>(r);
}


} // namespace GDL::Solver
