#pragma once

#include "gdl/base/exception.h"
#include "gdl/base/fundamentalTypes.h"

#include <array>
#include <limits>
#include <cstring>

#ifndef USE_STD_ALLOCATOR

#include "gdl/resources/memory/generalPurposeAllocator.h"
#include "gdl/resources/memory/stackAllocator.h"
#include "gdl/resources/memory/threadPrivateStackAllocator.h"

namespace GDL
{

using String = std::basic_string<char, std::char_traits<char>, GeneralPurposeAllocator<char>>;
using StringS = std::basic_string<char, std::char_traits<char>, StackAllocator<char>>;
using StringTPS = std::basic_string<char, std::char_traits<char>, ThreadPrivateStackAllocator<char>>;
} // namespace GDL

#else

namespace GDL
{

using String = std::basic_string<char>;
using StringS = std::basic_string<char>;
using StringTPS = std::basic_string<char>;
} // namespace GDL



#endif


namespace GDL
{

// Append -------------------------------------------------------------------------------------------------------------

template <typename _stringType, typename _appendedType>
void AppendToString(_stringType& string, const _appendedType& appendedText)
{
    string.append(appendedText);
}



template <typename _stringType, typename _appendedType, typename... _args>
void AppendToString(_stringType& string, const _appendedType& appendedText, const _args&... args)
{
    string.append(appendedText);
    AppendToString(string, args...);
}



// Make string ---------------------------------------------------------------------------------------------------------

template <typename _stringType = String, typename... _args>
_stringType MakeString(const _args&... args)
{
    _stringType string;
    AppendToString(string, args...);
    return string;
}


// GetFormatString ----------------------------------------------------------------------------------------------------


//! @brief Gets a format string that can be used with std::printf functions to cast a I32 into a string
//! @return Format string
constexpr const char* GetFormatStringI32()
{
    if constexpr (sizeof(long) == 4)
        return "%li";
    else if constexpr (sizeof(int) == 4)
        return "%i";
    else
        THROW("Could not determine std::snprintf format string for I32");
}



//! @brief Gets a format string that can be used with std::printf functions to cast a I64 into a string
//! @return Format string
constexpr const char* GetFormatStringI64()
{
    if constexpr (sizeof(long long) == 8)
        return "%lli";
    else if constexpr (sizeof(long) == 8)
        return "%li";
    else
        THROW("Could not determine std::snprintf format string for I32");
}



//! @brief Gets a format string that can be used with std::printf functions to cast a U32 into a string
//! @return Format string
constexpr const char* GetFormatStringU32()
{
    if constexpr (sizeof(unsigned long) == 4)
        return "%lu";
    else if constexpr (sizeof(unsigned int) == 4)
        return "%u";
    else
        THROW("Could not determine std::snprintf format string for I32");
}



//! @brief Gets a format string that can be used with std::printf functions to cast a U64 into a string
//! @return Format string
constexpr const char* GetFormatStringU64()
{
    if constexpr (sizeof(unsigned long long) == 8)
        return "%llu";
    else if constexpr (sizeof(unsigned long) == 8)
        return "%lu";
    else
        THROW("Could not determine std::snprintf format string for I32");
}



// ToString functions -------------------------------------------------------------------------------------------------

//! @brief Converts an F32 number into a human readable string
//! @tparam _bufferSize: Size of the temporary char buffer. Use the default value unless memory size is critical.
//! @param value: Number that should be converted
//! @param format: Char array to specify the output format. Have a look at the std::snprintf documentation.
template <U32 _bufferSize = std::numeric_limits<F32>::max_exponent10 + 20>
String ToString(F32 value, const char* format = "%.4f")
{
    std::array<char, _bufferSize> buffer;
    std::snprintf(buffer.data(), buffer.size(), format, value);
    return String{buffer.data()};
}



//! @brief Converts an F64 number into a human readable string
//! @tparam _bufferSize: Size of the temporary char buffer. Use the default value unless memory size is critical.
//! @param value: Number that should be converted
//! @param format: Char array to specify the output format. Have a look at the std::snprintf documentation.
template <U32 _bufferSize = std::numeric_limits<F64>::max_exponent10 + 20>
String ToString(F64 value, const char* format = "%.4f")
{
    std::array<char, _bufferSize> buffer;
    std::snprintf(buffer.data(), buffer.size(), format, value);
    return String{buffer.data()};
}



//! @brief Converts an I8 number into a human readable string
//! @tparam _bufferSize: Size of the temporary char buffer. Use the default value unless memory size is critical.
//! @param value: Number that should be converted
//! @param format: Char array to specify the output format. Have a look at the std::snprintf documentation.
template <U32 _bufferSize = 8>
String ToString(I8 value, const char* format = "%hhi")
{
    std::array<char, _bufferSize> buffer;
    std::snprintf(buffer.data(), buffer.size(), format, value);
    return String{buffer.data()};
}



//! @brief Converts an I16 number into a human readable string
//! @tparam _bufferSize: Size of the temporary char buffer. Use the default value unless memory size is critical.
//! @param value: Number that should be converted
//! @param format: Char array to specify the output format. Have a look at the std::snprintf documentation.
template <U32 _bufferSize = 8>
String ToString(I16 value, const char* format = "%hi")
{
    std::array<char, _bufferSize> buffer;
    std::snprintf(buffer.data(), buffer.size(), format, value);
    return String{buffer.data()};
}



//! @brief Converts an I32 number into a human readable string
//! @tparam _bufferSize: Size of the temporary char buffer. Use the default value unless memory size is critical.
//! @param value: Number that should be converted
//! @param format: Char array to specify the output format. Have a look at the std::snprintf documentation.
template <U32 _bufferSize = 16>
String ToString(I32 value, const char* format = GetFormatStringI32())
{
    std::array<char, _bufferSize> buffer;
    std::snprintf(buffer.data(), buffer.size(), format, value);
    return String{buffer.data()};
}



//! @brief Converts an I64 number into a human readable string
//! @tparam _bufferSize: Size of the temporary char buffer. Use the default value unless memory size is critical.
//! @param value: Number that should be converted
//! @param format: Char array to specify the output format. Have a look at the std::snprintf documentation.
template <U32 _bufferSize = 32>
String ToString(I64 value, const char* format = GetFormatStringI64())
{
    std::array<char, _bufferSize> buffer;
    std::snprintf(buffer.data(), buffer.size(), format, value);
    return String{buffer.data()};
}



//! @brief Converts an U8 number into a human readable string
//! @tparam _bufferSize: Size of the temporary char buffer. Use the default value unless memory size is critical.
//! @param value: Number that should be converted
//! @param format: Char array to specify the output format. Have a look at the std::snprintf documentation.
template <U32 _bufferSize = 8>
String ToString(U8 value, const char* format = "%hhu")
{
    std::array<char, _bufferSize> buffer;
    std::snprintf(buffer.data(), buffer.size(), format, value);
    return String{buffer.data()};
}



//! @brief Converts an U16 number into a human readable string
//! @tparam _bufferSize: Size of the temporary char buffer. Use the default value unless memory size is critical.
//! @param value: Number that should be converted
//! @param format: Char array to specify the output format. Have a look at the std::snprintf documentation.
template <U32 _bufferSize = 8>
String ToString(U16 value, const char* format = "%hu")
{
    std::array<char, _bufferSize> buffer;
    std::snprintf(buffer.data(), buffer.size(), format, value);
    return String{buffer.data()};
}



//! @brief Converts an U32 number into a human readable string
//! @tparam _bufferSize: Size of the temporary char buffer. Use the default value unless memory size is critical.
//! @param value: Number that should be converted
//! @param format: Char array to specify the output format. Have a look at the std::snprintf documentation.
template <U32 _bufferSize = 16>
String ToString(U32 value, const char* format = GetFormatStringU32())
{
    std::array<char, _bufferSize> buffer;
    std::snprintf(buffer.data(), buffer.size(), format, value);
    return String{buffer.data()};
}



//! @brief Converts an U64 number into a human readable string
//! @tparam _bufferSize: Size of the temporary char buffer. Use the default value unless memory size is critical.
//! @param value: Number that should be converted
//! @param format: Char array to specify the output format. Have a look at the std::snprintf documentation.
template <U32 _bufferSize = 32>
String ToString(U64 value, const char* format = GetFormatStringU64())
{
    std::array<char, _bufferSize> buffer;
    std::snprintf(buffer.data(), buffer.size(), format, value);
    return String{buffer.data()};
}
} // namespace GDL
