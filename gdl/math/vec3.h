#pragma once

#include "gdl/math/simd/vec3fSSE.h"

namespace GDL
{

using Vec3f = Vec3fSSE<true>;
using Vec3rf = Vec3fSSE<false>;

} // namespace GDL


