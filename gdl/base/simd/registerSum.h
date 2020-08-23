#pragma once


#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/utility.h"
#include "gdl/base/simd/x86intrin.h"

#include <array>

namespace GDL::simd
{



//! @brief Calculates the sum of a specific value range inside of an register array. This version  starts at an
//! arbitrary position inside the array and sums up all following values until the end of the array is reached.
//! @tparam _regIdxFirst: Selects the value inside the first register, that should be the first element of the sum
//! @tparam _registerType: Register Type
//! @tparam _size: Size of the array
//! @param arr: Registers array
//! @param idxFirst: Index of the first register that should be part of the sum
//! @return Register with all values equal to the sum
template <U32 _regIdxFirst = 0, typename _registerType, std::size_t _size>
[[nodiscard]] inline _registerType RegisterArraySum(const std::array<_registerType, _size>& arr,
                                                    U32 idxFirst = 0) noexcept;



//! @brief Calculates the square sum of a specific value range inside of an register array. This version  starts at an
//! arbitrary position inside the array and sums up all following values until the end of the array is reached.
//! @tparam _regIdxFirst: Selects the value inside the first register, that should be the first element of the sum
//! @tparam _registerType: Register Type
//! @tparam _size: Size of the array
//! @param arr: Registers array
//! @param idxFirst: Index of the first register that should be part of the sum
//! @return Register with all values equal to the sum
template <U32 _regIdxFirst = 0, typename _registerType, std::size_t _size>
[[nodiscard]] inline _registerType RegisterArraySquareSum(const std::array<_registerType, _size>& arr,
                                                          U32 idxFirst = 0) noexcept;



//! @brief Calculate the sum of multiple registers. The results are stored in a single register in the same order as the
//! registers occur in the array. In consequence, the array size must be equal to the number of values per register.
//! @param data: Array of 2 registers
//! @return Register containing the sums of the array registers.
[[nodiscard]] inline __m128d RegisterMultiSum(const std::array<__m128d, 2>& data) noexcept;



//! @brief Calculate the sum of multiple registers. The results are stored in a single register in the same order as the
//! registers occur in the array. In consequence, the array size must be equal to the number of values per register.
//! @param data: Array of 4 registers
//! @return Register containing the sums of the array registers.
[[nodiscard]] inline __m128 RegisterMultiSum(const std::array<__m128, 4>& data) noexcept;



#ifdef __AVX2__

//! @brief Calculate the sum of multiple registers. The results are stored in a single register in the same order as the
//! registers occur in the array. In consequence, the array size must be equal to the number of values per register.
//! @param data: Array of 4 registers
//! @return Register containing the sums of the array registers.
[[nodiscard]] inline __m256d RegisterMultiSum(const std::array<__m256d, 4>& data) noexcept;



//! @brief Calculate the sum of multiple registers. The results are stored in a single register in the same order as the
//! registers occur in the array. In consequence, the array size must be equal to the number of values per register.
//! @param data: Array of 8 registers
//! @return Register containing the sums of the array registers.
[[nodiscard]] inline __m256 RegisterMultiSum(const std::array<__m256, 8>& data) noexcept;

#endif



//! @brief Calculates the sum of all register values and returns a new register with all values set to the sum.
//! @param source: Source register
//! @return Register with all values equal to the sum of the source register's values.
[[nodiscard]] inline __m128 RegisterSum(__m128 source) noexcept;



//! @brief Calculates the sum of all register values and returns a new register with all values set to the sum.
//! @param source: Source register
//! @return Register with all values equal to the sum of the source register's values.
[[nodiscard]] inline __m128d RegisterSum(__m128d source) noexcept;



#ifdef __AVX2__

//! @brief Calculates the sum of all register values and returns a new register with all values set to the sum.
//! @param source: Source register
//! @return Register with all values equal to the sum of the source register's values.
[[nodiscard]] inline __m256 RegisterSum(__m256 source) noexcept;



//! @brief Calculates the sum of all register values and returns a new register with all values set to the sum.
//! @param source: Source register
//! @return Register with all values equal to the sum of the source register's values.
[[nodiscard]] inline __m256d RegisterSum(__m256d source) noexcept;

#endif //__AVX2__



} // namespace GDL::simd

#include "gdl/base/simd/registerSum.inl"
