#pragma once



#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/x86intrin.h"



namespace GDL::simd
{

//! @brief Transposes a 1x1 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose1x1(__m128d in, __m128d& out);



//! @brief Transposes a 1x2 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose1x2(__m128d in0, __m128d in1, __m128d& out0);



//! @brief Transposes a 2x1 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose2x1(__m128d in0, __m128d& out0, __m128d& out1);



//! @brief Transposes a 2x2 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose2x2(__m128d in0, __m128d in1, __m128d& out0, __m128d& out1);



} // namespace GDL::simd



#include "gdl/base/simd/_transpose/transpose_m128d.inl"