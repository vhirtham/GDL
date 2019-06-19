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
//! @param a: Register where the first two values represent the first column of the matrix
//! @param b: Register where the first two values represent the second column of the matrix
//! @return Determinant
//! @remark Due to the determinants property that it stays the same if the matrix is transposed, the input values can
//! also be treated as the rows of the matrix
inline F32 Determinant2x2(__m128 a, __m128 b);

//! @brief Calculates the determinant of a 3x3 matrix
//! @param a: Register where the first three values represent the first column of the matrix
//! @param b: Register where the first three values represent the second column of the matrix
//! @param c: Register where the first three values represent the third column of the matrix
//! @return Determinant
//! @remark Due to the determinants property that it stays the same if the matrix is transposed, the input values can
//! also be treated as the rows of the matrix
inline F32 Determinant3x3(__m128 a, __m128 b, __m128 c);

//! @brief Calculates the determinant of a 4x4 matrix
//! @param a: Register that represents the first column of the matrix
//! @param b: Register that represents the second column of the matrix
//! @param c: Register that represents the third column of the matrix
//! @param d: Register that represents the third fourth of the matrix
//! @return Determinant
//! @remark Due to the determinants property that it stays the same if the matrix is transposed, the input values can
//! also be treated as the rows of the matrix
inline F32 Determinant4x4(__m128 a, __m128 b, __m128 c, __m128 d);

#ifdef __AVX2__

//! @brief Calculates the determinant of a 4x4 matrix
//! @param ab: Register that represents the first two columns of the matrix
//! @param cd: Register that represents the last two columns of the matrix
//! @return Determinant
inline F32 Determinant4x4(__m256 ab, __m256 cd);

#endif // __AVX2__

} // namespace GDL::sse

#include "gdl/base/sse/determinant.inl"
