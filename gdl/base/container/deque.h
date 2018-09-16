#pragma once

#include <deque>

#ifndef USE_STD_ALLOCATOR

#include "gdl/resources/memory/generalPurposeAllocator.h"
#include "gdl/resources/memory/stackAllocator.h"
#include "gdl/resources/memory/threadPrivateStackAllocator.h"

namespace GDL
{

template <typename _type>
using Deque = std::deque<_type, GeneralPurposeAllocator<_type>>;

template <typename _type>
using DequeS = std::deque<_type, StackAllocator<_type>>;

template <typename _type>
using DequeTPS = std::deque<_type, ThreadPrivateStackAllocator<_type>>;
}

#else

namespace GDL
{

template <typename _type>
using Deque = std::deque<_type>;

template <typename _type>
using DequeS = std::deque<_type>;

template <typename _type>
using DequeTPS = std::deque<_type>;
}

#endif
