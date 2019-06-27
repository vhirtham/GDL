#pragma once


#include "gdl/base/simd/negate.h"
#include "gdl/base/simd/intrinsics.h"
#include "gdl/base/simd/utility.h"

#include <type_traits>


namespace GDL::simd
{


template <typename _type, U32 _negation>
constexpr _type SignMask()
{
    static_assert(_negation < 2, "Template parameter \"_negation\" must be 0 or 1.");
    static_assert(std::is_same<_type, F32>::value || std::is_same<_type, F64>::value,
                  "Function only compatible with F32 and F64 types.");


    if constexpr (_negation)
        return static_cast<_type>(-0.);
    else
        return static_cast<_type>(0.);
}



template <typename _registerType>
inline _registerType Negate(_registerType source)
{
    constexpr U32 numRegVals = numRegisterValues<_registerType>;

    static_assert(numRegVals == 2 || numRegVals == 4 || numRegVals == 8,
                  "Only registers with 2, 4 or 8 values are supported.");

    if constexpr (numRegVals == 2)
        return Negate<1, 1>(source);

    if constexpr (numRegVals == 4)
        return Negate<1, 1, 1, 1>(source);

    if constexpr (numRegVals == 8)
        return Negate<1, 1, 1, 1, 1, 1, 1, 1>(source);
}



template <U32 _n0, U32 _n1>
inline __m128d Negate(const __m128d source)
{
    return _mm_xor(source, NegationMask<_n0, _n1>());
}



template <U32 _n0, U32 _n1, U32 _n2, U32 _n3, typename _registerType>
inline _registerType Negate(const _registerType source)
{
    return _mm_xor(source, NegationMask<_registerType, _n0, _n1, _n2, _n3>());
}



#ifdef __AVX2__

template <U32 _n0, U32 _n1, U32 _n2, U32 _n3, U32 _n4, U32 _n5, U32 _n6, U32 _n7>
inline __m256 Negate(const __m256 source)
{
    return _mm_xor(source, NegationMask<_n0, _n1, _n2, _n3, _n4, _n5, _n6, _n7>());
}

#endif // __AVX2__



template <U32 _n0, U32 _n1>
inline __m128d NegationMask()
{
    return _mm_setr<__m128d>(SignMask<F64, _n0>(), SignMask<F64, _n1>());
}



template <typename _registerType, U32 _n0, U32 _n1, U32 _n2, U32 _n3>
inline _registerType NegationMask()
{
    static_assert(Is__m128<_registerType> || Is__m256d<_registerType>,
                  "Function overload only compatble with __m128 and __m256d registers.");

    using ValueType = decltype(GetDataType<_registerType>());
    return _mm_setr<_registerType>(SignMask<ValueType, _n0>(), SignMask<ValueType, _n1>(),
                                      SignMask<ValueType, _n2>(), SignMask<ValueType, _n3>());
}


#ifdef __AVX2__

template <U32 _n0, U32 _n1, U32 _n2, U32 _n3, U32 _n4, U32 _n5, U32 _n6, U32 _n7>
inline __m256 NegationMask()
{
    return _mm_setr<__m256>(SignMask<F32, _n0>(), SignMask<F32, _n1>(), SignMask<F32, _n2>(), SignMask<F32, _n3>(),
                               SignMask<F32, _n4>(), SignMask<F32, _n5>(), SignMask<F32, _n6>(), SignMask<F32, _n7>());
}

#endif // __AVX2__


} // namespace GDL::simd
