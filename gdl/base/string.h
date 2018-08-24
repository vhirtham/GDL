#pragma once

#include "gdl/base/fundamentalTypes.h"

#include <array>
#include <limits>
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


namespace GDL
{
template <U32 _bufferSize = std::numeric_limits<F32>::max_exponent10 + 20>
String ToString(F32 value, const char* format = "%.4e")
{
    std::array<char, _bufferSize> buffer;
    std::snprintf(buffer.data(), buffer.size(), format, value);
    return String{buffer.data()};
}

template <U32 _bufferSize = std::numeric_limits<F64>::max_exponent10 + 20>
String ToString(F64 value, const char* format = "%.4e")
{
    std::array<char, _bufferSize> buffer;
    std::snprintf(buffer.data(), buffer.size(), format, value);
    return String{buffer.data()};
}

} // namespace GDL
