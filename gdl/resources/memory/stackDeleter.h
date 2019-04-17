#pragma once

#include <type_traits>

namespace GDL
{

//! @brief Deleter class for smart pointers which use the memory stack.
//! @tparam _type: Data type of the object that should be deleted
template <typename _type>
class StackDeleter
{

    static_assert(!std::is_array<_type>::value,
                  "Stack deleter can't be used with arrays. Use a GDL::Vector instead of a GDL::UniquePtr.");

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
