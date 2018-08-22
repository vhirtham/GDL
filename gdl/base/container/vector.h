#pragma once

#include <vector>

#ifndef USE_STD_ALLOCATOR

#include "gdl/resources/memory/generalPurposeAllocator.h"
#include "gdl/resources/memory/stackAllocator.h"
#include "gdl/resources/memory/threadPrivateStackAllocator.h"

namespace GDL
{

template <typename _type>
using Vector = std::vector<_type,GeneralPurposeAllocator<_type>>;

template <typename _type>
using VectorS = std::vector<_type,StackAllocator<_type>>;

template <typename _type>
using VectorTPS = std::vector<_type,ThreadPrivateStackAllocator<_type>>;

}

#else

namespace GDL
{

template <typename _type>
using Vector = std::vector<_type>;

template <typename _type>
using VectorS = std::vector<_type>;

template <typename _type>
using VectorTPS = std::vector<_type>;

}

#endif
