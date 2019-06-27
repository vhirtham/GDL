#pragma once

#include "gdl/math/solver/internal/luDenseSmall.h"

#include "gdl/base/approx.h"
#include "gdl/base/exception.h"
#include "gdl/base/sse/directAccess.h"
#include "gdl/base/sse/swizzle.h"
#include "gdl/math/solver/internal/pivotDenseSmall.h"


namespace GDL::Solver
{

// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
inline LUDenseSmallSerial<_type, _size, _pivot>::Factorization::Factorization(
        const std::array<_type, _size * _size>& matrixData)
    : mLU{matrixData}
    , mPermutation{InitializePermutation()}
{
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
inline constexpr std::array<_type, _size>
LUDenseSmallSerial<_type, _size, _pivot>::Factorization::InitializePermutation() const
{
    static_assert(_size == 3 || _size == 4, "Unsupported array size");
    if constexpr (_size == 3)
        return {{0, 1, 2}};
    if constexpr (_size == 4)
        return {{0, 1, 2, 3}};
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
[[nodiscard]] inline typename LUDenseSmallSerial<_type, _size, _pivot>::Factorization
LUDenseSmallSerial<_type, _size, _pivot>::Factorize(const std::array<_type, _size * _size>& matrixData)
{
    Factorization factorization(matrixData);
    FactorizeLU(factorization);

    return factorization;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
[[nodiscard]] inline std::array<_type, _size>
LUDenseSmallSerial<_type, _size, _pivot>::Solve(const Factorization& factorization, std::array<_type, _size> r)
{
    if constexpr (_pivot != Pivot::NONE)
        r = PivotDenseSmallSerial<_type, _size>::PermuteVector(r, factorization.mPermutation);

    ForwardSubstitution(factorization.mLU, r);
    BackwardSubstitution(factorization.mLU, r);

    return r;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
template <U32 _idx>
inline void LUDenseSmallSerial<_type, _size, _pivot>::BackwardSubstitution(const std::array<_type, _size * _size>& lu,
                                                                           std::array<_type, _size>& r)
{
    constexpr U32 pivIdx = (_size + 1) * _idx;

    r[_idx] /= lu[pivIdx];

    for (U32 i = 0; i < _idx; ++i)
        r[i] -= r[_idx] * lu[i + _idx * _size];

    if constexpr (_idx > 0)
        BackwardSubstitution<_idx - 1>(lu, r);

    //    4x4 system:
    //    r[3] /= LU[15];
    //    r[2] = (r[2] - r[3] * LU[14]) / LU[10];
    //    r[1] = (r[1] - r[3] * LU[13] - r[2] * LU[9]) / LU[5];
    //    r[0] = (r[0] - r[3] * LU[12] - r[2] * LU[8] - r[1] * LU[4]) / LU[0];


    // Alternative version:

    //    for (U32 i = 0; i < _idx; ++i)
    //        r[i] -= r[_idx] * lu[i + _idx * _size];

    //    if constexpr (_idx > 1)
    //        BackwardSubstitution<_idx - 1>(lu, r);

    //    //    4x4 system:
    //    //    r[2] = (r[2] - r[3] * LU[14]);
    //    //    r[1] = (r[1] - r[3] * LU[13] - r[2] * LU[9]);
    //    //    r[0] = (r[0] - r[3] * LU[12] - r[2] * LU[8] - r[1] * LU[4]);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
template <U32 _idx>
inline void LUDenseSmallSerial<_type, _size, _pivot>::FactorizeLU(Factorization& factorization)
{
    if constexpr (_pivot != Pivot::NONE && _idx + 1 < _size)
        PivotDenseSmallSerial<_type, _size>::template PivotStep<_pivot, _idx>(factorization.mLU,
                                                                              factorization.mPermutation);

    FactorizationStep<_idx>(factorization);

    if constexpr (_idx + 1 < _size)
        FactorizeLU<_idx + 1>(factorization);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
template <U32 _idx>
inline void LUDenseSmallSerial<_type, _size, _pivot>::FactorizationStep(Factorization& factorization)
{
    constexpr U32 pivIdx = _idx * _size + _idx;
    DEV_EXCEPTION(factorization.mLU[pivIdx] == ApproxZero<_type>(1, 10), "Singular matrix - system not solveable");


    for (U32 i = pivIdx + 1; i < _size * (_idx + 1); ++i)
    {
        factorization.mLU[i] /= factorization.mLU[pivIdx];
        for (U32 j = _size; j < (_size - _idx) * _size; j += _size)
            factorization.mLU[i + j] -= factorization.mLU[i] * factorization.mLU[pivIdx + j];
    }


    // Alternative version:

    //    constexpr U32 pivIdx = _idx * _size + _idx;
    //    DEV_EXCEPTION(lu[pivIdx] == ApproxZero<_type>(1, 10), "Singular matrix - system not solveable");


    //    lu[pivIdx] = 1 / lu[pivIdx];


    //    for (U32 i = pivIdx + _size; i < _size * _size; i += _size)
    //        lu[i] *= lu[pivIdx];

    //    for (U32 i = pivIdx + 1; i < _size * (_idx + 1); ++i)
    //        for (U32 j = _size; j < (_size - _idx) * _size; j += _size)
    //            lu[i + j] -= lu[i] * lu[pivIdx + j];
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size, Pivot _pivot>
template <U32 _idx>
inline void LUDenseSmallSerial<_type, _size, _pivot>::ForwardSubstitution(const std::array<_type, _size * _size>& lu,
                                                                          std::array<_type, _size>& r)
{
    for (U32 i = _idx + 1; i < _size; ++i)
        r[i] -= lu[i + _idx * _size] * r[_idx];

    if constexpr (_idx + 2 < _size)
        ForwardSubstitution<_idx + 1>(lu, r);

    //    4x4 system:
    //    r[1] -= LU[1] * r[0];
    //    r[2] -= LU[2] * r[0] + LU[6] * r[1];
    //    r[3] -= LU[3] * r[0] + LU[7] * r[1] + LU[11] * r[2];



    // Alternative version:

    //    constexpr U32 pivIdx = (_size + 1) * _idx;

    //    r[_idx] *= lu[pivIdx];
    //    for (U32 i = _idx + 1; i < _size; ++i)
    //        r[i] -= lu[i + _idx * _size] * r[_idx];

    //    if constexpr (_idx + 1 < _size)
    //        ForwardSubstitution<_idx + 1>(lu, r);

    //    //    4x4 system:
    //    //    r[0] = LU[0] * r[0];
    //    //    r[1] = (r[1] - LU[1] * r[0]) * LU[5];
    //    //    r[2] = (r[2] - LU[2] * r[0] - LU[6] * r[1]) * LU[10];
    //    //    r[3] = (r[3] - LU[3] * r[0] - LU[7] * r[1] - LU[11] * r[2]) * LU[15];
}



// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// SSE Version
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size, Pivot _pivot>
inline LUDenseSmallSSE<_size, _pivot>::Factorization::Factorization(const std::array<__m128, _size>& lu)
    : mLU{lu}
{
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size, Pivot _pivot>
[[nodiscard]] inline typename LUDenseSmallSSE<_size, _pivot>::Factorization
LUDenseSmallSSE<_size, _pivot>::Factorize(const std::array<__m128, _size>& matrixData)
{

    __m128 permutation = _mmx_setr_p<__m128>(0, 1, 2, 3);

    Factorization factorization(matrixData);
    FactorizeLU(factorization, permutation);

    if constexpr (_pivot != Pivot::NONE)
        factorization.mPermutationHash = PivotDenseSmallSSE<_size>::CreatePermutationHash(permutation);

    return factorization;
} // namespace GDL::Solver



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size, Pivot _pivot>
[[nodiscard]] inline __m128 LUDenseSmallSSE<_size, _pivot>::Solve(const Factorization& factorization, __m128 r)
{
    if constexpr (_pivot != Pivot::NONE)
        r = PivotDenseSmallSSE<_size>::PermuteVector(r, factorization.mPermutationHash);

    ForwardSubstitution(factorization.mLU, r);
    BackwardSubstitution(factorization.mLU, r);

    return r;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size, Pivot _pivot>
template <U32 _idx>
inline void LUDenseSmallSSE<_size, _pivot>::BackwardSubstitution(const std::array<__m128, _size>& lu, __m128& r)
{
    using namespace GDL::simd;

    const __m128 zero = _mmx_setzero_p<__m128>();

    r = BlendIndex<_idx>(r, _mmx_div_p(r, lu[_idx]));
    r = _mmx_fnmadd_p(BlendAboveIndex<_idx>(zero, lu[_idx]), Broadcast<_idx>(r), r);

    if constexpr (_idx > 0)
        BackwardSubstitution<_idx - 1>(lu, r);



    //    // Alternative version:

    //    using namespace GDL::simd;

    //    r = BlendAboveIndex<_idx>(r, _mmx_fnmadd_p(lu[_idx], Broadcast<_idx>(r), r));

    //    if constexpr (_idx > 1)
    //        BackwardSubstitution<_idx - 1>(lu, r);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size, Pivot _pivot>
template <U32 _idx>
inline void LUDenseSmallSSE<_size, _pivot>::FactorizationStep(Factorization& factorization)
{
    using namespace GDL::simd;

    DEV_EXCEPTION(GetValue<_idx>(factorization.mLU[_idx]) == ApproxZero<F32>(1, 10),
                  "Singular matrix - system not solveable");

    if constexpr (_idx + 1 < _size)
    {
        const __m128 zero = _mmx_setzero_p<__m128>();

        __m128 rowMult = BlendBelowIndex<_idx>(
                zero, _mmx_div_p(factorization.mLU[_idx], Broadcast<_idx>(factorization.mLU[_idx])));
        factorization.mLU[_idx] = BlendBelowIndex<_idx>(factorization.mLU[_idx], rowMult);
        for (U32 i = _idx + 1; i < _size; ++i)
        {
            __m128 bc = Broadcast<_idx>(factorization.mLU[i]);
            factorization.mLU[i] = _mmx_fnmadd_p(rowMult, bc, factorization.mLU[i]);
        }
    }



    //    // Alternative version:

    //    using namespace GDL::simd;

    //    DEV_EXCEPTION(GetValue<_idx>(factorization.mLU[_idx]) == ApproxZero<F32>(1, 10),
    //                  "Singular matrix - system not solveable");


    //    constexpr F32 m10 = (_idx == 0) ? -1 : 0;
    //    constexpr F32 m11 = (_idx == 1) ? -1 : 0;
    //    constexpr F32 m12 = (_idx == 2) ? -1 : 0;
    //    constexpr F32 m13 = (_idx == 3) ? -1 : 0;

    //    const __m128 m1 = _mmx_setr_p<__m128>(m10, m11, m12, m13);
    //    const __m128 zero = _mmx_setzero_p<__m128>();

    //    __m128 bc = Broadcast<_idx>(factorization.mLU[_idx]);
    //    const __m128 rowMult = _mmx_div_p(BlendBelowIndex<_idx>(m1, factorization.mLU[_idx]), bc);
    //    factorization.mLU[_idx] = BlendIndex<_idx>(factorization.mLU[_idx], Negate(rowMult));

    //    for (U32 i = _idx + 1; i < _size; ++i)
    //    {
    //        bc = Broadcast<_idx>(factorization.mLU[i]);
    //        factorization.mLU[i] = _mmx_fnmadd_p(rowMult, bc, BlendIndex<_idx>(factorization.mLU[i], zero));
    //    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size, Pivot _pivot>
template <U32 _idx>
inline void LUDenseSmallSSE<_size, _pivot>::FactorizeLU(Factorization& factorization, __m128& permutation)
{
    if constexpr (_pivot != Pivot::NONE && _idx + 1 < _size)
        PivotDenseSmallSSE<_size>::template PivotStep<_pivot, _idx>(factorization.mLU, permutation);

    FactorizationStep<_idx>(factorization);

    if constexpr (_idx + 1 < _size)
        FactorizeLU<_idx + 1>(factorization, permutation);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size, Pivot _pivot>
template <U32 _idx>
inline void LUDenseSmallSSE<_size, _pivot>::ForwardSubstitution(const std::array<__m128, _size>& lu, __m128& r)
{
    using namespace GDL::simd;

    const __m128 zero = _mmx_setzero_p<__m128>();

    r = _mmx_fnmadd_p(BlendBelowIndex<_idx>(zero, lu[_idx]), Broadcast<_idx>(r), r);

    if constexpr (_idx + 2 < _size)
        ForwardSubstitution<_idx + 1>(lu, r);



    //    // Alternative version:

    //    using namespace GDL::simd;

    //    r = BlendIndex<_idx>(r, _mmx_mul_p(lu[_idx], r));
    //    r = BlendBelowIndex<_idx>(r, _mmx_fnmadd_p(lu[_idx], Broadcast<_idx>(r), r));

    //    if constexpr (_idx + 1 < _size)
    //        ForwardSubstitution<_idx + 1>(lu, r);
}



} // namespace GDL::Solver
