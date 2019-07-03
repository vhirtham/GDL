#pragma once

#include "gdl/math/solver/internal/lltDenseSmall.h"

#include "gdl/base/approx.h"
#include "gdl/base/exception.h"
#include "gdl/base/functions/pow.h"
#include "gdl/base/simd/directAccess.h"
#include "gdl/base/simd/swizzle.h"



namespace GDL::Solver
{

// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
inline LLTDenseSmallSerial<_type, _size>::Factorization::Factorization(
        const std::array<_type, _size * _size>& matrixData)
    : mLLT{matrixData}
{
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
[[nodiscard]] inline typename LLTDenseSmallSerial<_type, _size>::Factorization
LLTDenseSmallSerial<_type, _size>::Factorize(const std::array<_type, _size * _size>& matrixData)
{
    Factorization factorization(matrixData);
    FactorizeLLT(factorization);

    return factorization;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
[[nodiscard]] inline std::array<_type, _size>
LLTDenseSmallSerial<_type, _size>::Solve(const Factorization& factorization, std::array<_type, _size> r)
{

    ForwardSubstitution(factorization.mLLT, r);
    BackwardSubstitution(factorization.mLLT, r);

    return r;
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <U32 _idx>
inline void LLTDenseSmallSerial<_type, _size>::BackwardSubstitution(const std::array<_type, _size * _size>& llt,
                                                                    std::array<_type, _size>& r)
{
    constexpr U32 pivIdx = (_size + 1) * _idx;

    r[_idx] /= llt[pivIdx];

    for (U32 i = 0; i < _idx; ++i)
        r[i] -= r[_idx] * llt[i * _size + _idx];

    if constexpr (_idx > 0)
        BackwardSubstitution<_idx - 1>(llt, r);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <U32 _idx>
inline void LLTDenseSmallSerial<_type, _size>::FactorizationStep(Factorization& factorization)
{
    constexpr U32 pivIdx = (_size + 1) * _idx;

    DEV_EXCEPTION(factorization.mLLT[pivIdx] == ApproxZero<F32>(1, 10),
                  "Can't solve system - Matrix is not symmetric positive definit");


    for (U32 i = _idx; i < _size; ++i)
        for (U32 j = 0; j < _idx; ++j)
            factorization.mLLT[i + _idx * _size] -=
                    factorization.mLLT[_idx + j * _size] * factorization.mLLT[i + j * _size];

    factorization.mLLT[pivIdx] = sqrt(factorization.mLLT[pivIdx]);

    for (U32 i = pivIdx + 1; i < _size * (_idx + 1); ++i)
        factorization.mLLT[i] /= factorization.mLLT[pivIdx];
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <U32 _idx>
inline void LLTDenseSmallSerial<_type, _size>::FactorizeLLT(Factorization& factorization)
{
    FactorizationStep<_idx>(factorization);

    if constexpr (_idx + 1 < _size)
        FactorizeLLT<_idx + 1>(factorization);
}



// --------------------------------------------------------------------------------------------------------------------

template <typename _type, U32 _size>
template <U32 _idx>
inline void LLTDenseSmallSerial<_type, _size>::ForwardSubstitution(const std::array<_type, _size * _size>& llt,
                                                                   std::array<_type, _size>& r)
{

    constexpr U32 pivIdx = (_size + 1) * _idx;

    r[_idx] /= llt[pivIdx];
    for (U32 i = _idx + 1; i < _size; ++i)
        r[i] -= llt[i + _idx * _size] * r[_idx];

    if constexpr (_idx + 1 < _size)
        ForwardSubstitution<_idx + 1>(llt, r);
}



// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// SSE Version
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size>
inline LLTDenseSmallSSE<_size>::Factorization::Factorization(const std::array<__m128, _size>& matrixData)
    : mLLT{matrixData}
{
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size>
[[nodiscard]] inline typename LLTDenseSmallSSE<_size>::Factorization
LLTDenseSmallSSE<_size>::Factorize(const std::array<__m128, _size>& matrixData)
{
    Factorization factorization(matrixData);
    FactorizeLLT(factorization);

    return factorization;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size>
[[nodiscard]] inline __m128 LLTDenseSmallSSE<_size>::Solve(const Factorization& factorization, __m128 r)
{
    ForwardSubstitution(factorization.mLLT, r);
    BackwardSubstitution(factorization.mLLT, r);

    return r;
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size>
inline void LLTDenseSmallSSE<_size>::AddLT(std::array<__m128, _size>& llt)
{
    static_assert(_size == 4, "Unsupported system size.");

    if constexpr (_size == 4)
    {
        using namespace GDL::simd;

        __m128 col1 = Broadcast<1>(llt[0]);
        __m128 col2 = _mm_unpackhi_ps(llt[0], llt[1]);
        __m128 col3 = _mm_movehl_ps(Broadcast<3>(llt[2]), col2);

        llt[1] = BlendAboveIndex<1>(llt[1], col1);
        llt[2] = BlendAboveIndex<2>(llt[2], col2);
        llt[3] = BlendAboveIndex<3>(llt[3], col3);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size>
template <U32 _idx>
inline void LLTDenseSmallSSE<_size>::BackwardSubstitution(const std::array<__m128, _size>& llt, __m128& r)
{

    using namespace GDL::simd;

    r = BlendIndex<_idx>(r, _mm_div(r, llt[_idx]));
    r = BlendAboveIndex<_idx>(r, _mm_fnmadd(llt[_idx], Broadcast<_idx>(r), r));

    if constexpr (_idx > 0)
        BackwardSubstitution<_idx - 1>(llt, r);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size>
template <U32 _idx>
inline void LLTDenseSmallSSE<_size>::FactorizationStep(Factorization& factorization)
{
    using namespace GDL::simd;

    DEV_EXCEPTION(GetValue<_idx>(factorization.mLLT[_idx]) == ApproxZero<F32>(1, 10),
                  "Can't solve system - Matrix is not symmetric positive definit");


    for (U32 i = 0; i < _idx; ++i)
    {
        __m128 bc = Broadcast<_idx>(factorization.mLLT[i]);
        factorization.mLLT[_idx] = _mm_fnmadd(factorization.mLLT[i], bc, factorization.mLLT[_idx]);
    }

    __m128 idxSqrt = _mm_sqrt(factorization.mLLT[_idx]);

    factorization.mLLT[_idx] = BlendIndex<_idx>(factorization.mLLT[_idx], idxSqrt);

    if constexpr (_idx + 1 < _size)
    {
        __m128 div = _mm_div(factorization.mLLT[_idx], Broadcast<_idx>(idxSqrt));

        factorization.mLLT[_idx] = BlendIndex<_idx>(div, factorization.mLLT[_idx]);
    }
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size>
template <U32 _idx>
inline void LLTDenseSmallSSE<_size>::FactorizeLLT(Factorization& factorization)
{
    FactorizationStep<_idx>(factorization);

    if constexpr (_idx + 1 < _size)
        FactorizeLLT<_idx + 1>(factorization);
    else
        AddLT(factorization.mLLT);
}



// --------------------------------------------------------------------------------------------------------------------

template <U32 _size>
template <U32 _idx>
inline void LLTDenseSmallSSE<_size>::ForwardSubstitution(const std::array<__m128, _size>& llt, __m128& r)
{
    using namespace GDL::simd;

    r = BlendIndex<_idx>(r, _mm_div(r, llt[_idx]));
    r = BlendBelowIndex<_idx>(r, _mm_fnmadd(llt[_idx], Broadcast<_idx>(r), r));

    if constexpr (_idx + 1 < _size)
        ForwardSubstitution<_idx + 1>(llt, r);
}


} // namespace GDL::Solver
