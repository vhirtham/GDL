#pragma once

#include "gdl/base/exception.h"
#include "gdl/base/sse/utility.h"


#include <type_traits>



namespace GDL::sse
{


template <typename _registerType>
constexpr U32 CalcMinNumArrayRegisters(U32 numValues)
{
    constexpr U32 registerSize = numRegisterValues<_registerType>;
    return (numValues / registerSize) + ((numValues % registerSize > 0) ? 1 : 0);
}



template <typename _registerType, bool _returnTypeIfNoTARegister>
constexpr auto GetDataType()
{
    if constexpr (std::is_same<_registerType, __m128>::value)
        return F32{0};
    else if constexpr (std::is_same<_registerType, __m128d>::value)
        return F64{0};
#ifdef __AVX2__
    else if constexpr (std::is_same<_registerType, __m256>::value)
        return F32{0};
    else if constexpr (std::is_same<_registerType, __m256d>::value)
        return F64{0};
#endif // __AVX2__
    else if constexpr (_returnTypeIfNoTARegister)
        return _registerType();
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



template <typename _type, U32 _registerSize>
auto GetFittingRegister()
{
    static_assert(_registerSize == 128 || _registerSize == 256 || _registerSize == 512,
                  "Register size must be 128, 256 or 512");

    if constexpr (std::is_same<_type, F32>::value && _registerSize == 128)
        return __m128();
    if constexpr (std::is_same<_type, F64>::value && _registerSize == 128)
        return __m128d();
#ifdef __AVX2__
    if constexpr (std::is_same<_type, F32>::value && _registerSize == 256)
        return __m256();
    if constexpr (std::is_same<_type, F64>::value && _registerSize == 256)
        return __m256d();
#endif // __AVX2__
}



template <U32 _index>
inline F32 GetValue(__m128 reg)
{
    static_assert(_index < 4, "Invalid index");
    if constexpr (_index == 0)
        return _mm_cvtss_f32(reg);
    else
        return _mm_cvtss_f32(_mm_shuffle_ps(reg, reg, ShuffleMask<_index, _index, _index, _index>()));
}


inline F32 GetValue(__m128 reg, U32 index)
{
    DEV_EXCEPTION(index > 3, "Invalid index");
    return _mm_cvtss_f32(_mm_shuffle_ps(reg, reg, ShuffleMask(index, index, index, index)));
}


template <U32 _index>
inline F64 GetValue(__m128d reg)
{
    static_assert(_index < 2, "Invalid index");
    if constexpr (_index == 0)
        return _mm_cvtsd_f64(reg);
    else
        return _mm_cvtsd_f64(_mm_unpackhi_pd(reg, reg));
}



inline F64 GetValue(__m128d reg, U32 index)
{
    DEV_EXCEPTION(index > 1, "Invalid index");
    return _mm_cvtsd_f64(_mm_shuffle_pd(reg, reg, ShuffleMask(index, index)));
}


template <U32 _index>
inline F32 GetValue(__m256 reg)
{
    static_assert(_index < 8, "Invalid index");
    if constexpr (_index < 4)
        return GetValue<_index>(_mm256_extractf128_ps(reg, 0));
    else
        return GetValue<_index - 4>(_mm256_extractf128_ps(reg, 1));
}



inline F32 GetValue(__m256 reg, U32 index)
{
    DEV_EXCEPTION(index > 7, "Invalid index");
    const I32 subIndex = (index / 4); // ---> possible values 0,1
    return GetValue(_mm256_extractf128_ps(reg, subIndex), index - subIndex * 4);
}



template <U32 _index>
inline F64 GetValue(__m256d reg)
{
    static_assert(_index < 4, "Invalid index");
    if constexpr (_index < 2)
        return GetValue<_index>(_mm256_extractf128_pd(reg, 0));
    else
        return GetValue<_index - 2>(_mm256_extractf128_pd(reg, 1));
}



inline F64 GetValue(__m256d reg, U32 index)
{
    DEV_EXCEPTION(index > 3, "Invalid index");
    const I32 subIndex = (index / 2); // ---> possible values 0,1
    return GetValue(_mm256_extractf128_pd(reg, subIndex), index - subIndex * 2);
}



template <typename _registerType>
constexpr bool IsRegisterType()
{
    if constexpr (std::is_same<_registerType, __m128>::value || std::is_same<_registerType, __m128d>::value
#ifdef __AVX2__
                  || std::is_same<_registerType, __m256>::value || std::is_same<_registerType, __m256d>::value
#endif // __AVX2__
    )
        return true;
    return false;
}



constexpr U32 MaxRegisterSize()
{
#ifdef __AVX2__
    return 256;
#else
    return 128;
#endif
}



template <typename _registerType>
inline auto ReinterpretAsIntRegister(_registerType reg)
{
    if constexpr (std::is_same<_registerType, __m128>::value || std::is_same<_registerType, __m128d>::value)
        return reinterpret_cast<__m128i>(reg);
#ifdef __AVX2__
    if constexpr (std::is_same<_registerType, __m256>::value || std::is_same<_registerType, __m256d>::value)
        return reinterpret_cast<__m256i>(reg);
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



template <I32 _i0, I32 _i1>
constexpr I32 ShuffleMask()
{
    static_assert(_i0 > -1 && _i1 > -1 && _i0 < 2 && _i1 < 2, "Invalid indices");
    return _i0 + _i1 * 2;
}



constexpr I32 ShuffleMask(I32 i0, I32 i1)
{
    DEV_EXCEPTION(!(i0 > -1 && i1 > -1 && i0 < 2 && i1 < 2), "Invalid indices");
    return i0 + i1 * 2;
}



template <I32 _i0, I32 _i1, I32 _i2, I32 _i3>
constexpr I32 ShuffleMask()
{
    static_assert(_i0 > -1 && _i1 > -1 && _i2 > -1 && _i3 > -1 && _i0 < 4 && _i1 < 4 && _i2 < 4 && _i3 < 4,
                  "Invalid indices");
    return _i0 + _i1 * 4 + _i2 * 16 + _i3 * 64;
}



constexpr I32 ShuffleMask(I32 i0, I32 i1, I32 i2, I32 i3)
{
    DEV_EXCEPTION(!(i0 > -1 && i1 > -1 && i2 > -1 && i3 > -1 && i0 < 4 && i1 < 4 && i2 < 4 && i3 < 4),
                  "Invalid indices");
    return i0 + i1 * 4 + i2 * 16 + i3 * 64;
}



} // namespace GDL::sse
