#pragma once

#include "gdl/math/mat4.h"


#include <type_traits>

namespace GDL::TransformationMatrix4
{
//! @brief Creates a orthogonal projection matrix
//! @param width: Width of the output window
//! @param height: Height of the output window
//! @return Orthogonal projection matrix
template <typename _mat4 = Mat4f>
inline _mat4 OrthogonalProjection(F32 width, F32 height);

//! @brief Creates a perspective projection matrix
//! @param fov: Field of view angle
//! @param width: Width of the output window
//! @param height: Height of the output window
//! @param zNear: Nearest z value that can be drawn
//! @param zFar: Farthest z value that can be drawn
//! @return Perspective projection matrix
template <typename _mat4 = Mat4f>
inline _mat4 PerspectiveProjection(F32 fov, F32 width, F32 height, F32 zNear, F32 zFar);


//! @brief Creates a rotation matrix for a rotation around the x-axis
//! @param angle: Rotation angle in rad
//! @return Rotation matrix
template <typename _mat4 = Mat4f>
inline _mat4 RotationX(F32 angle);

//! @brief Creates a rotation matrix for a rotation around the y-axis
//! @param angle: Rotation angle in rad
//! @return Rotation matrix
template <typename _mat4 = Mat4f>
inline _mat4 RotationY(F32 angle);

//! @brief Creates a rotation matrix for a rotation around the z-axis
//! @param angle: Rotation angle in rad
//! @return Rotation matrix
template <typename _mat4 = Mat4f>
inline _mat4 RotationZ(F32 angle);

//! @brief Creates a scaling matrix
//! @param x: Scaling factor in x direction
//! @param y: Scaling factor in y direction
//! @param z: Scaling factor in z direction
//! @return Scaling matrix
template <typename _mat4 = Mat4f>
inline _mat4 Scale(F32 x, F32 y, F32 z);

//! @brief Creates a translation matrix
//! @param x: Translation in x direction
//! @param y: Translation in y direction
//! @param z: Translation in z direction
//! @return Translation matrix
template <typename _mat4 = Mat4f>
inline _mat4 Translation(F32 x, F32 y, F32 z);

} // namespace GDL::TransformationMatrix4

#include "gdl/math/transformationMatrix.inl"
