#pragma once

#include "gdl/base/fundamentalTypes.h"


namespace GDL
{

template <bool>
class Vec2fSSE;


//! @brief Checks the orientation of a 2d point c in relation to a line described by 2 points a and b. If the returned
//! value is lower than 0, the c lies to the right. If the value is greater than 0 c lies on the left hand side. In case
//! the value is 0 (considering floating point errors) the point lies on the line. Positive values also represent a
//! counter clockwise ordering if the 3 passed points are treated as triangle a-b-c. Negative values stand for clockwise
//! ordering. The value itself is twice the signed area of the triangle a-b-c.
//! @tparam _isCol: True if the passed vectors are column vectors, false otherwise
//! @param a: First point of the line a-b or the triangle a-b-c
//! @param b: Second point of the line a-b or the triangle a-b-c
//! @param c: Point that should be compared to the line or third point of the triangle a-b-c
//! @return See description
template <bool _isCol>
inline F32 Orientation(Vec2fSSE<_isCol> a, Vec2fSSE<_isCol> b, Vec2fSSE<_isCol> c);

} // namespace GDL


#include "gdl/physics/collision/functions/orientation.inl"
