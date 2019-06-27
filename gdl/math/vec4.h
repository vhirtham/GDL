#pragma once

#include "gdl/math/simd/vec4fSSE.h"

namespace GDL
{

using Vec4f = Vec4fSSE<true>;
using Vec4rf = Vec4fSSE<false>;

} // namespace GDL
