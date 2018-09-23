#pragma once


namespace GDL
{

//! @brief StackDeleter class for smart pointers.
//! @tparam _type: Data type of the object that should be deleted
template <typename _type>
class StackDeleter
{

public:
    StackDeleter() = default;
    template <typename _typeOther>
    StackDeleter(const StackDeleter<_typeOther>&);
    template <typename _typeOther>
    StackDeleter(StackDeleter<_typeOther>&&);
    template <typename _typeOther>
    StackDeleter& operator=(const StackDeleter<_typeOther>&);
    template <typename _typeOther>
    StackDeleter& operator=(StackDeleter<_typeOther>&&);
    ~StackDeleter() = default;



    //! @brief Destroys the passed opject and deallocates the memory
    //! @param pointer: Pointer to object that should be destroyed
    void operator()(_type* pointer);
};

} // namespace GDL


#include "gdl/resources/memory/stackDeleter.inl"
