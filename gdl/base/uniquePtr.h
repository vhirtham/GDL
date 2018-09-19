#pragma once

#include <memory>

#ifndef USE_STD_ALLOCATOR

#include "gdl/resources/memory/generalPurposeAllocator.h"
#include "gdl/resources/memory/stackAllocator.h"
#include "gdl/resources/memory/threadPrivateStackAllocator.h"

namespace GDL
{
template <typename _type>
using UniquePtr = std::unique_ptr<_type, decltype (&GeneralPurposeAllocator<_type>::Deleter)>;


//! @brief Creates a new unique pointer which uses the general purpose memory
//! @tparam _type: Type of the pointer
//! @tparam _args: Parameter pack of the arguments that should be passed to the managed types constructor
//! @param args: Arguments that should be passed to the types constructor
//! @return Unique pointer which uses the general purpose memory
template <typename _type,typename... _args>
inline UniquePtr<_type> MakeUnique(_args&&... args)
{
    static GeneralPurposeAllocator<_type> Allocator;
    _type* ptr = Allocator.allocate(1);
    *ptr = _type(std::forward<_args>(args)...);
    return UniquePtr<_type>(ptr,&GeneralPurposeAllocator<_type>::Deleter);
}
}

#else

namespace GDL
{

template <typename _type>
using UniquePtr = std::unique_ptr<_type>;


//! @brief Creates a new unique pointer
//! @tparam _type: Type of the pointer
//! @tparam _args: Parameter pack of the arguments that should be passed to the managed types constructor
//! @param args: Arguments that should be passed to the types constructor
//! @return Unique pointer
template <typename _type,typename... _args>
inline UniquePtr<_type> MakeUnique(_args&&... args)
{
    return std::make_unique<_type>(std::forward<_args>(args)...);
}

}



#endif
