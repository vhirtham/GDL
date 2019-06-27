#pragma once



#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/utility.h"
#include "gdl/base/simd/x86intrin.h"

namespace GDL::simd
{
//! @brief Creates a mask for the dot product intrinsics with 4 floating point values per 128bit (__m128 and __m256).
//! The template parameters specify the register values that should be used to calculate the dot product and the
//! positions in the result register where the dot product should be stored.
//! @tparam _src0 - _src3: If the value is set to 1 the corresponding values of the two source registers take part in
//! the calculation of the dot product. Otherwise the value must be set to 0.
//! @tparam _dst0 - _dst3: If the value is set to 1 the dot product will be written to the corresponding location in the
//! result register. If 0, the value is set to zero.
//! @return Mask for dot product intrinsics
template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, U32 _dst0, U32 _dst1, U32 _dst2, U32 _dst3>
constexpr inline U32 GetDotProductMask4();


//! @brief Calculates the dot product of two registers and returns a register with the dot product written to the
//! specified positions.
//! @tparam _src0 - _src3: If the value is set to 1 the corresponding values of the two source registers take part in
//! the calculation of the dot product. Otherwise the value must be set to 0.
//! @tparam _dst0 - _dst3: If the value is set to 1 the dot product will be written to the corresponding location in the
//! result register. If 0, the value is set to zero.
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Register containing the dot product
template <U32 _src0 = 1, U32 _src1 = 1, U32 _src2 = 1, U32 _src3 = 1, U32 _dst0 = 1, U32 _dst1 = 1, U32 _dst2 = 1,
          U32 _dst3 = 1>
inline __m128 DotProduct(const __m128& lhs, const __m128& rhs);


//! @brief Calculates the dot product of two registers
//! @tparam _src0 - _src3: If the value is set to 1 the corresponding values of the two source registers take part in
//! the calculation of the dot product. Otherwise the value must be set to 0.
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Dot product
template <U32 _src0 = 1, U32 _src1 = 1, U32 _src2 = 1, U32 _src3 = 1>
inline F32 DotProductF32(const __m128& lhs, const __m128& rhs);



//! @brief Calculates the dot products of the higher and lower 128 bit lane of two registers and returns a register with
//! the dot products written to the specified positions.
//! @tparam _src0 - _src3: If the value is set to 1 the corresponding values of the two source registers 128 bit lanes
//! take part in the calculation of the dot product. Otherwise the value must be set to 0.
//! @tparam _dst0 - _dst3: If the value is set to 1 the dot products will be written to the corresponding location in
//! the result registers 128bit lanes. If 0, the value is set to zero.
//! @param lhs: Left hand side register
//! @param rhs: Right hand side register
//! @return Register containing the dot products
template <U32 _src0 = 1, U32 _src1 = 1, U32 _src2 = 1, U32 _src3 = 1, U32 _dst0 = 1, U32 _dst1 = 1, U32 _dst2 = 1,
          U32 _dst3 = 1>
inline __m256 DotProduct(const __m256& lhs, const __m256& rhs);

} // namespace GDL::simd


#include "gdl/base/simd/dotProduct.inl"
