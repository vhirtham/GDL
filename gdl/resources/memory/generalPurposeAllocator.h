#pragma once

#include "gdl/resources/memory/memoryInterface.h"

#include <memory>

namespace GDL
{


//! @brief Allocator which allocates memory from general purpose memory if it is available at the memory manager. If
//! not, heap memory is used.
//! @tparam _type: Type of the allocated objects
template <class _type>
struct GeneralPurposeAllocator
{
    template <typename _type2>
    friend class GeneralPurposeDeleter;

    typedef _type value_type;

    //! @brief Ctor
    GeneralPurposeAllocator() noexcept;

    //! @brief Copy ctor
    template <class _typeOther>
    GeneralPurposeAllocator(const GeneralPurposeAllocator<_typeOther>&) noexcept;

    //! @brief Allocates memory for a given number of instances of the allocators template type
    //! @param numInstances: Scales the allocated memory in a way that the given number of instances of the allocators
    //! object type fit into the allocated memory block.
    //! @return Pointer to allocated memory
    _type* allocate(std::size_t numInstances);

    //! @brief Deallocates the memory starting at the passed pointers address
    //! @param pointer: Pointer to the first element of a memory block which should be deallocated
    void deallocate(_type* pointer, std::size_t);

private:
    //! @brief Gets the memory allocation pattern. This is initialized once and stored as a static variable. Have a look
    //! at the InitializeMemoryAllocationPattern for further explanation.
    //! @return Memory allocation pattern
    static inline MemoryInterface* GetMemoryAllocationPattern();

    //! @brief This function is used to initialize the static variable of the GetMemoryAllocationPattern method. The
    //! returned memory allocation pattern depends on the available resources of the memory manager. If multiple
    //! patterns are available the choice is made in the following order:
    //! general purpose memory -> heap memory
    static MemoryInterface* InitializeMemoryAllocationPattern();
};

//! Comparison operator for two general purpose allocators with different object types
template <class _type, class _typeOther>
constexpr bool operator==(const GeneralPurposeAllocator<_type>&, const GeneralPurposeAllocator<_typeOther>&) noexcept;

//! Comparison operator for two general purpose allocators with different object types
template <class _type, class _typeOther>
constexpr bool operator!=(const GeneralPurposeAllocator<_type>&, const GeneralPurposeAllocator<_typeOther>&) noexcept;


} // namespace GDL


#include "gdl/resources/memory/generalPurposeAllocator.inl"
