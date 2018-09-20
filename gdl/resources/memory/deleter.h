#pragma once


#include "gdl/resources/memory/memoryInterface.h"


namespace GDL
{

//! @brief Deleter class for smart pointers. It stores a reference to the memory allocation pattern which should
//! deallocate the pointer.
//! @tparam _type: Data type of the pointer
template <typename _type>
class Deleter
{
    MemoryInterface& mMemoryAllocationPattern;

public:
    Deleter() = delete;
    Deleter(const Deleter&) = default;
    Deleter(Deleter&&) = default;
    Deleter& operator=(const Deleter&) = default;
    Deleter& operator=(Deleter&&) = default;
    ~Deleter() = default;

    //! @brief ctor
    //! @param memoryAllocationPattern: Memory allocation pattern which should deallocate the pointer
    Deleter(MemoryInterface& memoryAllocationPattern)
        : mMemoryAllocationPattern{memoryAllocationPattern}
    {
    }

    //! @brief Deallocates the passed pointer
    //! @param pointer: Pointer that should be deallocated
    void operator()(_type* pointer)
    {
        mMemoryAllocationPattern.Deallocate(pointer, alignof(_type));
    }
};

} // namespace GDL
