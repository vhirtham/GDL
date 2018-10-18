#pragma once

#include <set>

#ifndef USE_STD_ALLOCATOR

#include "gdl/resources/memory/poolAllocator.h"
#include "gdl/resources/memory/stackAllocator.h"
#include "gdl/resources/memory/threadPrivateStackAllocator.h"

namespace GDL
{

template <typename _keyType,  typename _compare = std::less<_keyType>>
using Set = std::set<_keyType,  _compare, PoolAllocator<_keyType>>;

template <typename _keyType,  typename _compare = std::less<_keyType>>
using SetS = std::set<_keyType,  _compare, StackAllocator<_keyType>>;

template <typename _keyType,  typename _compare = std::less<_keyType>>
using SetTPS =
        std::set<_keyType,  _compare, ThreadPrivateStackAllocator<_keyType>>;
}

#else

namespace GDL
{

template <typename _keyType,  typename _compare = std::less<_keyType>>
using Set = std::set<_keyType,  _compare>;

template <typename _keyType,  typename _compare = std::less<_keyType>>
using SetS = std::set<_keyType,  _compare>;

template <typename _keyType,  typename _compare = std::less<_keyType>>
using SetTPS = std::set<_keyType,  _compare>;
}

#endif
