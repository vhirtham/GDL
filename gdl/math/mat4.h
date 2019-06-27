#pragma once

#ifndef __AVX2__
#include "gdl/math/simd/mat4fSSE.h"
#else
#include "gdl/math/simd/mat4fAVX.h"
#endif // __AVX2__



namespace GDL
{

// forward declarations
class Mat4fAVX;
class Mat4fSSE;
template <typename _type>
class Mat4Serial;

#ifndef __AVX2__
using Mat4f = Mat4fSSE;
#else
using Mat4f = Mat4fAVX;
#endif // __AVX2__



//! @brief Constant that specifies if a type is a 4x4 matrix
template <typename _type>
constexpr const bool IsMat4 = false;
template <>
constexpr const bool IsMat4<Mat4Serial<F32>> = true;
template <>
constexpr const bool IsMat4<Mat4Serial<F64>> = true;
template <>
constexpr const bool IsMat4<Mat4fSSE> = true;
template <>
constexpr const bool IsMat4<Mat4fAVX> = true;

} // namespace GDL
