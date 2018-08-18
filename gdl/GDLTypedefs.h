#pragma once



#include <cstdint>
#include <limits.h>

// rename header to numeric types

namespace GDL
{

// Integers
// --------

typedef std::int8_t I8;
typedef std::int16_t I16;
typedef std::int32_t I32;
typedef std::int64_t I64;

typedef std::uint8_t U8;
typedef std::uint16_t U16;
typedef std::uint32_t U32;
typedef std::uint64_t U64;

// Floats
// ------

typedef float F32;
typedef double F64;


static_assert(sizeof(float) * CHAR_BIT == 32, "float  is not 32 bit on this architecture, fix the F32 typedef.");
static_assert(sizeof(double) * CHAR_BIT == 64, "double is not 64 bit on this architecture, fix the F64 typedef.");
}
