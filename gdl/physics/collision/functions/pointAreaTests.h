#pragma once


#include "gdl/base/fundamentalTypes.h"

namespace GDL
{

template <bool>
class Vec2fSSE;

//! @brief Returns a positive value if the point lies inside the circle described by the three counter clockwise ordered
//! points c0, c1 and c2. If the returned value is 0, the point lies on the circles boundary. If the returned value is
//! negative, the point lies outside of the circle.
//! @tparam _isCol: True if the passed vectors are column vectors, false otherwise
//! @param point: Point that should be checked.
//! @param c0: First point that describes the circle
//! @param c1: Second point that describes the circle
//! @param c2: Third point that describes the circle
//! @return Positive values, if the point is inside the circle, 0 if it lies on the boundary and negative values
//! otherwise.
template <bool _isCol = true>
F32 PointInsideCircle(const Vec2fSSE<_isCol>& point, const Vec2fSSE<_isCol>& c0, const Vec2fSSE<_isCol>& c1,
                      const Vec2fSSE<_isCol>& c2);

} // namespace GDL


#include "gdl/physics/collision/functions/pointAreaTests.inl"
