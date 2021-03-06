#pragma once

#include <type_traits>

namespace GDL
{

//! @brief Deleter class for smart pointers which use the general purpose memory.
//! @tparam _type: Data type of the object that should be deleted
template <typename _type>
class GeneralPurposeDeleter
{
    static_assert(!std::is_array<_type>::value,
                  "General purpose deleter can't be used with arrays. Use a GDL::Vector instead of a GDL::UniquePtr.");

public:
    GeneralPurposeDeleter() = default;
    template <typename _typeOther>
    GeneralPurposeDeleter(const GeneralPurposeDeleter<_typeOther>&);
    template <typename _typeOther>
    GeneralPurposeDeleter(GeneralPurposeDeleter<_typeOther>&&);
    template <typename _typeOther>
    GeneralPurposeDeleter& operator=(const GeneralPurposeDeleter<_typeOther>&);
    template <typename _typeOther>
    GeneralPurposeDeleter& operator=(GeneralPurposeDeleter<_typeOther>&&);
    ~GeneralPurposeDeleter() = default;



    //! @brief Destroys the passed opject and deallocates the memory
    //! @param pointer: Pointer to object that should be destroyed
    void operator()(_type* pointer);
};



} // namespace GDL


#include "gdl/resources/memory/generalPurposeDeleter.inl"
