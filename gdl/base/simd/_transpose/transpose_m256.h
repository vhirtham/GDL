#pragma once



#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/simd/x86intrin.h"

#include <array>



#ifdef __AVX2__

namespace GDL::simd
{


//! @brief Transposes a 1x1 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @tparam _registerType: Register type
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose1x1(__m256 in_0, __m256& out_0) noexcept;



//! @brief Transposes a 1x2 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @tparam _registerType: Register type
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose1x2(__m256 in_0, __m256 in_1, __m256& out_0) noexcept;



//! @brief Transposes a 1x3 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose1x3(__m256 in_0, __m256 in_1, __m256 in_2, __m256& out_0) noexcept;



//! @brief Transposes a 1x4 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose1x4(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256& out_0) noexcept;



//! @brief Transposes a 1x5 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose1x5(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256& out_0) noexcept;



//! @brief Transposes a 1x6 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose1x6(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5,
                         __m256& out_0) noexcept;



//! @brief Transposes a 1x7 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose1x7(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5, __m256 in_6,
                         __m256& out_0) noexcept;



//! @brief Transposes a 1x8 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose1x8(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5, __m256 in_6,
                         __m256 in_7, __m256& out_0) noexcept;



//! @brief Transposes a 2x1 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose2x1(__m256 in_0, __m256& out_0, __m256& out_1) noexcept;



//! @brief Transposes a 2x2 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose2x2(__m256 in_0, __m256 in_1, __m256& out_0, __m256& out_1) noexcept;



//! @brief Transposes a 2x3 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose2x3(__m256 in_0, __m256 in_1, __m256 in_2, __m256& out_0, __m256& out_1) noexcept;



//! @brief Transposes a 2x4 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose2x4(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256& out_0, __m256& out_1) noexcept;



//! @brief Transposes a 2x5 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose2x5(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256& out_0,
                         __m256& out_1) noexcept;



//! @brief Transposes a 2x6 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose2x6(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5, __m256& out_0,
                         __m256& out_1) noexcept;



//! @brief Transposes a 2x7 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose2x7(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5, __m256 in_6,
                         __m256& out_0, __m256& out_1) noexcept;



//! @brief Transposes a 2x8 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose2x8(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5, __m256 in_6,
                         __m256 in_7, __m256& out_0, __m256& out_1) noexcept;



//! @brief Transposes a 3x1 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose3x1(__m256 in_0, __m256& out_0, __m256& out_1, __m256& out_2) noexcept;



//! @brief Transposes a 3x2 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose3x2(__m256 in_0, __m256 in_1, __m256& out_0, __m256& out_1, __m256& out_2) noexcept;



//! @brief Transposes a 3x3 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose3x3(__m256 in_0, __m256 in_1, __m256 in_2, __m256& out_0, __m256& out_1, __m256& out_2) noexcept;



//! @brief Transposes a 3x4 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose3x4(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256& out_0, __m256& out_1,
                         __m256& out_2) noexcept;



//! @brief Transposes a 3x5 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose3x5(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256& out_0, __m256& out_1,
                         __m256& out_2) noexcept;



//! @brief Transposes a 3x6 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose3x6(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5, __m256& out_0,
                         __m256& out_1, __m256& out_2) noexcept;



//! @brief Transposes a 3x7 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose3x7(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5, __m256 in_6,
                         __m256& out_0, __m256& out_1, __m256& out_2) noexcept;



//! @brief Transposes a 3x8 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose3x8(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5, __m256 in_6,
                         __m256 in_7, __m256& out_0, __m256& out_1, __m256& out_2) noexcept;



//! @brief Transposes a 4x1 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose4x1(__m256 in_0, __m256& out_0, __m256& out_1, __m256& out_2, __m256& out_3) noexcept;



//! @brief Transposes a 4x2 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose4x2(__m256 in_0, __m256 in_1, __m256& out_0, __m256& out_1, __m256& out_2, __m256& out_3) noexcept;



//! @brief Transposes a 4x3 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose4x3(__m256 in_0, __m256 in_1, __m256 in_2, __m256& out_0, __m256& out_1, __m256& out_2,
                         __m256& out_3) noexcept;



//! @brief Transposes a 4x4 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose4x4(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256& out_0, __m256& out_1,
                         __m256& out_2, __m256& out_3) noexcept;



