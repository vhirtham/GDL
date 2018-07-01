#pragma once

#include <cstddef>

namespace GDL
{

//! @brief Determines if the passed number is a power of 2
//! @param x: Number that should be checked
//! @return TRUE / FALSE
inline bool IsPowerOf2(size_t x)
{
    return x > 0 && !(x & (x - 1));
}
}
