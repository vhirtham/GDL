#pragma once

#include "gdl/math/solver/internal/ldltDenseSmall.h"

#include "gdl/base/approx.h"
#include "gdl/base/exception.h"
#include "gdl/base/functions/pow.h"
#include "gdl/base/simd/directAccess.h"
#include "gdl/base/simd/swizzle.h"



namespace GDL::Solver
{

// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
inline LDLTDenseSmallSerial<_type, _size>::Factorization::Factorization(
        const std::array<_type, _size * _size>& matrixData)
    : mLDLT{matrixData}
{
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
[[nodiscard]] inline typename LDLTDenseSmallSerial<_type, _size>::Factorization
LDLTDenseSmallSerial<_type, _size>::Factorize(const std::array<_type, _size * _size>& matrixData)
{
    Factorization factorization(matrixData);
    FactorizeLDLT(factorization);

    return factorization;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
[[nodiscard]] inline std::array<_type, _size>
LDLTDenseSmallSerial<_type, _size>::Solve(const Factorization& factorization, std::array<_type, _size> r)
{

    ForwardSubstitution(factorization.mLDLT, r);
    DiagonalValueDivision(factorization.mLDLT, r);
    BackwardSubstitution(factorization.mLDLT, r);

    return r;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <U32 _idx>
inline void LDLTDenseSmallSerial<_type, _size>::BackwardSubstitution(const std::array<_type, _size * _size>& ldlt,
                                                                     std::array<_type, _size>& r)
{
    for (U32 i = 0; i < _idx; ++i)
        r[i] -= r[_idx] * ldlt[i * _size + _idx];


    if constexpr (_idx > 0)
        BackwardSubstitution<_idx - 1>(ldlt, r);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
inline void LDLTDenseSmallSerial<_type, _size>::DiagonalValueDivision(const std::array<_type, _size * _size>& ldlt,
                                                                      std::array<_type, _size>& r)
{
    r[0] /= ldlt[0];
    r[1] /= ldlt[_size + 1];
    if constexpr (_size >= 3)
        r[2] /= ldlt[(_size + 1) * 2];
    if constexpr (_size >= 4)
        r[3] /= ldlt[(_size + 1) * 3];
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <U32 _idx>
inline void LDLTDenseSmallSerial<_type, _size>::FactorizationStep(Factorization& factorization)
{
    constexpr U32 pivIdx = (_size + 1) * _idx;

    DEV_EXCEPTION(factorization.mLDLT[pivIdx] == ApproxZero<F32>(1, 10),
                  "Can't solve system - Matrix is not symmetric positive definit");


    for (U32 i = _idx; i < _size; ++i)
        for (U32 j = 0; j < _idx; ++j)
            factorization.mLDLT[i + _idx * _size] -= factorization.mLDLT[_idx + j * _size] *
                                                     factorization.mLDLT[i + j * _size] *
                                                     factorization.mLDLT[j * (_size + 1)];


    for (U32 i = pivIdx + 1; i < _size * (_idx + 1); ++i)
        factorization.mLDLT[i] /= factorization.mLDLT[pivIdx];
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <U32 _idx>
inline void LDLTDenseSmallSerial<_type, _size>::FactorizeLDLT(Factorization& factorization)
{
    FactorizationStep<_idx>(factorization);

    if constexpr (_idx + 1 < _size)
        FactorizeLDLT<_idx + 1>(factorization);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <U32 _idx>
inline void LDLTDenseSmallSerial<_type, _size>::ForwardSubstitution(const std::array<_type, _size * _size>& ldlt,
                                                                    std::array<_type, _size>& r)
{

    for (U32 i = _idx + 1; i < _size; ++i)
        r[i] -= ldlt[i + _idx * _size] * r[_idx];

    if constexpr (_idx + 2 < _size)
        ForwardSubstitution<_idx + 1>(ldlt, r);
}



// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// SSE Version
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size>
inline LDLTDenseSmallSSE<_size>::Factorization::Factorization(const std::array<__m128, _size>& matrixData)
    : mLDLT{matrixData}
{
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size>
[[nodiscard]] inline typename LDLTDenseSmallSSE<_size>::Factorization
LDLTDenseSmallSSE<_size>::Factorize(const std::array<__m128, _size>& matrixData)
{
    std::array<__m128, _size> tmp = matrixData;
    Factorization factorization(matrixData);
    FactorizeLDLT(factorization, tmp);

    return factorization;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size>
[[nodiscard]] inline __m128 LDLTDenseSmallSSE<_size>::Solve(const Factorization& factorization, __m128 r)
{
    ForwardSubstitution(factorization.mLDLT, r);
    DiagonalValueDivision(factorization.mDiag, r);
    BackwardSubstitution(factorization.mLDLT, r);

    return r;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size>
inline void LDLTDenseSmallSSE<_size>::AddLT(std::array<__m128, _size>& ldlt)
{
    static_assert(_size == 4, "Unsupported system size.");

    if constexpr (_size == 4)
    {
        using namespace GDL::simd;

        __m128 col1 = Broadcast<1>(ldlt[0]);
        __m128 col2 = _mm_unpackhi_ps(ldlt[0], ldlt[1]);
        __m128 col3 = _mm_movehl_ps(Broadcast<3>(ldlt[2]), col2);

        ldlt[1] = BlendAboveIndex<1>(ldlt[1], col1);
        ldlt[2] = BlendAboveIndex<2>(ldlt[2], col2);
        ldlt[3] = BlendAboveIndex<3>(ldlt[3], col3);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size>
template <U32 _idx>
inline void LDLTDenseSmallSSE<_size>::BackwardSubstitution(const std::array<__m128, _size>& ldlt, __m128& r)
{
    using namespace GDL::simd;

    r = BlendAboveIndex<_idx>(r, _mm_fnmadd(ldlt[_idx], Broadcast<_idx>(r), r));

    if constexpr (_idx > 1)
        BackwardSubstitution<_idx - 1>(ldlt, r);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size>
inline void LDLTDenseSmallSSE<_size>::DiagonalValueDivision(const __m128& diag, __m128& r)
{
    r = _mm_div(r, diag);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size>
template <U32 _idx>
inline void LDLTDenseSmallSSE<_size>::FactorizationStep(Factorization& factorization, std::array<__m128, _size>& tmp)
{
    using namespace GDL::simd;

    DEV_EXCEPTION(GetValue<_idx>(factorization.mLDLT[_idx]) == ApproxZero<F32>(1, 10), "Can't solve system");


    for (U32 i = 0; i < _idx; ++i)
    {
        __m128 bc = Broadcast<_idx>(factorization.mLDLT[i]);
        tmp[_idx] = _mm_fnmadd(tmp[i], bc, tmp[_idx]);
    }


    if constexpr (_idx + 1 < _size)
    {
        __m128 div = _mm_div(tmp[_idx], Broadcast<_idx>(tmp[_idx]));

        factorization.mLDLT[_idx] = BlendIndex<_idx>(div, tmp[_idx]);
    }
    else
        factorization.mLDLT[_idx] = tmp[_idx];
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size>
template <U32 _idx>
inline void LDLTDenseSmallSSE<_size>::FactorizeLDLT(Factorization& factorization, std::array<__m128, _size>& tmp)
{
    FactorizationStep<_idx>(factorization, tmp);

    if constexpr (_idx + 1 < _size)
        FactorizeLDLT<_idx + 1>(factorization, tmp);
    else
    {
        AddLT(factorization.mLDLT);
        StoreDiagonal(factorization);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size>
template <U32 _idx>
inline void LDLTDenseSmallSSE<_size>::ForwardSubstitution(const std::array<__m128, _size>& ldlt, __m128& r)
{
    using namespace GDL::simd;

    r = BlendBelowIndex<_idx>(r, _mm_fnmadd(ldlt[_idx], Broadcast<_idx>(r), r));

    if constexpr (_idx + 2 < _size)
        ForwardSubstitution<_idx + 1>(ldlt, r);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size>
inline void LDLTDenseSmallSSE<_size>::StoreDiagonal(Factorization& factorization)
{
    using namespace GDL::simd;

    static_assert(_size == 3 || _size == 4, "Unsupported system size.");

    if constexpr (_size == 3)
        factorization.mDiag =
                BlendIndex<2>(BlendIndex<1>(factorization.mLDLT[0], factorization.mLDLT[1]), factorization.mLDLT[2]);

    if constexpr (_size == 4)
        factorization.mDiag = BlendBelowIndex<1>(BlendIndex<1>(factorization.mLDLT[0], factorization.mLDLT[1]),
                                                 BlendIndex<3>(factorization.mLDLT[2], factorization.mLDLT[3]));
}

} // namespace GDL::Solver
