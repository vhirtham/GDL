#pragma once

#include "gdl/math/solver/solver3.h"

#include "gdl/base/approx.h"
#include "gdl/base/simd/abs.h"
#include "gdl/base/simd/crossProduct.h"
#include "gdl/base/simd/determinant.h"
#include "gdl/base/simd/directAccess.h"
#include "gdl/base/simd/dotProduct.h"
#include "gdl/base/simd/swizzle.h"
#include "gdl/math/solver/internal/GaussDenseSmall.h"
#include "gdl/math/serial/mat3Serial.h"
#include "gdl/math/serial/vec3Serial.h"
#include "gdl/math/simd/mat3fSSE.h"


namespace GDL::Solver
{

// --------------------------------------------------------------------------------------------------------------------
// Cramers Rule
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

    std::array<_type, 3> solution = {{(r[0] * crossbc[0] + r[1] * crossbc[1] + r[2] * crossbc[2]) / detA,
                                      (a[0] * crossrc[0] + a[1] * crossrc[1] + a[2] * crossrc[2]) / detA,
                                      (a[0] * crossbr[0] + a[1] * crossbr[1] + a[2] * crossbr[2]) / detA}};

    return Vec3Serial<_type, true>(solution);
}



// --------------------------------------------------------------------------------------------------------------------

inline Vec3fSSE<true> Cramer(const Mat3fSSE& matA, const Vec3fSSE<true>& vecRhs)
{
    using namespace GDL::simd;

    const std::array<__m128, 3>& dataA = matA.DataSSE();
    const __m128& a = dataA[0];
    const __m128& b = dataA[1];
    const __m128& c = dataA[2];

    const __m128& r = vecRhs.DataSSE();

    __m128 crossbc = CrossProduct(b, c);
    __m128 detA = DotProduct<1, 1, 1, 0>(a, crossbc);

    DEV_EXCEPTION(_mm_cvtsF(detA) == ApproxZero<F32>(10), "Singular matrix - system not solveable");

    __m128 crossrc = CrossProduct(r, c);
    __m128 crossbr = CrossProduct(b, r);

    __m128 determinants = DotProduct<1, 1, 1, 0>(r, crossbc);
    determinants = Blend<0, 1, 0, 0>(determinants, DotProduct<1, 1, 1, 0>(a, crossrc));
    determinants = Blend<0, 0, 1, 0>(determinants, DotProduct<1, 1, 1, 0>(a, crossbr));

    __m128 solution = _mm_div(determinants, detA);

    return Vec3fSSE<true>(solution);
}



// --------------------------------------------------------------------------------------------------------------------
// Gaussian Elimination
// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot, typename _type>
inline Vec3Serial<_type, true> Gauss(const Mat3Serial<_type>& matA, const Vec3Serial<_type, true>& vecRhs)
{
    std::array<_type, 9> matrixData = matA.Data();
    std::array<_type, 3> vectorData = vecRhs.Data();

    // INFO: Tried using template recursion here, but it seems to prevent clang from doing some optimizations ---> runs
    // slower

    GaussDenseSmallSerial<_type, 3, _pivot>::template GaussStep<0>(matrixData, vectorData);
    GaussDenseSmallSerial<_type, 3, _pivot>::template GaussStep<1>(matrixData, vectorData);
    GaussDenseSmallSerial<_type, 3, _pivot>::template GaussStep<2>(matrixData, vectorData);

    return Vec3Serial<_type, true>(vectorData);
}



// --------------------------------------------------------------------------------------------------------------------


template <Pivot _pivot>
inline Vec3fSSE<true> Gauss(const Mat3fSSE& matA, const Vec3fSSE<true>& vecRhs)
{
    using namespace GDL::simd;

    alignas(alignmentBytes<__m128>) std::array<__m128, 3> matrixData = matA.DataSSE();
    __m128 vectorData = vecRhs.DataSSE();

    GaussDenseSmallSSE<3, _pivot>::template GaussStep<0>(matrixData, vectorData);
    GaussDenseSmallSSE<3, _pivot>::template GaussStep<1>(matrixData, vectorData);
    GaussDenseSmallSSE<3, _pivot>::template GaussStep<2>(matrixData, vectorData);

    return Vec3fSSE<true>(vectorData);
}



