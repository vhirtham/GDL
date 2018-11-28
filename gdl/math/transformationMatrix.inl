#pragma once

#include "gdl/math/transformationMatrix.h"

namespace GDL::TransformationMatrix4
{



template <typename _mat4>
_mat4 OrthogonalProjection(F32 width, F32 height)
{
    static_assert(IsMat4<_mat4>(), "Template parameter is no valid 4x4 matrix type.");

    // clang-format off
    return _mat4(
              2./width,         0.,         0.,         0.,
                    0.,  2./height,         0.,         0.,
                    0.,         0.,         1.,         0.,
                    0.,         0.,         0.,         1.);
    // clang-format on
}



template <typename _mat4>
inline _mat4 RotationZ(F32 angle)
{
    static_assert(IsMat4<_mat4>(), "Template parameter is no valid 4x4 matrix type.");

    F32 sinAngle = std::sin(angle);
    F32 cosAngle = std::cos(angle);

    // clang-format off
    return _mat4(
         cosAngle, sinAngle,       0.,       0.,
        -sinAngle, cosAngle,       0.,       0.,
               0.,       0.,       1.,       0.,
               0.,       0.,       0.,       1.);
    // clang-format on
}



template <typename _mat4>
_mat4 Scale(F32 x, F32 y, F32 z)
{
    static_assert(IsMat4<_mat4>(), "Template parameter is no valid 4x4 matrix type.");

    // clang-format off
    return _mat4(
        x ,     0.,     0.,     0.,
        0.,     y ,     0.,     0.,
        0.,     0.,     z ,     0.,
        0.,     0.,     0.,     1.);
    // clang-format on
}



template<typename _mat4>
_mat4 Translation(F32 x, F32 y, F32 z)
{
    static_assert(IsMat4<_mat4>(), "Template parameter is no valid 4x4 matrix type.");

    // clang-format off
    return _mat4(
        1.,     0.,     0.,     0 ,
        0.,     1.,     0.,     0 ,
        0.,     0.,     1.,     0 ,
        x ,     y ,     z ,     1.);
    // clang-format on
}



} // namespace GDL::TransformationMatrix4
