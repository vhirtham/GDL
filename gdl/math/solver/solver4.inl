#pragma once

#include "gdl/math/solver/solver4.h"


#include "gdl/base/simd/abs.h"
#include "gdl/base/simd/crossProduct.h"
#include "gdl/base/simd/compareAll.h"
#include "gdl/base/simd/dotProduct.h"
#include "gdl/base/simd/swizzle.h"
#include "gdl/math/solver/internal/GaussDenseSmall.h"
#include "gdl/math/serial/mat4Serial.h"
#include "gdl/math/serial/vec4Serial.h"
#include "gdl/math/simd/mat4fAVX.h"
#include "gdl/math/simd/mat4fSSE.h"
#include "gdl/math/simd/vec4fSSE.h"


namespace GDL::Solver
{



// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
[[nodiscard]] inline Vec4Serial<_type, true> Cramer(const Mat4Serial<_type>& matA,
                                                    const Vec4Serial<_type, true>& vecRhs)
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

[[nodiscard]] inline Vec4fSSE<true> Cramer(const Mat4fSSE& matA, const Vec4fSSE<true>& vecRhs)
{
    using namespace GDL::simd;

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

    __m128 ab03 = _mm_fmsub(a, bP1230, _mm_mul(aP1230, b));
    __m128 cd03 = _mm_fmsub(c, dP1230, _mm_mul(cP1230, d));

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

    __m128 abcd45 = _mm_fmsub(acB0011, bdB1100P2301, _mm_mul(acB1100P2301, bdB0011));
    __m128 abcd45P3210 = Permute<3, 2, 1, 0>(abcd45);

    __m128 products45 = _mm_mul(abcd45, abcd45P3210);
    __m128 sum45 = _mm_add(products45, Permute<1, 0, 3, 2>(products45));


    // Calculate det(A)
    __m128 detA = _mm_add(sum03, sum45);


    DEV_EXCEPTION(_mm_cvtsF(detA) == ApproxZero<F32>(10), "Singular matrix - system not solveable");


    // Calculate the first 4 terms of the modified matrix determinants
    const __m128& r = vecRhs.DataSSE();
    __m128 rP1230 = Permute<1, 2, 3, 0>(r);

    __m128 rb03 = _mm_fmsub(r, bP1230, _mm_mul(rP1230, b));
    __m128 ar03 = _mm_fmsub(a, rP1230, _mm_mul(aP1230, r));
    __m128 rd03 = _mm_fmsub(r, dP1230, _mm_mul(rP1230, d));
    __m128 cr03 = _mm_fmsub(c, rP1230, _mm_mul(cP1230, r));

    __m128 rd03P2301 = Permute<2, 3, 0, 1>(rd03);
    __m128 cr03P2301 = Permute<2, 3, 0, 1>(cr03);

    __m128 rd03P2301N = Negate<0, 1, 0, 1>(rd03P2301);
    __m128 cr03P2301N = Negate<0, 1, 0, 1>(cr03P2301);

    __m128 products03M0 = _mm_mul(rb03, cd03P2301N);
    __m128 products03M1 = _mm_mul(ar03, cd03P2301N);
    __m128 products03M2 = _mm_mul(ab03, rd03P2301N);
    __m128 products03M3 = _mm_mul(ab03, cr03P2301N);


    // Calculate last 2 terms of the modified determinants
    __m128 rP2301 = Permute<2, 3, 0, 1>(r);

    __m128 rbrd45 = _mm_fmsub(r, bdB1100P2301, _mm_mul(rP2301, bdB0011));
    __m128 arcr45 = _mm_fmsub(acB0011, rP2301, _mm_mul(acB1100P2301, r));


    // Calculate modified determinants
    __m128 products03M0M2B0011 = Blend<0, 0, 1, 1>(products03M0, products03M2);
    __m128 products03M0M2B1100 = Blend<1, 1, 0, 0>(products03M0, products03M2);
    __m128 products03M1M3B0011 = Blend<0, 0, 1, 1>(products03M1, products03M3);
    __m128 products03M1M3B1100 = Blend<1, 1, 0, 0>(products03M1, products03M3);

    __m128 products03M0M2B1100P2301 = Permute<2, 3, 0, 1>(products03M0M2B1100);
    __m128 products03M1M3B1100P2301 = Permute<2, 3, 0, 1>(products03M1M3B1100);

    __m128 sumsM0M2 = _mm_fmadd(rbrd45, abcd45P3210, _mm_add(products03M0M2B0011, products03M0M2B1100P2301));
    __m128 sumsM1M3 = _mm_fmadd(arcr45, abcd45P3210, _mm_add(products03M1M3B0011, products03M1M3B1100P2301));

    __m128 sumsB0101 = Blend<0, 1, 0, 1>(sumsM0M2, sumsM1M3);
    __m128 sumsB1010 = Blend<1, 0, 1, 0>(sumsM0M2, sumsM1M3);

    __m128 sumsB1010P1032 = Permute<1, 0, 3, 2>(sumsB1010);

    __m128 determinants = _mm_add(sumsB0101, sumsB1010P1032);


    // Calculate and return solution
    __m128 solution = _mm_div(determinants, detA);

    return Vec4fSSE<true>(solution);
}



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

[[nodiscard]] inline Vec4fSSE<true> Cramer(const Mat4fAVX& matA, const Vec4fSSE<true>& vecRhs)
{
    using namespace GDL::simd;

    const std::array<__m256, 2>& dataA = matA.DataAVX();
    const __m256& ab = dataA[0];
    const __m256& cd = dataA[1];


    // Calculate the components of the first 4 terms of det(a)
    __m256 abP1230 = Permute<1, 2, 3, 0>(ab);
    __m256 cdP1230 = Permute<1, 2, 3, 0>(cd);

    __m256 acbd03 = _mm_fmsub(ab, cdP1230, _mm_mul(cd, abP1230));
    __m256 acbd03N = Negate<0, 0, 0, 0, 1, 0, 1, 0>(acbd03);


    // Calculate the components of the last 2 terms of det(a)
    __m256 abP2323 = Permute<2, 3, 2, 3>(ab);
    __m256 cdP2323 = Permute<2, 3, 2, 3>(cd);
    __m256 acbd45 = _mm_fmsub(ab, cdP2323, _mm_mul(cd, abP2323));


    // Calculate determinant of A
    __m256 ac03bd45 = Blend<0, 0, 0, 0, 1, 1, 1, 1>(acbd03N, acbd45);
    __m256 bd03ac45 = Permute2F128<0, 1, 1, 0>(acbd03N, acbd45);
    __m256 bd03ac45P = Permute<2, 3, 0, 1, 1, 0, 2, 3>(bd03ac45);

    __m256 sums = DotProduct(ac03bd45, bd03ac45P);

    __m256 detA = _mm_add(sums, Permute2F128<1, 0>(sums));


    DEV_EXCEPTION(_mm_cvtsF(detA) == ApproxZero<F32>(10), "Singular matrix - system not solveable");

    const __m256 r = _mm256_insertf128_ps(_mm256_castps128_ps256(vecRhs.DataSSE()), vecRhs.DataSSE(), 1);


    // Calculate first 4 products of all modified matrix determinants
    __m256 rP1230 = Permute<1, 2, 3, 0>(r);

    __m256 rcrd03 = _mm_fmsub(r, cdP1230, _mm_mul(rP1230, cd));
    __m256 arbr03 = _mm_fmsub(ab, rP1230, _mm_mul(r, abP1230));

    __m256 rcrd03N = Negate<0, 0, 0, 0, 1, 0, 1, 0>(rcrd03);
    __m256 arbr03N = Negate<0, 0, 0, 0, 1, 0, 1, 0>(arbr03);

    __m256 bdac03 = Permute2F128<1, 0>(acbd03N);
    __m256 bdac03P2301 = Permute<2, 3, 0, 1>(bdac03);

    __m256 products03M0M1 = _mm_mul(rcrd03N, bdac03P2301);
    __m256 products03M2M3 = _mm_mul(arbr03N, bdac03P2301);
    __m256 products03B0011 = Blend<0, 0, 1, 1, 0, 0, 1, 1>(products03M0M1, products03M2M3);
    __m256 products03B1100 = Blend<1, 1, 0, 0, 1, 1, 0, 0>(products03M0M1, products03M2M3);
    __m256 products03B1100P2301 = Permute<2, 3, 0, 1>(products03B1100);

    __m256 sum03 = _mm_add(products03B0011, products03B1100P2301);


    // Calculate last 2 products of all modified matrix determinants
    __m256 acbdB1100 = Blend<1, 1, 0, 0, 1, 1, 0, 0>(ab, cd);
    __m256 acbdB0011 = Blend<0, 0, 1, 1, 0, 0, 1, 1>(ab, cd);
    __m256 acbdB0011P2301 = Permute<2, 3, 0, 1>(acbdB0011);

    __m256 rP2301 = Permute<2, 3, 0, 1>(r);

    __m256 components45 = _mm_fmsub(acbdB0011P2301, r, _mm_mul(rP2301, acbdB1100));
    __m256 bdac45 = Permute2F128<1, 0>(acbd45);
    __m256 bdac45P1010 = Permute<1, 0, 1, 0>(bdac45);

    __m256 products45M = _mm_mul(components45, bdac45P1010);


    // Calculate and return solution
    __m256 sumsM = _mm_add(sum03, products45M);
    __m256 determinants00221133 = _mm_add(sumsM, Permute<1, 0, 3, 2>(sumsM));
    __m256 determinants11330022 = Permute2F128<1, 0>(determinants00221133);
    __m256 determinants = Blend<0, 1, 0, 1, 1, 0, 1, 0>(determinants00221133, determinants11330022);

    __m256 solution = _mm_div(determinants, detA);

    return Vec4fSSE<true>(_mm256_castps256_ps128(solution));
}

#endif // __AVX2__



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot, typename _type>
[[nodiscard]] inline Vec4Serial<_type, true> Gauss(const Mat4Serial<_type>& matA, const Vec4Serial<_type, true>& vecRhs)
{
    std::array<_type, 16> matrixData = matA.Data();
    std::array<_type, 4> vectorData = vecRhs.Data();

    // INFO: Tried using template recursion here, but it seems to prevent clang from doing some optimizations ---> runs
    // slower

    GaussDenseSmallSerial<_type, 4, _pivot>::template GaussStep<0>(matrixData, vectorData);
    GaussDenseSmallSerial<_type, 4, _pivot>::template GaussStep<1>(matrixData, vectorData);
    GaussDenseSmallSerial<_type, 4, _pivot>::template GaussStep<2>(matrixData, vectorData);
    GaussDenseSmallSerial<_type, 4, _pivot>::template GaussStep<3>(matrixData, vectorData);

    return Vec4Serial<_type, true>(vectorData);
}



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot>
[[nodiscard]] inline Vec4fSSE<true> Gauss(const Mat4fSSE& matA, const Vec4fSSE<true>& vecRhs)
{
    using namespace GDL::simd;

    alignas(alignmentBytes<__m128>) std::array<__m128, 4> matrixData = matA.DataSSE();
    __m128 vectorData = vecRhs.DataSSE();

    GaussDenseSmallSSE<4, _pivot>::template GaussStep<0>(matrixData, vectorData);
    GaussDenseSmallSSE<4, _pivot>::template GaussStep<1>(matrixData, vectorData);
    GaussDenseSmallSSE<4, _pivot>::template GaussStep<2>(matrixData, vectorData);
    GaussDenseSmallSSE<4, _pivot>::template GaussStep<3>(matrixData, vectorData);

    return Vec4fSSE<true>(vectorData);
}



// --------------------------------------------------------------------------------------------------------------------

#ifdef __AVX2__

template <Pivot _pivot>
[[nodiscard]] inline Vec4fSSE<true> Gauss(const Mat4fAVX& matA, const Vec4fSSE<true>& vecRhs)
{
    // INFO: There is no specialized AVX version of this function. During the elimination step, the row multipliers must
    // be transferred across lane boundaries which results in a performance penalty when compared to the SSE version.
    // Therefore, the AVX matrix uses the same function as the SSE matrix.

    static_assert(sizeof(Mat4fAVX) == sizeof(Mat4fSSE), "Internal error - Matrix types have different sizes");
    return Gauss<_pivot>(*reinterpret_cast<const Mat4fSSE*>(&matA), vecRhs);
}

#endif // __AVX2__



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot, typename _type>
[[nodiscard]] inline Vec4Serial<_type, true> LU(const Mat4Serial<_type>& matA, const Vec4Serial<_type, true>& vecRhs)
{
    using LUSolver = LUDenseSmallSerial<_type, 4, _pivot>;

    typename LUSolver::Factorization factorization = LUFactorization<_pivot>(matA);

    return LU<_pivot>(factorization, vecRhs);
}



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot, typename _type>
[[nodiscard]] inline Vec4Serial<_type, true>
LU(const typename LUDenseSmallSerial<_type, 4, _pivot>::Factorization& factorization,
   const Vec4Serial<_type, true>& vecRhs)
{
    using LUSolver = LUDenseSmallSerial<_type, 4, _pivot>;

    return Vec4Serial<_type>(LUSolver::Solve(factorization, vecRhs.Data()));
}



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot, typename _type>
[[nodiscard]] typename LUDenseSmallSerial<_type, 4, _pivot>::Factorization
LUFactorization(const Mat4Serial<_type>& matA)
{
    using LUSolver = LUDenseSmallSerial<_type, 4, _pivot>;

    return LUSolver::Factorize(matA.Data());
}



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot>
[[nodiscard]] inline Vec4fSSE<true> LU(const Mat4fSSE& matA, const Vec4fSSE<true>& vecRhs)
{
    using LUSolver = LUDenseSmallSSE<4, _pivot>;

    typename LUSolver::Factorization factorization = LUFactorization<_pivot>(matA);

    return LU<_pivot>(factorization, vecRhs);
}



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot>
[[nodiscard]] inline Vec4fSSE<true> LU(const typename LUDenseSmallSSE<4, _pivot>::Factorization& factorization,
                                       const Vec4fSSE<true>& vecRhs)
{
    using LUSolver = LUDenseSmallSSE<4, _pivot>;

    return Vec4fSSE<true>(LUSolver::Solve(factorization, vecRhs.DataSSE()));
}



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot>
[[nodiscard]] inline typename LUDenseSmallSSE<4, _pivot>::Factorization LUFactorization(const Mat4fSSE& matA)
{
    using LUSolver = LUDenseSmallSSE<4, _pivot>;

    return LUSolver::Factorize(matA.DataSSE());
}



} // namespace GDL::Solver
