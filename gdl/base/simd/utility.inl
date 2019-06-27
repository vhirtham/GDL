#pragma once

#include "gdl/base/exception.h"
#include "gdl/base/simd/constants.h"
#include "gdl/base/simd/utility.h"


#include <type_traits>



namespace GDL::simd
{


template <typename _registerType>
constexpr U32 CalcMinNumArrayRegisters(U32 numValues)
{
    static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

    constexpr U32 registerSize = numRegisterValues<_registerType>;
    return (numValues / registerSize) + ((numValues % registerSize > 0) ? 1 : 0);
}



template <typename _registerType, bool _returnTypeIfNoTARegister>
constexpr auto GetDataType()
{
    if constexpr (Is__m128<_registerType>)
        return F32{0};
    else if constexpr (Is__m128d<_registerType>)
        return F64{0};
#ifdef __AVX2__
    else if constexpr (Is__m256<_registerType>)
        return F32{0};
    else if constexpr (Is__m256d<_registerType>)
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
    if constexpr (Is__m128<_registerType> || Is__m128d<_registerType>)
        return reinterpret_cast<__m128i>(reg);
#ifdef __AVX2__
    if constexpr (Is__m256<_registerType> || Is__m256d<_registerType>)
        return reinterpret_cast<__m256i>(reg);
#endif // __AVX2__
    else
        throw Exception(__PRETTY_FUNCTION__, "Not defined for selected register type.");
}



} // namespace GDL::simd