//! @brief Transposes a 4x5 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose4x5(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256& out_0, __m256& out_1,
                         __m256& out_2, __m256& out_3) noexcept;



//! @brief Transposes a 4x6 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose4x6(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5, __m256& out_0,
                         __m256& out_1, __m256& out_2, __m256& out_3) noexcept;



//! @brief Transposes a 4x7 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose4x7(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5, __m256 in_6,
                         __m256& out_0, __m256& out_1, __m256& out_2, __m256& out_3) noexcept;



//! @brief Transposes a 4x8 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose4x8(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5, __m256 in_6,
                         __m256 in_7, __m256& out_0, __m256& out_1, __m256& out_2, __m256& out_3) noexcept;



//! @brief Transposes a 5x1 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose5x1(__m256 in_0, __m256& out_0, __m256& out_1, __m256& out_2, __m256& out_3,
                         __m256& out_4) noexcept;



//! @brief Transposes a 5x2 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose5x2(__m256 in_0, __m256 in_1, __m256& out_0, __m256& out_1, __m256& out_2, __m256& out_3,
                         __m256& out_4) noexcept;



//! @brief Transposes a 5x3 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose5x3(__m256 in_0, __m256 in_1, __m256 in_2, __m256& out_0, __m256& out_1, __m256& out_2,
                         __m256& out_3, __m256& out_4) noexcept;



//! @brief Transposes a 5x4 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose5x4(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256& out_0, __m256& out_1,
                         __m256& out_2, __m256& out_3, __m256& out_4) noexcept;



//! @brief Transposes a 5x5 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose5x5(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256& out_0, __m256& out_1,
                         __m256& out_2, __m256& out_3, __m256& out_4) noexcept;



//! @brief Transposes a 5x6 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose5x6(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5, __m256& out_0,
                         __m256& out_1, __m256& out_2, __m256& out_3, __m256& out_4) noexcept;



//! @brief Transposes a 5x7 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose5x7(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5, __m256 in_6,
                         __m256& out_0, __m256& out_1, __m256& out_2, __m256& out_3, __m256& out_4) noexcept;



//! @brief Transposes a 5x8 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose5x8(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5, __m256 in_6,
                         __m256 in_7, __m256& out_0, __m256& out_1, __m256& out_2, __m256& out_3,
                         __m256& out_4) noexcept;



//! @brief Transposes a 6x1 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose6x1(__m256 in_0, __m256& out_0, __m256& out_1, __m256& out_2, __m256& out_3, __m256& out_4,
                         __m256& out_5) noexcept;



//! @brief Transposes a 6x2 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose6x2(__m256 in_0, __m256 in_1, __m256& out_0, __m256& out_1, __m256& out_2, __m256& out_3,
                         __m256& out_4, __m256& out_5) noexcept;



//! @brief Transposes a 6x3 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose6x3(__m256 in_0, __m256 in_1, __m256 in_2, __m256& out_0, __m256& out_1, __m256& out_2,
                         __m256& out_3, __m256& out_4, __m256& out_5) noexcept;



//! @brief Transposes a 6x4 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose6x4(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256& out_0, __m256& out_1,
                         __m256& out_2, __m256& out_3, __m256& out_4, __m256& out_5) noexcept;



//! @brief Transposes a 6x5 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose6x5(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256& out_0, __m256& out_1,
                         __m256& out_2, __m256& out_3, __m256& out_4, __m256& out_5) noexcept;



//! @brief Transposes a 6x6 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose6x6(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5, __m256& out_0,
                         __m256& out_1, __m256& out_2, __m256& out_3, __m256& out_4, __m256& out_5) noexcept;



//! @brief Transposes a 6x7 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose6x7(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5, __m256 in_6,
                         __m256& out_0, __m256& out_1, __m256& out_2, __m256& out_3, __m256& out_4,
                         __m256& out_5) noexcept;



//! @brief Transposes a 6x8 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose6x8(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5, __m256 in_6,
                         __m256 in_7, __m256& out_0, __m256& out_1, __m256& out_2, __m256& out_3, __m256& out_4,
                         __m256& out_5) noexcept;



//! @brief Transposes a 7x1 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose7x1(__m256 in_0, __m256& out_0, __m256& out_1, __m256& out_2, __m256& out_3, __m256& out_4,
                         __m256& out_5, __m256& out_6) noexcept;



