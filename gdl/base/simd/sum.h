#pragma once


#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/utility.h"
#include "gdl/base/simd/x86intrin.h"

#include <array>

namespace GDL::simd
{

//! @brief Calculates the sum of all register values and returns a new register with all values set to the sum.
//! @param source: Source register
//! @return Register with all values equal to the sum of the source register's values.
inline __m128 Sum(__m128 source);

//! @brief Calculates the sum of all register values and returns a new register with all values set to the sum.
//! @param source: Source register
//! @return Register with all values equal to the sum of the source register's values.
inline __m128d Sum(__m128d source);

#ifdef __AVX2__

//! @brief Calculates the sum of all register values and returns a new register with all values set to the sum.
//! @param source: Source register
//! @return Register with all values equal to the sum of the source register's values.
inline __m256 Sum(__m256 source);

//! @brief Calculates the sum of all register values and returns a new register with all values set to the sum.
//! @param source: Source register
//! @return Register with all values equal to the sum of the source register's values.
inline __m256d Sum(__m256d source);

#endif //__AVX2__

//! @brief Calculates the sum of a specific value range inside of an register array. This version  starts at an
//! arbitrary position inside the array and sums up all following values until the end of the array is reached.
//! @tparam _regIdxFirst: Selects the value inside the first register, that should be the first element of the sum
//! @tparam _registerType: Register Type
//! @tparam _size: Size of the array
//! @param arr: Registers array
//! @param idxFirst: Index of the first register that should be part of the sum
//! @return Register with all values equal to the sum
template <U32 _regIdxFirst = 0, typename _registerType, std::size_t _size>
inline _registerType Sum(const std::array<_registerType, _size>& arr, U32 idxFirst = 0);



//! @brief Calculates the square sum of a specific value range inside of an register array. This version  starts at an
//! arbitrary position inside the array and sums up all following values until the end of the array is reached.
//! @tparam _regIdxFirst: Selects the value inside the first register, that should be the first element of the sum
//! @tparam _registerType: Register Type
//! @tparam _size: Size of the array
//! @param arr: Registers array
//! @param idxFirst: Index of the first register that should be part of the sum
//! @return Register with all values equal to the sum
template <U32 _regIdxFirst = 0, typename _registerType, std::size_t _size>
inline _registerType SquareSum(const std::array<_registerType, _size>& arr, U32 idxFirst = 0);


} // namespace GDL::simd

#include "gdl/base/simd/sum.inl"
