#pragma once

#include "gdl/resources/memory/memoryInterface.h"

#include <memory>

namespace GDL
{
//! @brief Allocator which allocates memory from the memory stack if it is available at the memory manager. If
//! not, general purpose memory or heap memory is used.
//! @tparam _type: Type of the allocated objects
template <class _type>
struct StackAllocator
{
    typedef _type value_type;

    //! @brief Ctor
    StackAllocator() noexcept;

    //! @brief Copy ctor
    template <class _typeOther>
    StackAllocator(const StackAllocator<_typeOther>&) noexcept;

    //! @brief Allocates memory for a given number of instances of the allocators template type
    //! @param numInstances: Scales the allocated memory in a way that the given number of instances of the allocators
    //! object type fit into the allocated memory block.
    //! @return Pointer to allocated memory
    _type* allocate(std::size_t numInstances);

    //! @brief Deallocates the memory starting at the passed pointers address
    //! @param pointer: Pointer to the first element of a memory block which should be deallocated
    void deallocate(_type* pointer, std::size_t);

    //! @brief Deleter for smart pointers
    //! @param pointer: Pointer to the first element of a memory block which should be deallocated
    static void Deleter(_type* pointer);

private:
    //! @brief Gets the memory allocation pattern. This is initialized once and stored as a static variable. Have a look
    //! at the InitializeMemoryAllocationPattern for further explanation.
    //! @return Memory allocation pattern
    static inline MemoryInterface* GetMemoryAllocationPattern();

    //! @brief This function is used to initialize the static variable of the GetMemoryAllocationPattern method. The
    //! returned memory allocation pattern depends on the available resources of the memory manager. If multiple
    //! patterns are available the choice is made in the following order:
    //! memory stack -> general purpose memory -> heap memory
    static MemoryInterface* InitializeMemoryAllocationPattern();
};

//! Comparison operator for two stack allocators with different object types
template <class _type, class _typeOther>
constexpr bool operator==(const StackAllocator<_type>&, const StackAllocator<_typeOther>&) noexcept;

//! Comparison operator for two stack allocators with different object types
template <class _type, class _typeOther>
constexpr bool operator!=(const StackAllocator<_type>&, const StackAllocator<_typeOther>&) noexcept;


} // namespace GDL


#include "gdl/resources/memory/stackAllocator.inl"