//! @brief Transposes a 7x2 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose7x2(__m256 in_0, __m256 in_1, __m256& out_0, __m256& out_1, __m256& out_2, __m256& out_3,
                         __m256& out_4, __m256& out_5, __m256& out_6) noexcept;



//! @brief Transposes a 7x3 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose7x3(__m256 in_0, __m256 in_1, __m256 in_2, __m256& out_0, __m256& out_1, __m256& out_2,
                         __m256& out_3, __m256& out_4, __m256& out_5, __m256& out_6) noexcept;



//! @brief Transposes a 7x4 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose7x4(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256& out_0, __m256& out_1,
                         __m256& out_2, __m256& out_3, __m256& out_4, __m256& out_5, __m256& out_6) noexcept;



//! @brief Transposes a 7x5 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose7x5(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256& out_0, __m256& out_1,
                         __m256& out_2, __m256& out_3, __m256& out_4, __m256& out_5, __m256& out_6) noexcept;



//! @brief Transposes a 7x6 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose7x6(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5, __m256& out_0,
                         __m256& out_1, __m256& out_2, __m256& out_3, __m256& out_4, __m256& out_5,
                         __m256& out_6) noexcept;



//! @brief Transposes a 7x7 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose7x7(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5, __m256 in_6,
                         __m256& out_0, __m256& out_1, __m256& out_2, __m256& out_3, __m256& out_4, __m256& out_5,
                         __m256& out_6) noexcept;



//! @brief Transposes a 7x8 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose7x8(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5, __m256 in_6,
                         __m256 in_7, __m256& out_0, __m256& out_1, __m256& out_2, __m256& out_3, __m256& out_4,
                         __m256& out_5, __m256& out_6) noexcept;



//! @brief Transposes a 8x1 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose8x1(__m256 in_0, __m256& out_0, __m256& out_1, __m256& out_2, __m256& out_3, __m256& out_4,
                         __m256& out_5, __m256& out_6, __m256& out_7) noexcept;



//! @brief Transposes a 8x2 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose8x2(__m256 in_0, __m256 in_1, __m256& out_0, __m256& out_1, __m256& out_2, __m256& out_3,
                         __m256& out_4, __m256& out_5, __m256& out_6, __m256& out_7) noexcept;



//! @brief Transposes a 8x3 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose8x3(__m256 in_0, __m256 in_1, __m256 in_2, __m256& out_0, __m256& out_1, __m256& out_2,
                         __m256& out_3, __m256& out_4, __m256& out_5, __m256& out_6, __m256& out_7) noexcept;



//! @brief Transposes a 8x4 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose8x4(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256& out_0, __m256& out_1,
                         __m256& out_2, __m256& out_3, __m256& out_4, __m256& out_5, __m256& out_6,
                         __m256& out_7) noexcept;



//! @brief Transposes a 8x5 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose8x5(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256& out_0, __m256& out_1,
                         __m256& out_2, __m256& out_3, __m256& out_4, __m256& out_5, __m256& out_6,
                         __m256& out_7) noexcept;



//! @brief Transposes a 8x6 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose8x6(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5, __m256& out_0,
                         __m256& out_1, __m256& out_2, __m256& out_3, __m256& out_4, __m256& out_5, __m256& out_6,
                         __m256& out_7) noexcept;



//! @brief Transposes a 8x7 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose8x7(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5, __m256 in_6,
                         __m256& out_0, __m256& out_1, __m256& out_2, __m256& out_3, __m256& out_4, __m256& out_5,
                         __m256& out_6, __m256& out_7) noexcept;



//! @brief Transposes a 8x8 matrix
//! @tparam _firstRowIn: Index of the matrix's first row in each register
//! @tparam _firstRowOut: Index of the output matrix's first row in each register
//! @tparam _overwriteUnused: Option that specifies if unused values in the output registers can/should be overwritten
//! @tparam _unusedSetZero: Option that specifies if unused values in the output registers are set to zero
//! @param in: Input registers
//! @param out: Output registers
template <U32 _firstRowIn = 0, U32 _firstRowOut = 0, bool _overwriteUnused = true, bool _unusedSetZero = false>
inline void Transpose8x8(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5, __m256 in_6,
                         __m256 in_7, __m256& out_0, __m256& out_1, __m256& out_2, __m256& out_3, __m256& out_4,
                         __m256& out_5, __m256& out_6, __m256& out_7) noexcept;



// internal functions -------------------------------------------------------------------------------------------------

