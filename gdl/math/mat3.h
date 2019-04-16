#pragma once

#include "gdl/math/sse/mat3fSSE.h"

namespace GDL
{

// forward declarations
class Mat3fSSE;
template <typename _type>
class Mat3Serial;



using Mat3f = Mat3fSSE;



//! @brief Constant that specifies if a type is a 3x3 matrix
template <typename _type>
constexpr const bool IsMat3 = false;
template <>
constexpr const bool IsMat3<Mat3Serial<F32>> = true;
template <>
constexpr const bool IsMat3<Mat3Serial<F64>> = true;
template <>
constexpr const bool IsMat3<Mat3fSSE> = true;
} // namespace GDL
