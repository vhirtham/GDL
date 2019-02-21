#pragma once

#include "gdl/math/transformations4.h"

namespace GDL::Transformations4
{



template <typename _mat4>
_mat4 OrthogonalProjection(F32 width, F32 height)
{
    static_assert(IsMat4<_mat4>, "Template parameter is no valid 4x4 matrix type.");

    // clang-format off
    return _mat4(
              2./width,         0.,         0.,         0.,
                    0.,  2./height,         0.,         0.,
                    0.,         0.,        -1.,         0.,
                    0.,         0.,         0.,         1.);
    // clang-format on
}



template <typename _mat4>
_mat4 PerspectiveProjection(F32 fov, F32 width, F32 height, F32 zNear, F32 zFar)
{
    static_assert(IsMat4<_mat4>, "Template parameter is no valid 4x4 matrix type.");

    F32 aspectRatio = width / height;
    F32 f = 1.f / std::tan(fov * static_cast<F32>(M_PI) / 720.f);

    // clang-format off
    return _mat4(
        f / aspectRatio,        0.,                                 0.,         0.,
        0.,                     f ,                                 0.,         0.,
        0.,                     0.,    (zFar + zNear) / (zNear - zFar),        -1.,
        0.,                     0.,  2 * zFar * zNear / (zNear - zFar),         0.);
    // clang-format on
}



template <typename _mat4>
inline _mat4 RotationX(F32 angle)
{
    static_assert(IsMat4<_mat4>, "Template parameter is no valid 4x4 matrix type.");

    F32 sinAngle = std::sin(angle);
    F32 cosAngle = std::cos(angle);

    // clang-format off
    return _mat4(
               1.,         0.,        0.,       0.,
               0.,   cosAngle,  sinAngle,       0.,
               0.,  -sinAngle,  cosAngle,       0.,
               0.,         0.,        0.,       1.);
    // clang-format on
}



template <typename _mat4>
inline _mat4 RotationY(F32 angle)
{
    static_assert(IsMat4<_mat4>, "Template parameter is no valid 4x4 matrix type.");

    F32 sinAngle = std::sin(angle);
    F32 cosAngle = std::cos(angle);

    // clang-format off
    return _mat4(
         cosAngle,        0., -sinAngle,        0.,
               0.,        1.,        0.,        0.,
         sinAngle,        0.,  cosAngle,        0.,
               0.,        0.,        0.,        1.);
    // clang-format on
}



template <typename _mat4>
inline _mat4 RotationZ(F32 angle)
{
    static_assert(IsMat4<_mat4>, "Template parameter is no valid 4x4 matrix type.");

    F32 sinAngle = std::sin(angle);
    F32 cosAngle = std::cos(angle);

    // clang-format off
    return _mat4(
         cosAngle,  sinAngle,        0.,        0.,
        -sinAngle,  cosAngle,        0.,        0.,
               0.,        0.,        1.,        0.,
               0.,        0.,        0.,        1.);
    // clang-format on
}



template <typename _mat4>
_mat4 Scale(F32 x, F32 y, F32 z)
{
    static_assert(IsMat4<_mat4>, "Template parameter is no valid 4x4 matrix type.");

    // clang-format off
    return _mat4(
        x ,     0.,     0.,     0.,
        0.,     y ,     0.,     0.,
        0.,     0.,     z ,     0.,
        0.,     0.,     0.,     1.);
    // clang-format on
}



template <typename _mat4>
_mat4 Translation(F32 x, F32 y, F32 z)
{
    static_assert(IsMat4<_mat4>, "Template parameter is no valid 4x4 matrix type.");

    // clang-format off
    return _mat4(
        1.,     0.,     0.,     0.,
        0.,     1.,     0.,     0.,
        0.,     0.,     1.,     0.,
        x ,     y ,     z ,     1.);
    // clang-format on
}



template <typename _mat4>
_mat4 ScaleTranslate(F32 sx, F32 sy, F32 sz, F32 tx, F32 ty, F32 tz)
{
    static_assert(IsMat4<_mat4>, "Template parameter is no valid 4x4 matrix type.");

    // clang-format off
    return _mat4(
        sx,     0.,     0.,     0.,
        0.,     sy,     0.,     0.,
        0.,     0.,     sz,     0.,
        tx,     ty,     tz,     1.);
    // clang-format on
}



} // namespace GDL::Transformations4
