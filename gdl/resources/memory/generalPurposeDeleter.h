#pragma once


#include "gdl/resources/memory/generalPurposeAllocator.h"


namespace GDL
{

//! @brief GeneralPurposeDeleter class for smart pointers.
//! @tparam _type: Data type of the pointer
template <typename _type>
class GeneralPurposeDeleter
{

public:
    GeneralPurposeDeleter() = default;
    GeneralPurposeDeleter(const GeneralPurposeDeleter&) = default;
    GeneralPurposeDeleter(GeneralPurposeDeleter&&) = default;
    GeneralPurposeDeleter& operator=(const GeneralPurposeDeleter&) = default;
    GeneralPurposeDeleter& operator=(GeneralPurposeDeleter&&) = default;
    ~GeneralPurposeDeleter() = default;


    //! @brief Deallocates the passed pointer and destroys the corresponding object
    //! @param pointer: Pointer that should be deallocated
    void operator()(_type* pointer)
    {
        pointer->~_type();
        GeneralPurposeAllocator<_type>::GetMemoryAllocationPattern()->Deallocate(pointer, alignof(_type));
    }
};

} // namespace GDL
