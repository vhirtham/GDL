#pragma once

#include <memory>

#ifndef USE_STD_ALLOCATOR

#include "gdl/resources/memory/generalPurposeDeleter.h"
#include "gdl/resources/memory/poolAllocator.h"
#include "gdl/resources/memory/stackAllocator.h"
#include "gdl/resources/memory/threadPrivateStackAllocator.h"

namespace GDL
{
template <typename _type>
using UniquePtr = std::unique_ptr<_type, GeneralPurposeDeleter<_type>>;

template <typename _type>
using UniquePtrP = std::unique_ptr<_type, decltype(&PoolAllocator<_type>::Deleter)>;

template <typename _type>
using UniquePtrS = std::unique_ptr<_type, decltype(&StackAllocator<_type>::Deleter)>;

template <typename _type>
using UniquePtrTPS = std::unique_ptr<_type, Deleter<_type>>;

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
    _type* ptr = Allocator.allocate(1);
    *ptr = _type(std::forward<_args>(args)...);
    return UniquePtrP<_type>(ptr, &PoolAllocator<_type>::Deleter);
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
    _type* ptr = Allocator.allocate(1);
    *ptr = _type(std::forward<_args>(args)...);
    return UniquePtrS<_type>(ptr, &StackAllocator<_type>::Deleter);
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
    _type* ptr = Allocator.allocate(1);
    *ptr = _type(std::forward<_args>(args)...);
    return UniquePtrTPS<_type>(ptr, Allocator.GetDeleter());
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
