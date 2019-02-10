#pragma once

#include <x86intrin.h>
#include "gdl/base/fundamentalTypes.h"


namespace GDL::sse
{



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

//! @brief Creates a new 128 bit register with arbitrary value combination from the source register.
//! @tparam _src0 - _src3: Indices of the source register values that should be stored at corresponding position
//! @param source: Source register
//! @return Register with arbitrary value combination from the source register
template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
inline __m128 Permute(__m128 source);

//! @brief Creates a new 256 bit register with arbitrary value combination from the source register inside each lane.
//! The permutations specified by the template parameters are equal for both lanes.
//! @tparam _src0 - _src3: Indices of the source register values that should be stored at corresponding position
//! @param source: Source register
//! @return Register with arbitrary value combination from the source register
template <U32 _src0, U32 _src1, U32 _src2, U32 _src3>
inline __m256 Permute(__m256 source);

//! @brief Creates a new 256 bit register with arbitrary value combination from the source register inside each lane.
//! The permutations of both lanes specified by the template parameters may differ.
//! @tparam _src0 - _src7: Index of the source register value that should be stored as corresponding element of the new
//! register
//! @param source: Source register
//! @return Register with arbitrary value combination from the source register
template <U32 _src0, U32 _src1, U32 _src2, U32 _src3, U32 _src4, U32 _src5, U32 _src6, U32 _src7>
inline __m256 Permute(__m256 source);

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


//! @brief creates a shuffle mask for a 256 bit double precision register.
//! @tparam _src0 - _src3: Index of the source registers value that should be stored as corresponding element of the new
//! register
//! @return Shuffle mask
constexpr U32 ShuffleMask256d(U32 _src0, U32 _src1, U32 _src2, U32 _src3);

} // namespace GDL::sse


#include "gdl/base/sse/swizzle.inl"