namespace intern
{

//! @brief Perform in-lane permutations after permutations across lane boundaries have been applied for 2xN sized
//! matrices. After this step, no more permutations are requiered.
//! @tparam _firstRowIn: Index of the matrix's first row in each input register
//! @tparam _arraySizeIn: Size of the input array
//! @param in: Array of input registers
//! @return: Array of output registers
template <U32 _firstRowIn, UST _arraySizeIn>
[[nodiscard]] inline std::array<__m256, 2>
IntraLaneTransposeAfterPermute2xN(std::array<__m256, _arraySizeIn> in) noexcept;



//! @brief Perform in-lane permutations after permutations across lane boundaries have been applied for 3xN sized
//! matrices. After this step, no more permutations are requiered.
//! @tparam _firstRowIn: Index of the matrix's first row in each input register
//! @tparam _arraySizeIn: Size of the input array
//! @param in: Array of input registers
//! @return: Array of output registers
template <U32 _firstRowIn, UST _arraySizeIn>
[[nodiscard]] inline std::array<__m256, 3>
IntraLaneTransposeAfterPermute3xN(std::array<__m256, _arraySizeIn> in) noexcept;



//! @brief Perform in-lane permutations after permutations across lane boundaries have been applied for 4xN sized
//! matrices. After this step, no more permutations are requiered.
//! @tparam _firstRowIn: Index of the matrix's first row in each input register
//! @tparam _arraySizeIn: Size of the input array
//! @param in: Array of input registers
//! @return: Array of output registers
template <U32 _firstRowIn, UST _arraySizeIn>
[[nodiscard]] inline std::array<__m256, 4>
IntraLaneTransposeAfterPermute4xN(std::array<__m256, _arraySizeIn> in) noexcept;



//! @brief Perform in-lane permutations after permutations across lane boundaries have been applied for 5xN sized
//! matrices. After this step, no more permutations are requiered.
//! @tparam _firstRowIn: Index of the matrix's first row in each input register
//! @tparam _arraySizeIn: Size of the input array
//! @param in: Array of input registers
//! @return: Array of output registers
template <U32 _firstRowIn>
[[nodiscard]] inline std::array<__m256, 5> IntraLaneTransposeAfterPermute5xN(std::array<__m256, 8> in) noexcept;



//! @brief Perform in-lane permutations after permutations across lane boundaries have been applied for 6xN sized
//! matrices. After this step, no more permutations are requiered.
//! @tparam _firstRowIn: Index of the matrix's first row in each input register
//! @tparam _arraySizeIn: Size of the input array
//! @param in: Array of input registers
//! @return: Array of output registers
template <U32 _firstRowIn>
[[nodiscard]] inline std::array<__m256, 6> IntraLaneTransposeAfterPermute6xN(std::array<__m256, 8> in) noexcept;



//! @brief Perform in-lane permutations after permutations across lane boundaries have been applied for 7xN sized
//! matrices. After this step, no more permutations are requiered.
//! @tparam _firstRowIn: Index of the matrix's first row in each input register
//! @tparam _arraySizeIn: Size of the input array
//! @param in: Array of input registers
//! @return: Array of output registers
template <U32 _firstRowIn>
[[nodiscard]] inline std::array<__m256, 7> IntraLaneTransposeAfterPermute7xN(std::array<__m256, 8> in) noexcept;



//! @brief Perform in-lane permutations before permutations across lane boundaries are applied for Nx1 sized
//! matrices. After this step, the register lanes need to be permuted.
//! @tparam _firstRowOut: Index of the transposed matrix's first row elements in their corresponding registers.
//! @param in_0: Input register
//! @return Array of output registers
template <U32 _firstRowOut>
[[nodiscard]] inline std::array<__m256, 4> IntraLaneTransposeBeforePermuteNx1(__m256 in_0) noexcept;



//! @brief Perform in-lane permutations before permutations across lane boundaries are applied for Nx2 sized
//! matrices. After this step, the register lanes need to be permuted.
//! @tparam _firstRowOut: Index of the transposed matrix's first row elements in their corresponding registers.
//! @param in: Input registers
//! @return Array of output registers
template <U32 _firstRowOut>
[[nodiscard]] inline auto IntraLaneTransposeBeforePermuteNx2(__m256 in_0, __m256 in_1) noexcept;



//! @brief Perform in-lane permutations before permutations across lane boundaries are applied for Nx3 sized
//! matrices. After this step, the register lanes need to be permuted.
//! @tparam _firstRowOut: Index of the transposed matrix's first row elements in their corresponding registers.
//! @param in: Input registers
//! @return Array of output registers
template <U32 _firstRowOut>
[[nodiscard]] inline auto IntraLaneTransposeBeforePermuteNx3(__m256 in_0, __m256 in_1, __m256 in_2) noexcept;



//! @brief Perform in-lane permutations before permutations across lane boundaries are applied for Nx4 sized
//! matrices. After this step, the register lanes need to be permuted.
//! @tparam _firstRowIn: Index of the source matrix's first row elements in their corresponding registers.
//! @tparam _firstRowOut: Index of the transposed matrix's first row elements in their corresponding registers.
//! @param in: Input registers
//! @return Array of output registers
template <U32 _firstRowIn, U32 _firstRowOut>
[[nodiscard]] inline std::array<__m256, 4> IntraLaneTransposeBeforePermuteNx4(__m256 in_0, __m256 in_1, __m256 in_2,
                                                                              __m256 in_3) noexcept;



//! @brief Perform in-lane permutations before permutations across lane boundaries are applied for Nx5 sized
//! matrices. After this step, the register lanes need to be permuted.
//! @tparam _firstRowOut: Index of the transposed matrix's first row elements in their corresponding registers.
//! @param in: Input registers
//! @return Array of output registers
template <U32 _firstRowOut>
[[nodiscard]] inline std::array<__m256, 8> IntraLaneTransposeBeforePermuteNx5(__m256 in_0, __m256 in_1, __m256 in_2,
                                                                              __m256 in_3, __m256 in_4) noexcept;



//! @brief Perform in-lane permutations before permutations across lane boundaries are applied for Nx6 sized
//! matrices. After this step, the register lanes need to be permuted.
//! @tparam _firstRowOut: Index of the transposed matrix's first row elements in their corresponding registers.
//! @param in: Input registers
//! @return Array of output registers
template <U32 _firstRowOut>
[[nodiscard]] inline std::array<__m256, 8> IntraLaneTransposeBeforePermuteNx6(__m256 in_0, __m256 in_1, __m256 in_2,
                                                                              __m256 in_3, __m256 in_4,
                                                                              __m256 in_5) noexcept;



//! @brief Perform in-lane permutations before permutations across lane boundaries are applied for Nx7 sized
//! matrices. After this step, the register lanes need to be permuted.
//! @tparam _firstRowOut: Index of the transposed matrix's first row elements in their corresponding registers.
//! @param in: Input registers
//! @return Array of output registers
template <U32 _firstRowOut>
[[nodiscard]] inline std::array<__m256, 8> IntraLaneTransposeBeforePermuteNx7(__m256 in_0, __m256 in_1, __m256 in_2,
                                                                              __m256 in_3, __m256 in_4, __m256 in_5,
                                                                              __m256 in_6) noexcept;



//! @brief Perform in-lane permutations before permutations across lane boundaries are applied for Nx8 sized
//! matrices. After this step, the register lanes need to be permuted.
//! @tparam _firstRowOut: Index of the transposed matrix's first row elements in their corresponding registers.
//! @param in: Input registers
//! @return Array of output registers
template <U32 _firstRowOut>
[[nodiscard]] inline std::array<__m256, 8> IntraLaneTransposeBeforePermuteNx8(__m256 in_0, __m256 in_1, __m256 in_2,
                                                                              __m256 in_3, __m256 in_4, __m256 in_5,
                                                                              __m256 in_6, __m256 in_7) noexcept;



//! @brief Permute lanes after permutations inside lane boundaries are applied for Nx4 sized
//! matrices. After this step, no more permutations are requiered.
//! @tparam _firstRowIn: Index of the source matrix's first row elements in their corresponding registers.
//! @tparam _firstRowOut: Index of the transposed matrix's first row elements in their corresponding registers.
//! @tparam _rows: Number of rows of the source matrix.
//! @param in: Array of input registers
//! @return Array of output registers
template <U32 _firstRowIn, U32 _firstRowOut, U32 _rows>
[[nodiscard]] inline std::array<__m256, _rows> PermuteAfterIntraLaneTransposeNx4(std::array<__m256, 4> in) noexcept;



//! @brief Permute lanes after permutations inside lane boundaries are applied. After this step, no more permutations
//! are requiered.
//! @tparam _firstRowIn: Index of the source matrix's first row elements in their corresponding registers.
//! @tparam _firstRowOut: Index of the transposed matrix's first row elements in their corresponding registers.
//! @tparam _rows: Number of rows of the source matrix / size of the output array.
//! @tparam _arraySizeIn: Size of the input array
//! @param in: Array of input registers
//! @return Array of output registers
template <U32 _firstRowIn, U32 _firstRowOut, U32 _rows, UST _arraySizeIn>
[[nodiscard]] inline std::array<__m256, _rows>
PermuteAfterIntraLaneTranspose(std::array<__m256, _arraySizeIn> in) noexcept;



//! @brief Permute lanes after permutations inside lane boundaries are applied for Nx8 sized
//! matrices. After this step, no more permutations are requiered.
//! @tparam _firstRowIn: Index of the source matrix's first row elements in their corresponding registers.
//! @tparam _firstRowOut: Index of the transposed matrix's first row elements in their corresponding registers.
//! @tparam _arraySizeIn: Size of the input array
//! @param in: Array of input registers
//! @return Array of output registers
template <U32 _firstRowIn, U32 _firstRowOut, UST _arraySizeIn>
[[nodiscard]] inline std::array<__m256, 8>
PermuteAfterIntraLaneTranspose8xN(std::array<__m256, _arraySizeIn> in) noexcept;



//! @brief Permute lanes before permutations inside lane boundaries are applied for Nx5 sized
//! matrices. After this step, intra lane permutations must be performed.
//! @tparam _firstRowIn: Index of the source matrix's first row elements in their corresponding registers.
//! @tparam _firstRowOut: Index of the transposed matrix's first row elements in their corresponding registers.
//! @tparam _arraySizeOut: Size of the output array
//! @param in: Input registers
//! @return Array of output registers
template <U32 _firstRowIn, U32 _firstRowOut, UST _arraySizeOut>
[[nodiscard]] inline std::array<__m256, _arraySizeOut>
PermuteBeforeIntraLaneTransposeNx5(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4) noexcept;



//! @brief Permute lanes before permutations inside lane boundaries are applied for Nx6 sized
//! matrices. After this step, intra lane permutations must be performed.
//! @tparam _firstRowIn: Index of the source matrix's first row elements in their corresponding registers.
//! @tparam _firstRowOut: Index of the transposed matrix's first row elements in their corresponding registers.
//! @tparam _arraySizeOut: Size of the output array
//! @param in: Input registers
//! @return Array of output registers
template <U32 _firstRowIn, U32 _firstRowOut, UST _arraySizeOut>
[[nodiscard]] inline std::array<__m256, _arraySizeOut>
PermuteBeforeIntraLaneTransposeNx6(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4,
                                   __m256 in_5) noexcept;



//! @brief Permute lanes before permutations inside lane boundaries are applied for Nx7 sized
//! matrices. After this step, intra lane permutations must be performed.
//! @tparam _firstRowIn: Index of the source matrix's first row elements in their corresponding registers.
//! @tparam _firstRowOut: Index of the transposed matrix's first row elements in their corresponding registers.
//! @tparam _arraySizeOut: Size of the output array
//! @param in: Input registers
//! @return Array of output registers
template <U32 _firstRowIn, U32 _firstRowOut, UST _arraySizeOut>
[[nodiscard]] inline std::array<__m256, _arraySizeOut>
PermuteBeforeIntraLaneTransposeNx7(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5,
                                   __m256 in_6) noexcept;



//! @brief Permute lanes before permutations inside lane boundaries are applied for Nx8 sized
//! matrices. After this step, intra lane permutations must be performed.
//! @tparam _firstRowIn: Index of the source matrix's first row elements in their corresponding registers.
//! @tparam _firstRowOut: Index of the transposed matrix's first row elements in their corresponding registers.
//! @tparam _arraySizeOut: Size of the output array
//! @param in: Input registers
//! @return Array of output registers
template <U32 _firstRowIn, U32 _firstRowOut, UST _arraySizeOut>
[[nodiscard]] inline std::array<__m256, _arraySizeOut>
PermuteBeforeIntraLaneTransposeNx8(__m256 in_0, __m256 in_1, __m256 in_2, __m256 in_3, __m256 in_4, __m256 in_5,
                                   __m256 in_6, __m256 in_7) noexcept;

} // namespace intern

} // namespace GDL::simd


#endif // __AVX2__



#include "gdl/base/simd/_transpose/transpose_m256.inl"
