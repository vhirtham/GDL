#pragma once

#include "gdl/base/fundamentalTypes.h"


namespace GDL
{

template <bool>
class  Vec2fSSE;

template<bool _isCol>
inline F32 Orientation(Vec2fSSE<_isCol> a,Vec2fSSE<_isCol> b,Vec2fSSE<_isCol> c);

} // namespace GDL


#include "gdl/physics/collision/functions/orientation.inl"
