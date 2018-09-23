#pragma once

#include "gdl/resources/memory/memoryInterface.h"

#include <memory>

namespace GDL
{

//! @brief Allocator which allocates memory from a thread private memory stack if it is available at the memory manager.
//! If not, general purpose memory or heap memory is used.
//! @tparam _type: Type of the allocated objects
template <class _type>
class ThreadPrivateStackAllocator
{
    template <typename _type2>
    friend class ThreadPrivateStackDeleter;


    MemoryInterface& mMemoryAllocationPattern;

public:
    typedef _type value_type;

    //! @brief Ctor
    ThreadPrivateStackAllocator() noexcept;

    //! @brief Copy ctor
    template <class _typeOther>
    ThreadPrivateStackAllocator(const ThreadPrivateStackAllocator<_typeOther>& other) noexcept;

    //! @brief Allocates memory for a given number of instances of the allocators template type
    //! @param numInstances: Scales the allocated memory in a way that the given number of instances of the allocators
    //! object type fit into the allocated memory block.
    //! @return Pointer to allocated memory
    _type* allocate(std::size_t numInstances);

    //! @brief Deallocates the memory starting at the passed pointers address
    //! @param pointer: Pointer to the first element of a memory block which should be deallocated
    void deallocate(_type* pointer, std::size_t);

private:
    //! @brief If thread private stacks are enabled at the memory manager, this function returns the current threads
    //! memory stack or throws, if one forgot to create one for the current thread. If thread private stacks are
    //! disabled, this function initializes a static variable with an alternative allocation pattern and returns it.
    //! Have a look at InitializeAlternativeMemoryAllocationPattern for more details.
    //! @return Memory allocation pattern
    static inline MemoryInterface* GetMemoryAllocationPattern();

    //! @brief This function is used to initialize the static variable of the GetMemoryAllocationPattern method if
    //! thread private stacks are disabled. The returned memory allocation pattern depends on the available resources of
    //! the memory manager. If multiple patterns are available the choice is made in the following order:
    //! general purpose memory -> heap memory
    static MemoryInterface* InitializeAlternativeMemoryAllocationPattern();
};

//! Comparison operator for two thread private stack allocators with different object types
template <class _type, class _typeOther>
constexpr bool operator==(const ThreadPrivateStackAllocator<_type>&,
                          const ThreadPrivateStackAllocator<_typeOther>&) noexcept;

//! Comparison operator for two thread private stack allocators with different object types
template <class _type, class _typeOther>
constexpr bool operator!=(const ThreadPrivateStackAllocator<_type>&,
                          const ThreadPrivateStackAllocator<_typeOther>&) noexcept;


} // namespace GDL


#include "gdl/resources/memory/threadPrivateStackAllocator.inl"
