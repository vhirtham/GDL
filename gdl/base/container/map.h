#pragma once

#include <map>

#ifndef USE_STD_ALLOCATOR

#include "gdl/resources/memory/poolAllocator.h"
#include "gdl/resources/memory/stackAllocator.h"
#include "gdl/resources/memory/threadPrivateStackAllocator.h"

namespace GDL
{

template <typename _keyType, typename _valueType, typename _compare = std::less<_keyType>>
using Map = std::map<_keyType, _valueType, _compare, PoolAllocator<std::pair<const _keyType, _valueType>>>;

template <typename _keyType, typename _valueType, typename _compare = std::less<_keyType>>
using MapS = std::map<_keyType, _valueType, _compare, StackAllocator<std::pair<const _keyType, _valueType>>>;

template <typename _keyType, typename _valueType, typename _compare = std::less<_keyType>>
using MapTPS =
        std::map<_keyType, _valueType, _compare, ThreadPrivateStackAllocator<std::pair<const _keyType, _valueType>>>;
}

#else

namespace GDL
{

template <typename _keyType, typename _valueType, typename _compare = std::less<_keyType>>
using Map = std::map<_keyType, _valueType, _compare>;

template <typename _keyType, typename _valueType, typename _compare = std::less<_keyType>>
using MapS = std::map<_keyType, _valueType, _compare>;

template <typename _keyType, typename _valueType, typename _compare = std::less<_keyType>>
using MapTPS = std::map<_keyType, _valueType, _compare>;
}

#endif
