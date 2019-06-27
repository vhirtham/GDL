#pragma once

#include "gdl/base/simd/dotProduct.h"
#include "gdl/base/simd/intrinsics.h"


namespace GDL::simd
{


template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, U32 _dst0, U32 _dst1, U32 _dst2, U32 _dst3>
constexpr inline U32 GetDotProductMask4()
{
    static_assert(_src0 < 2 && _src1 < 2 && _src2 < 2 && _src3 < 2 && _dst0 < 2 && _dst1 < 2 && _dst2 < 2 && _dst3 < 2,
                  "Destination and source template parameters can only be 0 or 1.");

    U32 mask = ((_src3 << 7) | (_src2 << 6) | (_src1 << 5) | (_src0 << 4) | (_dst3 << 3) | (_dst2 << 2) | (_dst1 << 1) |
                (_dst0 << 0));
    return mask;
}



template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, U32 _dst0, U32 _dst1, U32 _dst2, U32 _dst3>
inline __m128 DotProduct(const __m128& lhs, const __m128& rhs)
{
    return _mm_dp<GetDotProductMask4<_src0, _src1, _src2, _src3, _dst0, _dst1, _dst2, _dst3>()>(lhs, rhs);
}



template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
inline F32 DotProductF32(const __m128& lhs, const __m128& rhs)
{
    return _mm_cvtsF(DotProduct<_src0, _src1, _src2, _src3>(lhs, rhs));
}



template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, U32 _dst0, U32 _dst1, U32 _dst2, U32 _dst3>
inline __m256 DotProduct(const __m256& lhs, const __m256& rhs)
{
    return _mm_dp<GetDotProductMask4<_src0, _src1, _src2, _src3, _dst0, _dst1, _dst2, _dst3>()>(lhs, rhs);
}

} // namespace GDL::simd