// --------------------------------------------------------------------------------------------------------------------
// LDLT decomposition
// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
[[nodiscard]] inline Vec3Serial<_type, true> LDLT(const Mat3Serial<_type>& matA, const Vec3Serial<_type, true>& vecRhs)
{
    using LDLTSolver = LDLTDenseSmallSerial<_type, 3>;

    typename LDLTSolver::Factorization factorization = LDLTFactorization<_type>(matA);

    return LDLT<_type>(factorization, vecRhs);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
[[nodiscard]] inline Vec3Serial<_type, true>
LDLT(const typename LDLTDenseSmallSerial<_type, 3>::Factorization& factorization, const Vec3Serial<_type, true>& vecRhs)
{
    using LDLTSolver = LDLTDenseSmallSerial<_type, 3>;

    return Vec3Serial<_type, true>(LDLTSolver::Solve(factorization, vecRhs.Data()));
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
[[nodiscard]] inline typename LDLTDenseSmallSerial<_type, 3>::Factorization
LDLTFactorization(const Mat3Serial<_type>& matA)
{
    using LDLTSolver = LDLTDenseSmallSerial<_type, 3>;

    return LDLTSolver::Factorize(matA.Data());
}



// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline Vec3fSSE<true> LDLT(const Mat3fSSE& matA, const Vec3fSSE<true>& vecRhs)
{
    using LDLTSolver = LDLTDenseSmallSSE<3>;

    typename LDLTSolver::Factorization factorization = LDLTFactorization(matA);

    return LDLT(factorization, vecRhs);
}



// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline Vec3fSSE<true> LDLT(const typename LDLTDenseSmallSSE<3>::Factorization& factorization,
                                         const Vec3fSSE<true>& vecRhs)
{
    using LLTSolver = LDLTDenseSmallSSE<3>;

    return Vec3fSSE<true>(LLTSolver::Solve(factorization, vecRhs.DataSSE()));
}



// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline typename LDLTDenseSmallSSE<3>::Factorization LDLTFactorization(const Mat3fSSE& matA)
{
    using LDLTSolver = LDLTDenseSmallSSE<3>;

    return LDLTSolver::Factorize(matA.DataSSE());
}



// --------------------------------------------------------------------------------------------------------------------
// LLT decomposition
// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
[[nodiscard]] inline Vec3Serial<_type, true> LLT(const Mat3Serial<_type>& matA, const Vec3Serial<_type, true>& vecRhs)
{
    using LLTSolver = LLTDenseSmallSerial<_type, 3>;

    typename LLTSolver::Factorization factorization = LLTFactorization<_type>(matA);
    return LLT<_type>(factorization, vecRhs);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
[[nodiscard]] inline Vec3Serial<_type, true>
LLT(const typename LLTDenseSmallSerial<_type, 3>::Factorization& factorization, const Vec3Serial<_type, true>& vecRhs)
{
    using LLTSolver = LLTDenseSmallSerial<_type, 3>;

    return Vec3Serial<_type, true>(LLTSolver::Solve(factorization, vecRhs.Data()));
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type>
[[nodiscard]] inline typename LLTDenseSmallSerial<_type, 3>::Factorization
LLTFactorization(const Mat3Serial<_type>& matA)
{
    using LLTSolver = LLTDenseSmallSerial<_type, 3>;

    return LLTSolver::Factorize(matA.Data());
}



// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline Vec3fSSE<true> LLT(const Mat3fSSE& matA, const Vec3fSSE<true>& vecRhs)
{
    using LLTSolver = LLTDenseSmallSSE<3>;

    typename LLTSolver::Factorization factorization = LLTFactorization(matA);

    return LLT(factorization, vecRhs);
}



// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline Vec3fSSE<true> LLT(const typename LLTDenseSmallSSE<3>::Factorization& factorization,
                                        const Vec3fSSE<true>& vecRhs)
{
    using LLTSolver = LLTDenseSmallSSE<3>;

    return Vec3fSSE<true>(LLTSolver::Solve(factorization, vecRhs.DataSSE()));
}



// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline typename LLTDenseSmallSSE<3>::Factorization LLTFactorization(const Mat3fSSE& matA)
{
    using LLTSolver = LLTDenseSmallSSE<3>;

    return LLTSolver::Factorize(matA.DataSSE());
}



// --------------------------------------------------------------------------------------------------------------------
// LU decomposition
// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot, typename _type>
[[nodiscard]] inline Vec3Serial<_type, true> LU(const Mat3Serial<_type>& matA, const Vec3Serial<_type, true>& vecRhs)
{
    using LUSolver = LUDenseSmallSerial<_type, 3, _pivot>;

    typename LUSolver::Factorization factorization = LUFactorization<_pivot>(matA);

    return LU<_pivot>(factorization, vecRhs);
}



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot, typename _type>
[[nodiscard]] inline Vec3Serial<_type, true>
LU(const typename LUDenseSmallSerial<_type, 3, _pivot>::Factorization& factorization,
   const Vec3Serial<_type, true>& vecRhs)
{
    using LUSolver = LUDenseSmallSerial<_type, 3, _pivot>;

    return Vec3Serial<_type>(LUSolver::Solve(factorization, vecRhs.Data()));
}



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot, typename _type>
[[nodiscard]] typename LUDenseSmallSerial<_type, 3, _pivot>::Factorization
LUFactorization(const Mat3Serial<_type>& matA)
{
    using LUSolver = LUDenseSmallSerial<_type, 3, _pivot>;

    return LUSolver::Factorize(matA.Data());
}



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot>
[[nodiscard]] inline Vec3fSSE<true> LU(const Mat3fSSE& matA, const Vec3fSSE<true>& vecRhs)
{
    using LUSolver = LUDenseSmallSSE<3, _pivot>;

    typename LUSolver::Factorization factorization = LUFactorization<_pivot>(matA);

    return LU<_pivot>(factorization, vecRhs);
}



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot>
[[nodiscard]] inline Vec3fSSE<true> LU(const typename LUDenseSmallSSE<3, _pivot>::Factorization& factorization,
                                       const Vec3fSSE<true>& vecRhs)
{
    using LUSolver = LUDenseSmallSSE<3, _pivot>;

    return Vec3fSSE<true>(LUSolver::Solve(factorization, vecRhs.DataSSE()));
}



// --------------------------------------------------------------------------------------------------------------------

template <Pivot _pivot>
[[nodiscard]] inline typename LUDenseSmallSSE<3, _pivot>::Factorization LUFactorization(const Mat3fSSE& matA)
{
    using LUSolver = LUDenseSmallSSE<3, _pivot>;

    return LUSolver::Factorize(matA.DataSSE());
}



} // namespace GDL::Solver
