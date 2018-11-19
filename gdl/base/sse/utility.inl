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



template <I32 _i0, I32 _i1, I32 _i2, I32 _i3>
constexpr I32 ShuffleMask()
{
    static_assert(_i0 > -1 && _i1 > -1 && _i2 > -1 && _i3 > -1 && _i0 < 4 && _i1 < 4 && _i2 < 4 && _i3 < 4,
                  "Invalid indices");
    return _i0 + _i1 * 4 + _i2 * 16 + _i3 * 64;
}



template <I32 _i0, I32 _i1>
constexpr I32 ShuffleMask()
{
    static_assert(_i0 > -1 && _i1 > -1 && _i0 < 2 && _i1 < 2, "Invalid indices");
    return _i0 + _i1 * 2;
}


} // namespace GDL::sse
