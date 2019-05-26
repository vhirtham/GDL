#pragma once

#include <forward_list>

#ifndef USE_STD_ALLOCATOR

#include "gdl/resources/memory/poolAllocator.h"
#include "gdl/resources/memory/stackAllocator.h"
#include "gdl/resources/memory/threadPrivateStackAllocator.h"

namespace GDL
{

template <typename _type>
using ForwardList = std::forward_list<_type, PoolAllocator<_type>>;

template <typename _type>
using ForwardListS = std::forward_list<_type, StackAllocator<_type>>;

template <typename _type>
using ForwardListTPS = std::forward_list<_type, ThreadPrivateStackAllocator<_type>>;
}

#else

namespace GDL
{

template <typename _type>
using ForwardList = std::forward_list<_type>;

template <typename _type>
using ForwardListS = std::forward_list<_type>;

template <typename _type>
using ForwardListTPS = std::forward_list<_type>;
}

#endif
