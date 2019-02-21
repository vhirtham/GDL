#pragma once

#include "gdl/math/mat3.h"


#include <type_traits>

namespace GDL::Transformations3
{

//! @brief Creates a rotation matrix for a rotation around the x-axis
//! @param angle: Rotation angle in rad
//! @return Rotation matrix
template <typename _mat3 = Mat3f>
inline _mat3 RotationX(F32 angle);

//! @brief Creates a rotation matrix for a rotation around the y-axis
//! @param angle: Rotation angle in rad
//! @return Rotation matrix
template <typename _mat3 = Mat3f>
inline _mat3 RotationY(F32 angle);

//! @brief Creates a rotation matrix for a rotation around the z-axis
//! @param angle: Rotation angle in rad
//! @return Rotation matrix
template <typename _mat3 = Mat3f>
inline _mat3 RotationZ(F32 angle);

//! @brief Creates a scaling matrix
//! @param x: Scaling factor in x direction
//! @param y: Scaling factor in y direction
//! @param z: Scaling factor in z direction
//! @return Scaling matrix
template <typename _mat3 = Mat3f>
inline _mat3 Scale(F32 x, F32 y, F32 z);

} // namespace GDL::Transformations3

#include "gdl/math/transformations3.inl"
