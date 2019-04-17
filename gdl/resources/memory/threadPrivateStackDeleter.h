#pragma once

#include "gdl/resources/memory/memoryInterface.h"
#include "gdl/resources/memory/threadPrivateStackAllocator.h"

#include <type_traits>

namespace GDL
{

//! @brief Deleter class for smart pointers which use the thread private memory stack.
//! @tparam _type: Data type of the object that should be deleted
template <typename _type>
class ThreadPrivateStackDeleter
{
    static_assert(
            !std::is_array<_type>::value,
            "Thread private stack deleter can't be used with arrays. Use a GDL::Vector instead of a GDL::UniquePtr.");

    MemoryInterface& mMemoryAllocationPattern;

public:
    ThreadPrivateStackDeleter();
    template <typename _typeOther>
    ThreadPrivateStackDeleter(const ThreadPrivateStackDeleter<_typeOther>&);
    template <typename _typeOther>
    ThreadPrivateStackDeleter(ThreadPrivateStackDeleter<_typeOther>&&);
    template <typename _typeOther>
    ThreadPrivateStackDeleter& operator=(const ThreadPrivateStackDeleter<_typeOther>&);
    template <typename _typeOther>
    ThreadPrivateStackDeleter& operator=(ThreadPrivateStackDeleter<_typeOther>&&);
    ~ThreadPrivateStackDeleter() = default;

    //! @brief ctor
    //! @tparam _typeAllocator: Data type of the allocator
    //! @param threadPrivateStackAllocator: Allocator that was used to create the object that should be deleted by the
    //! deleter
    template <typename _typeAllocator>
    ThreadPrivateStackDeleter(ThreadPrivateStackAllocator<_typeAllocator>& threadPrivateStackAllocator);

    //! @brief Destroys the passed opject and deallocates the memory
    //! @param pointer: Pointer to object that should be destroyed
    void operator()(_type* pointer);
};

} // namespace GDL


#include "gdl/resources/memory/threadPrivateStackDeleter.inl"
