#pragma once

#include "gdl/base/sse/directAccess.h"

#include "gdl/base/exception.h"
#include "gdl/base/sse/utility.h"


namespace GDL::sse
{


template <U32 _index>
inline F32 GetValue(__m128 reg)
{
    static_assert(_index < 4, "Invalid index");
    if constexpr (_index == 0)
        return _mm_cvtss_f32(reg);
    else
        return _mm_cvtss_f32(_mm_shuffle_ps(reg, reg, _MM_SHUFFLE(_index, _index, _index, _index)));
}


inline F32 GetValue(__m128 reg, const U32 index)
{
    switch (index)
    {
    case 0:
        return GetValue<0>(reg);
    case 1:
        return GetValue<1>(reg);
    case 2:
        return GetValue<2>(reg);
    case 3:
        return GetValue<3>(reg);
    default:
        EXCEPTION(true, "Invalid index");
    }
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
    switch (index)
    {
    case 0:
        return GetValue<0>(reg);
    case 1:
        return GetValue<1>(reg);
    default:
        EXCEPTION(true, "Invalid index");
    }
}

#ifdef __AVX2__

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
    switch (index)
    {
    case 0:
        return GetValue<0>(reg);
    case 1:
        return GetValue<1>(reg);
    case 2:
        return GetValue<2>(reg);
    case 3:
        return GetValue<3>(reg);
    case 4:
        return GetValue<4>(reg);
    case 5:
        return GetValue<5>(reg);
    case 6:
        return GetValue<6>(reg);
    case 7:
        return GetValue<7>(reg);
    default:
        EXCEPTION(true, "Invalid index");
    }
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
    switch (index)
    {
    case 0:
        return GetValue<0>(reg);
    case 1:
        return GetValue<1>(reg);
    case 2:
        return GetValue<2>(reg);
    case 3:
        return GetValue<3>(reg);
    default:
        EXCEPTION(true, "Invalid index");
    }
}

#endif // __AVX2__

} // namespace GDL::sse
