#pragma once

#include "gdl/base/fundamentalTypes.h"

#include <x86intrin.h>


namespace GDL::sse
{

//! @brief Calculates the determinant of a 2x2 matrix
//! @param data: Register with matrix values where the first two values represent the first column and the third and
//! fourth value the second column
//! @return Determinant
inline F32 Determinant2x2(__m128 data);

//! @brief Calculates the determinant of a 2x2 matrix
//! @param col0: Register where the first two values represent the first column of the matrix
//! @param col1: Register where the first two values represent the second column of the matrix
//! @return Determinant
inline F32 Determinant2x2(__m128 col0, __m128 col1);

//! @brief Calculates the determinant of a 3x3 matrix
//! @param col0: Register where the first three values represent the first column of the matrix
//! @param col1: Register where the first three values represent the second column of the matrix
//! @param col2: Register where the first three values represent the third column of the matrix
//! @return Determinant
inline F32 Determinant3x3(__m128 col0, __m128 col1, __m128 col2);

} // namespace GDL::sse

#include "gdl/base/sse/determinant.inl"
