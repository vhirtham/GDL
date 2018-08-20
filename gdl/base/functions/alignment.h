#pragma once

#include <cstddef>
#include <cstdint>

namespace GDL
{

//! @brief Calculates the misalignment in bytes of the passed pointer
//! @param pointer: Pointer which should be checked
//! @param alignment: Desired alignment
//! @return Misalignment
inline size_t Misalignment(const volatile void* pointer, size_t alignment)
{
    return reinterpret_cast<std::uintptr_t>(pointer) % alignment;
}




//! @brief Checks if a passed pointer is aligned as specified
//! @param pointer: Pointer which should be checked
//! @param alignment: Desired alignment
//! @return TRUE / FALSE
inline bool IsAligned(const volatile void* pointer, size_t alignment)
{
    return Misalignment(pointer,alignment) % alignment == 0;
}

} // namespace GDL
