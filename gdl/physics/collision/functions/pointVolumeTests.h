#pragma once


#include "gdl/base/fundamentalTypes.h"

namespace GDL
{

template <bool>
class Vec3fSSE;

//! @brief Returns a positive value if the point lies inside the sphere described by the four points c0, c1 c2 and c3.
//! If the returned value is 0, the point lies on the spheres boundary. If the returned value is negative, the point
//! lies outside of the sphere. The four points c0, c1, c2 anf c3 must be oriented in such a way that the Orientation
//! function defined in orientation.h returns positive values for those four points.
//! @tparam _isCol: True if the passed vectors are column vectors, false otherwise
//! @param point: Point that should be checked.
//! @param c0: First point that describes the sphere
//! @param c1: Second point that describes the sphere
//! @param c2: Third point that describes the sphere
//! @param c3: Fourth point that describes the sphere
//! @return Positive values, if the point is inside the sphere, 0 if it lies on the boundary and negative values
//! otherwise.
template <bool _isCol = true>
F32 PointInsideSphere(const Vec3fSSE<_isCol>& point, const Vec3fSSE<_isCol>& c0, const Vec3fSSE<_isCol>& c1,
                      const Vec3fSSE<_isCol>& c2, const Vec3fSSE<_isCol>& c3);

} // namespace GDL


#include "gdl/physics/collision/functions/pointVolumeTests.inl"
