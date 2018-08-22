#pragma once



#include <cstdint>
#include <limits.h>

namespace GDL
{

// Integers
// --------

using I8 = std::int8_t;
using I16 = std::int16_t;
using I32 = std::int32_t;
using I64 = std::int64_t;

using U8 = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;

// Floats
// ------

using F32 = float;
using F64 = double;


static_assert(sizeof(float) * CHAR_BIT == 32, "float  is not 32 bit on this architecture, fix the F32 typedef.");
static_assert(sizeof(double) * CHAR_BIT == 64, "double is not 64 bit on this architecture, fix the F64 typedef.");
}
