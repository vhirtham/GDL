#pragma once

#include <string>

#ifndef USE_STD_ALLOCATOR

#include "gdl/resources/memory/generalPurposeAllocator.h"
#include "gdl/resources/memory/stackAllocator.h"
#include "gdl/resources/memory/threadPrivateStackAllocator.h"

namespace GDL
{

using String = std::basic_string<char, std::char_traits<char>, GeneralPurposeAllocator<char>>;
using StringS = std::basic_string<char, std::char_traits<char>, StackAllocator<char>>;
using StringTPS = std::basic_string<char, std::char_traits<char>, ThreadPrivateStackAllocator<char>>;

}

#else

namespace GDL
{

using String = std::basic_string<char>;
using StringS = std::basic_string<char>;
using StringTPS = std::basic_string<char>;

}

#endif
