#pragma once

#include "gdl/math/transformations3.h"

namespace GDL::Transformations3
{




template <typename _mat3>
inline _mat3 RotationX(F32 angle)
{
    static_assert(IsMat3<_mat3>, "Template parameter is no valid 3x3 matrix type.");

    F32 sinAngle = std::sin(angle);
    F32 cosAngle = std::cos(angle);

    // clang-format off
    return _mat3(
               1.,         0.,        0.,
               0.,   cosAngle,  sinAngle,
               0.,  -sinAngle,  cosAngle);
    // clang-format on
}



template <typename _mat3>
inline _mat3 RotationY(F32 angle)
{
    static_assert(IsMat3<_mat3>, "Template parameter is no valid 3x3 matrix type.");

    F32 sinAngle = std::sin(angle);
    F32 cosAngle = std::cos(angle);

    // clang-format off
    return _mat3(
         cosAngle,        0., -sinAngle,
               0.,        1.,        0.,
         sinAngle,        0.,  cosAngle);
    // clang-format on
}



template <typename _mat3>
inline _mat3 RotationZ(F32 angle)
{
    static_assert(IsMat3<_mat3>, "Template parameter is no valid 3x3 matrix type.");

    F32 sinAngle = std::sin(angle);
    F32 cosAngle = std::cos(angle);

    // clang-format off
    return _mat3(
         cosAngle,  sinAngle,        0.,
        -sinAngle,  cosAngle,        0.,
               0.,        0.,        1.);
    // clang-format on
}



template <typename _mat3>
_mat3 Scale(F32 x, F32 y, F32 z)
{
    static_assert(IsMat3<_mat3>, "Template parameter is no valid 3x3 matrix type.");

    // clang-format off
    return _mat3(
        x ,     0.,     0.,
        0.,     y ,     0.,
        0.,     0.,     z );
    // clang-format on
}


} // namespace GDL::Transformations3
