#pragma once

#include <x86intrin.h>
#include "gdl/base/fundamentalTypes.h"


namespace GDL::sse
{


//! @brief Blends two registers into a new register
//! @tparam _src0 - _src1: Index of the source register that should provide the corresponding value
//! @param source0: First source register
//! @param source1: Second source register
//! @return Register with blended values
template <U32 _src0, U32 _src1>
inline __m128d Blend(__m128d source0, __m128d source1);

//! @brief Blends two registers into a new register
//! @tparam _src0 - _src3: Index of the source register that should provide the corresponding value
//! @tparam _registerType: Register type
//! @param source0: First source register
//! @param source1: Second source register
//! @return Register with blended values
template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, typename _registerType>
inline _registerType Blend(_registerType source0, _registerType source1);

//! @brief Blends two registers into a new register
//! @tparam _src0 - _src7: Index of the source register that should provide the corresponding value
//! @param source0: First source register
//! @param source1: Second source register
//! @return Register with blended values
template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, U32 _src4, U32 _src5, U32 _src6, U32 _src7>
inline __m256 Blend(__m256 source0, __m256 source1);

//! @brief Creates a mask for blend intrinsics
//! @tparam _src0 - _src1: Index of the source register that should provide the corresponding value
//! @return Mask
template <U32 _src0, U32 _src1>
constexpr U32 BlendMask();

//! @brief Creates a mask for blend intrinsics
//! @tparam _src0 - _src3: Index of the source register that should provide the corresponding value
//! @return Mask
template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
constexpr U32 BlendMask();

//! @brief Creates a mask for blend intrinsics
//! @tparam _src0 - _src7: Index of the source register that should provide the corresponding value
//! @return Mask
template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, U32 _src4, U32 _src5, U32 _src6, U32 _src7>
constexpr U32 BlendMask();

//! @brief Picks the value at position _index in each lane and returns a register where each lane element contains the
//! selected value
//! @tparam _index: Index of the value that should be selected in each lane
//! @tparam _registerType: Register type
//! @param source: Source register
//! @return Register where each lane element contains the selected value
template <U32 _index, typename _registerType>
inline _registerType Broadcast(_registerType reg);

//! @brief Creates a new register where each element contains the same value selected from the source register
//! @tparam _index: Index of the value from the source register that should be selected
//! @tparam _registerType: Register type
//! @param source: Source register
//! @return Register with a single arbitrary value from the source register
template <U32 _index, typename _registerType>
inline _registerType BroadcastAcrossLanes(_registerType reg);

//! @brief Exchanges 2 values between 2 registers
//! @tparam _idx0: Index of the first registers value that should be exchanged
//! @tparam _idx1: Index of the second registers value that should be exchanged
//! @param reg0: First register
//! @param reg1: Second register
//! @remark: Use the swap function to swap 2 values that are in the same register.
template <U32 _idx0, U32 _idx1>
inline void Exchange(__m128& reg0, __m128& reg1);

#ifdef __AVX2__

//! @brief Exchanges 2 values between 2 registers
//! @tparam _idx0: Index of the first registers value that should be exchanged
//! @tparam _idx1: Index of the second registers value that should be exchanged
//! @param reg0: First register
//! @param reg1: Second register
//! @remark: Use the swap function to swap 2 values that are in the same register.
template <U32 _idx0, U32 _idx1>
inline void Exchange(__m256& reg0, __m256& reg1);

#endif // __AVX2__


//! @brief Creates a new register with arbitrary value combination from the source register
//! @tparam _src0 - _src1: Indices of the source register values that should be stored at corresponding position
//! @param source: Source register
//! @return Register with arbitrary value combination from the source register
template <U32 _src0, U32 _src1>
inline __m128d Permute(__m128d source);

//! @brief Creates a new register with arbitrary value combination from the source register.
//! @tparam _src0 - _src3: Indices of the source register values that should be stored at corresponding position
//! @tparam _registerType: Register type
//! @param source: Source register
//! @return Register with arbitrary value combination from the source register
template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
inline __m128 Permute(__m128 source);

#ifdef __AVX2__

//! @brief Creates a new register with arbitrary value combination from the source register. Values cant be swizzled
//! across lanes.
//! @tparam _src0 - _src3: Indices of the source register values that should be stored at corresponding position
//! @tparam _registerType: Register type
//! @param source: Source register
//! @return Register with arbitrary value combination from the source register
template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
inline __m256 Permute(__m256 source);

//! @brief Creates a new register with arbitrary value combination from the source register. Values cant be swizzled
//! across lanes.
//! @tparam _src0 - _src3: Indices of the source register values that should be stored at corresponding position
//! @tparam _registerType: Register type
//! @param source: Source register
//! @return Register with arbitrary value combination from the source register
template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
inline __m256d Permute(__m256d source);

//! @brief Creates a new 256 bit register with arbitrary value combination from the source register inside each lane.
//! The permutations of both lanes specified by the template parameters may differ.
//! @tparam _src0 - _src7: Index of the source register value that should be stored as corresponding element of the new
//! register
//! @param source: Source register
//! @return Register with arbitrary value combination from the source register
template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, U32 _src4, U32 _src5, U32 _src6, U32 _src7>
inline __m256 Permute(__m256 source);

#endif // __AVX2__

//! @brief Creates a mask for permute intrinsics
//! @tparam _src0 - _src1: Indices of the source register values that should be stored at corresponding position
//! @return Mask
template <U32 _src0, U32 _src1>
constexpr U32 PermuteMask();

//! @brief Creates a mask for permute intrinsics
//! @tparam _src0 - _src3: Indices of the source register values that should be stored at corresponding position
//! @return Mask
template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
constexpr U32 PermuteMask();

//! @brief Creates a mask for permute intrinsics for __m256d register. Permutations may differ in each lane.
//! @tparam _src0 - _src3: Indices of the source register values that should be stored at corresponding position
//! @return Mask
template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
constexpr U32 Permute256dMask();

#ifdef __AVX2__

//! @brief Creates a new register with both lanes being an arbitrary combination of the source registers lanes.
//! @tparam _lane0SrcLane: Index of the source registers lane that should be used as first lane of the result register
//! @tparam _lane1SrcLane: Index of the source registers lane that should be used as second lane of the result register
//! @tparam _registerType: Register type
//! @param source: Source register
//! @return Register with both lanes being an arbitrary combination of the source registers lanes.
template <U32 _lane0SrcLane, U32 _lane1SrcLane, typename _registerType>
inline _registerType Permute2F128(_registerType source);

//! @brief Creates a new register with both lanes being an arbitrary combination of the source registers lanes.
//! @tparam _lane0SrcReg: Source register that should provide the first lane (0: source0, 1: source1)
//! @tparam _lane0SrcLane: Index of the source registers lane that should be used as first lane of the result register
//! @tparam _lane0SrcReg: Source register that should provide the second lane (0: source0, 1: source1)
//! @tparam _lane1SrcLane: Index of the source registers lane that should be used as second lane of the result register
//! @tparam _registerType: Register type
//! @param source0: First source register
//! @param source1: Second source register
//! @return Register with both lanes being an arbitrary combination of the source registers lanes.
template <U32 _lane0SrcReg, U32 _lane0SrcLane, U32 _lane1SrcReg, U32 _lane1SrcLane, typename _registerType>
inline _registerType Permute2F128(_registerType source0, _registerType source1);

//! @brief Creates a mask for the _mm256_permute2f128 intrinsics
//! @param _lane0SrcReg: Selects the source register for the results first SIMD lane (0 = first / 1 = second register)
//! @param _lane0SrcLane: Index of the source registers lane that should be usd as the results first SIMD lane
//! @param _lane1SrcReg: Selects the source register for the results second SIMD lane (0 = first / 1 = second register)
//! @param _lane0SrcLane: Index of the source registers lane that should be usd as the results second SIMD lane
//! @return Mask
template <U32 _lane0SrcReg, U32 _lane0SrcLane, U32 _lane1SrcReg, U32 _lane1SrcLane>
constexpr U32 Permute2F128Mask();

#endif // __AVX2__

//! @brief Creates a new register with its first value per lane being an arbitrary value of the first registers values
//! from the same lane. The second value is taken from the second register in the same manner.
//! @tparam _src0 - _src1: Index of the source registers value that should be stored as corresponding element of the new
//! register
//! @tparam _registerType: Register type
//! @param source0: First source register
//! @param source1: Second source register
//! @return New register with shuffled values
template <U32 _src0, U32 _src1, typename _registerType>
inline _registerType Shuffle(_registerType source0, _registerType source1);

#ifdef __AVX2__

//! @brief Creates a new register 256 bit double precision with its first value per lane being an arbitrary value of
//! the first registers values from the same lane. The second value is taken from the second register in the same
//! manner.
//! @tparam _src0 - _src3: Index of the source registers value that should be stored as corresponding element of the new
//! register
//! @param source0: First source register
//! @param source1: Second source register
//! @return New register with shuffled values
template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
inline __m256d Shuffle(__m256d source0, __m256d source1);

#endif // __AVX2__

//! @brief Creates a new register with its first two values per lane being an arbitrary combination of the first
//! registers values from the same lane. The second two values are taken from the second register in the same manner.
//! @tparam _src0 - _src3: Index of the source registers value that should be stored as corresponding element of the new
//! register
//! @tparam _registerType: Register type
//! @param source0: First source register
//! @param source1: Second source register
//! @return New register with shuffled values
template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, typename _registerType>
inline _registerType Shuffle(_registerType source0, _registerType source1);

//! @brief Creates a shuffle mask for registers with 2 values per SIMD lane.
//! @tparam _src0 - _src1: Index of the source registers value that should be stored as corresponding element of the new
//! register
//! @return Shuffle mask
template <U32 _src0, U32 _src1>
constexpr U32 ShuffleMask();

//! @brief Creates a shuffle mask for registers with 4 values per SIMD lane.
//! @tparam _src0 - _src3: Index of the source registers value that should be stored as corresponding element of the new
//! register
//! @return Shuffle mask
template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
constexpr U32 ShuffleMask();

//! @brief Creates a shuffle mask for a 256 bit double precision register.
//! @tparam _src0 - _src3: Index of the source registers value that should be stored as corresponding element of the new
//! register
//! @return Shuffle mask
template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
constexpr U32 ShuffleMask256d();

#ifdef __AVX2__


//! @brief Creates a new register where 2 values are swapped
//! @tparam _idx0: Index of the first value that should be swapped
//! @tparam _idx1: Index of the second value that should be swapped
//! @param source: Source register
//! @return New register with swapped values
//! @remark: Use the exchange function to swap 2 values that are not in the same register.
template <U32 _idx0, U32 _idx1>
inline __m128 Swap(__m128 source);

//! @brief Creates a new register where 2 values are swapped
//! @tparam _idx0: Index of the first value that should be swapped
//! @tparam _idx1: Index of the second value that should be swapped
//! @param source: Source register
//! @return New register with swapped values
//! @remark: Use the exchange function to swap 2 values that are not in the same register.
template <U32 _idx0, U32 _idx1>
inline __m256 Swap(__m256 source);

#endif // __AVX2__

} // namespace GDL::sse


#include "gdl/base/sse/swizzle.inl"
