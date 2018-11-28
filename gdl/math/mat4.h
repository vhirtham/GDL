#pragma once

#include "gdl/math/mat4SSE.h"

namespace GDL
{

// forward declarations
class Mat4SSE;
template <typename _type>
class Mat4Single;



typedef Mat4SSE Mat4f;



//! @brief Returns if a type is a 4x4 matrix
//! @tparam _type: Type that should be checked
//! @return TRUE or FALSE
template <typename _type>
constexpr bool IsMat4()
{
    return std::is_same<_type, Mat4SSE>::value || std::is_same<_type, Mat4Single<F32>>::value ||
           std::is_same<_type, Mat4Single<F64>>::value;
}
} // namespace GDL
