#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/constants.h"
#include "gdl/base/simd/x86intrin.h"

#include <array>

namespace GDL::simd::intern
{

//! @brief Helper struct for transposition of multi lane registers. It contains only static values.
//! @tparam _registerType: Register type
//! @tparam _firstRowIn: Index of the register element that is part of the input matrix's first row
//! @tparam _firstRowOut: Index of the register element that is part of the output matrix's first row
template <typename _registerType, U32 _firstRowIn, U32 _firstRowOut>
struct TranspositionLaneData
{
    //! @brief Register lane that contains the first row of the input matrix
    static constexpr U32 In = _firstRowIn / numValuesPerLane<_registerType>;

    //! @brief Register lane that contains the first row of the output matrix
    static constexpr U32 Out = _firstRowOut / numValuesPerLane<_registerType>;

    //! @brief Index of the input matrix's first row inside it's lane.
    static constexpr U32 OffsetIn = _firstRowIn % numValuesPerLane<_registerType>;

    //! @brief Index of the output matrix's first row inside it's lane.
    static constexpr U32 OffsetOut = _firstRowOut % numValuesPerLane<_registerType>;
};



//! @brief Copies values recursively from the input register array to the passed output registers.
//! @tparam _idxFirst: Index of the first element in each register that should be copied.
//! @tparam _numValues: Number of consecutive values that should be copied.
//! @tparam _overwriteUnused: If true, register values that don't lie in the specified index range can be overwritten.
//! @tparam _unusedSetZero: If true, register values that don't lie in the specified index range are set to zero.
//! Requires _overwriteUnused to be true.
//! @tparam _initialCall: Do not set this manually. Helps to check if the number of output registers is correct.
//! @tparam _registerType: Register type
//! @tparam _arraySize: Size of the input array
//! @tparam _args: Parameter pack type
//! @param in: Array of input registers
//! @param out: Output register, that is written to during the current recursion.
//! @param args: Parameter pack containing the output registers that should be modified in subsequent recursive function
//! calls.
template <U32 _idxFirst, U32 _numValues, bool _overwriteUnused, bool _unusedSetZero, bool _initialCall = true,
          typename _registerType, UST _arraySize, typename... _args>
inline void TransposeSetOutput(std::array<_registerType, _arraySize> in, _registerType& out, _args&... args);



} // namespace GDL::simd::intern

#include "gdl/base/simd/_transpose/transpose_utility.inl"
