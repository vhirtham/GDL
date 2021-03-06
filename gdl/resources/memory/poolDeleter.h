#pragma once

#include <type_traits>

namespace GDL
{

//! @brief Deleter class for smart pointers which use the memory pool.
//! @tparam _type: Data type of the object that should be deleted
template <typename _type>
class PoolDeleter
{

    static_assert(!std::is_array<_type>::value,
                  "Pool deleter can't be used with arrays. Use a GDL::Vector instead of a GDL::UniquePtr.");

public:
    PoolDeleter() = default;
    template <typename _typeOther>
    PoolDeleter(const PoolDeleter<_typeOther>&);
    template <typename _typeOther>
    PoolDeleter(PoolDeleter<_typeOther>&&);
    template <typename _typeOther>
    PoolDeleter& operator=(const PoolDeleter<_typeOther>&);
    template <typename _typeOther>
    PoolDeleter& operator=(PoolDeleter<_typeOther>&&);
    ~PoolDeleter() = default;



    //! @brief Destroys the passed opject and deallocates the memory
    //! @param pointer: Pointer to object that should be destroyed
    void operator()(_type* pointer);
};

} // namespace GDL


#include "gdl/resources/memory/poolDeleter.inl"
