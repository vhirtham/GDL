#pragma once

#include "gdl/base/functions/pow.h"

#include <cassert>
#include <limits>
#include <memory>

namespace GDL
{

constexpr inline size_t operator""_B(unsigned long long lhs)
{
    assert(std::numeric_limits<size_t>::max() >= lhs);
    return static_cast<size_t>(lhs) * Pow<0, size_t>(1000);
}

constexpr inline size_t operator""_kB(unsigned long long lhs)
{
    assert(std::numeric_limits<size_t>::max() >= lhs);
    return static_cast<size_t>(lhs) * Pow<1, size_t>(1000);
}

constexpr inline size_t operator""_MB(unsigned long long lhs)
{
    assert(std::numeric_limits<size_t>::max() >= lhs);
    return static_cast<size_t>(lhs) * Pow<2, size_t>(1000);
}

constexpr inline size_t operator""_GB(unsigned long long lhs)
{
    assert(std::numeric_limits<size_t>::max() >= lhs);
    return static_cast<size_t>(lhs) * Pow<3, size_t>(1000);
}

constexpr inline size_t operator""_TB(unsigned long long lhs)
{
    assert(std::numeric_limits<size_t>::max() >= lhs);
    return static_cast<size_t>(lhs) * Pow<4, size_t>(1000);
}

constexpr inline size_t operator""_kiB(unsigned long long lhs)
{
    assert(std::numeric_limits<size_t>::max() >= lhs);
    return static_cast<size_t>(lhs) * Pow<1, size_t>(1024);
}

constexpr inline size_t operator""_MiB(unsigned long long lhs)
{
    assert(std::numeric_limits<size_t>::max() >= lhs);
    return static_cast<size_t>(lhs) * Pow<2, size_t>(1024);
}

constexpr inline size_t operator""_GiB(unsigned long long lhs)
{
    assert(std::numeric_limits<size_t>::max() >= lhs);
    return static_cast<size_t>(lhs) * Pow<3, size_t>(1024);
}

constexpr inline size_t operator""_TiB(unsigned long long lhs)
{
    assert(std::numeric_limits<size_t>::max() >= lhs);
    return static_cast<size_t>(lhs) * Pow<4, size_t>(1024);
}

} // namespace GDL
