#pragma once

#include <memory>

#ifndef USE_STD_ALLOCATOR

#include "gdl/resources/memory/generalPurposeDeleter.h"
#include "gdl/resources/memory/poolDeleter.h"
#include "gdl/resources/memory/stackDeleter.h"
#include "gdl/resources/memory/threadPrivateStackDeleter.h"

namespace GDL
{
template <typename _type>
using UniquePtr = std::unique_ptr<_type, GeneralPurposeDeleter<_type>>;

template <typename _type>
using UniquePtrP = std::unique_ptr<_type, PoolDeleter<_type>>;

template <typename _type>
using UniquePtrS = std::unique_ptr<_type, StackDeleter<_type>>;

template <typename _type>
using UniquePtrTPS = std::unique_ptr<_type, ThreadPrivateStackDeleter<_type>>;

} // namespace GDL

#else

namespace GDL
{

template <typename _type>
using UniquePtr = std::unique_ptr<_type>;

template <typename _type>
using UniquePtrP = std::unique_ptr<_type>;

template <typename _type>
using UniquePtrS = std::unique_ptr<_type>;

template <typename _type>
using UniquePtrTPS = std::unique_ptr<_type>;

} // namespace GDL



#endif


// MakeUnique functions -----------------------------------------------------------------------------------------------

namespace GDL
{

#ifndef USE_STD_ALLOCATOR


//! @brief Creates a new unique pointer which uses the general purpose memory
//! @tparam _type: Type of the unique pointer
//! @tparam _args: Parameter pack of the arguments that should be passed to the managed types constructor
//! @param args: Arguments that should be passed to the types constructor
//! @return Unique pointer which uses the general purpose memory
template <typename _type, typename... _args>
inline UniquePtr<_type> MakeUnique(_args&&... args)
{
    static GeneralPurposeAllocator<_type> Allocator;
    return UniquePtr<_type>(new (Allocator.allocate(1)) _type(std::forward<_args>(args)...));
}

//! @brief Creates a new unique pointer which uses the pool memory
//! @tparam _type: Type of the pointer
//! @tparam _args: Parameter pack of the arguments that should be passed to the managed types constructor
//! @param args: Arguments that should be passed to the types constructor
//! @return Unique pointer which uses the pool memory
template <typename _type, typename... _args>
inline UniquePtrP<_type> MakeUniqueP(_args&&... args)
{
    static PoolAllocator<_type> Allocator;
    return UniquePtrP<_type>(new (Allocator.allocate(1)) _type(std::forward<_args>(args)...));
}



//! @brief Creates a new unique pointer which uses the memory stack
//! @tparam _type: Type of the pointer
//! @tparam _args: Parameter pack of the arguments that should be passed to the managed types constructor
//! @param args: Arguments that should be passed to the types constructor
//! @return Unique pointer which uses the memory stack
template <typename _type, typename... _args>
inline UniquePtrS<_type> MakeUniqueS(_args&&... args)
{
    static StackAllocator<_type> Allocator;
    return UniquePtrS<_type>(new (Allocator.allocate(1)) _type(std::forward<_args>(args)...));
}



//! @brief Creates a new unique pointer which uses the thread private memory stack
//! @tparam _type: Type of the pointer
//! @tparam _args: Parameter pack of the arguments that should be passed to the managed types constructor
//! @param args: Arguments that should be passed to the types constructor
//! @return Unique pointer which uses the thread private memory stack
template <typename _type, typename... _args>
inline UniquePtrTPS<_type> MakeUniqueTPS(_args&&... args)
{
    ThreadPrivateStackAllocator<_type> Allocator;
    return UniquePtrTPS<_type>(new (Allocator.allocate(1)) _type(std::forward<_args>(args)...), Allocator);
}



#else

//! @brief Creates a new unique pointer
//! @tparam _type: Type of the pointer
//! @tparam _args: Parameter pack of the arguments that should be passed to the managed types constructor
//! @param args: Arguments that should be passed to the types constructor
//! @return Unique pointer
template <typename _type, typename... _args>
inline UniquePtr<_type> MakeUnique(_args&&... args)
{
    return std::make_unique<_type>(std::forward<_args>(args)...);
}



//! @brief Creates a new unique pointer
//! @tparam _type: Type of the pointer
//! @tparam _args: Parameter pack of the arguments that should be passed to the managed types constructor
//! @param args: Arguments that should be passed to the types constructor
//! @return Unique pointer
template <typename _type, typename... _args>
inline UniquePtrP<_type> MakeUniqueP(_args&&... args)
{
    return std::make_unique<_type>(std::forward<_args>(args)...);
}



//! @brief Creates a new unique pointer
//! @tparam _type: Type of the pointer
//! @tparam _args: Parameter pack of the arguments that should be passed to the managed types constructor
//! @param args: Arguments that should be passed to the types constructor
//! @return Unique pointer
template <typename _type, typename... _args>
inline UniquePtrS<_type> MakeUniqueS(_args&&... args)
{
    return std::make_unique<_type>(std::forward<_args>(args)...);
}



//! @brief Creates a new unique pointer
//! @tparam _type: Type of the pointer
//! @tparam _args: Parameter pack of the arguments that should be passed to the managed types constructor
//! @param args: Arguments that should be passed to the types constructor
//! @return Unique pointer
template <typename _type, typename... _args>
inline UniquePtrTPS<_type> MakeUniqueTPS(_args&&... args)
{
    return std::make_unique<_type>(std::forward<_args>(args)...);
}
#endif

} // namespace GDL
