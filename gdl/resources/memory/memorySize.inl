#pragma once

#include "gdl/resources/memory/memorySize.h"

namespace GDL
{

constexpr MemorySize::MemorySize(size_t numBytes)
    : mNumBytes{numBytes}
{
}



constexpr MemorySize& MemorySize::operator=(MemorySize&& other)
{
    mNumBytes = other.mNumBytes;
    return *this;
}



constexpr bool MemorySize::operator==(const MemorySize& rhs) const
{
    return mNumBytes == rhs.mNumBytes;
}



constexpr bool MemorySize::operator!=(const MemorySize& rhs) const
{
    return !(*this == rhs);
}



constexpr bool MemorySize::operator<(size_t rhs) const
{
    return mNumBytes < rhs;
}



constexpr bool operator<(size_t lhs, const MemorySize& rhs)
{
    return rhs > lhs;
}



constexpr bool MemorySize::operator>(size_t rhs) const
{
    return mNumBytes > rhs;
}



constexpr bool operator>(size_t lhs, const MemorySize& rhs)
{
    return rhs < lhs;
}



constexpr bool MemorySize::operator<=(size_t rhs) const
{
    return mNumBytes <= rhs;
}



constexpr bool operator<=(size_t lhs, const MemorySize& rhs)
{
    return rhs >= lhs;
}



constexpr bool MemorySize::operator>=(size_t rhs) const
{
    return mNumBytes >= rhs;
}



constexpr bool operator>=(size_t lhs, const MemorySize& rhs)
{
    return rhs <= lhs;
}



constexpr bool MemorySize::operator<(const MemorySize& rhs) const
{
    return mNumBytes < rhs.mNumBytes;
}



constexpr bool MemorySize::operator>(const MemorySize& rhs) const
{
    return mNumBytes > rhs.mNumBytes;
}


constexpr bool MemorySize::operator<=(const MemorySize& rhs) const
{
    return mNumBytes <= rhs.mNumBytes;
}



constexpr bool MemorySize::operator>=(const MemorySize& rhs) const
{
    return mNumBytes >= rhs.mNumBytes;
}



constexpr MemorySize MemorySize::operator*(size_t rhs) const
{
    return MemorySize(mNumBytes * rhs);
}



constexpr MemorySize operator*(size_t lhs, const MemorySize& rhs)
{
    return rhs * lhs;
}



constexpr bool MemorySize::operator==(size_t rhs) const
{
    return mNumBytes == rhs;
}



constexpr bool operator==(size_t lhs, const MemorySize& rhs)
{
    return rhs == lhs;
}



constexpr bool MemorySize::operator!=(size_t rhs) const
{
    return !(*this == rhs);
}



constexpr bool operator!=(size_t lhs, const MemorySize& rhs)
{
    return rhs != lhs;
}



constexpr size_t MemorySize::GetNumBytes() const
{
    return mNumBytes;
}



constexpr MemorySize& MemorySize::operator=(const MemorySize& other)
{
    mNumBytes = other.mNumBytes;
    return *this;
}



constexpr inline MemorySize operator""_B(unsigned long long lhs)
{
    assert(std::numeric_limits<size_t>::max() >= lhs);
    return MemorySize(static_cast<size_t>(lhs) * Pow<0, size_t>(1000));
}



constexpr inline MemorySize operator""_kB(unsigned long long lhs)
{
    assert(std::numeric_limits<size_t>::max() >= lhs);
    return MemorySize(static_cast<size_t>(lhs) * Pow<1, size_t>(1000));
}



constexpr inline MemorySize operator""_MB(unsigned long long lhs)
{
    assert(std::numeric_limits<size_t>::max() >= lhs);
    return MemorySize(static_cast<size_t>(lhs) * Pow<2, size_t>(1000));
}



constexpr inline MemorySize operator""_GB(unsigned long long lhs)
{
    assert(std::numeric_limits<size_t>::max() >= lhs);
    return MemorySize(static_cast<size_t>(lhs) * Pow<3, size_t>(1000));
}



constexpr inline MemorySize operator""_TB(unsigned long long lhs)
{
    assert(std::numeric_limits<size_t>::max() >= lhs);
    return MemorySize(static_cast<size_t>(lhs) * Pow<4, size_t>(1000));
}



constexpr inline MemorySize operator""_KiB(unsigned long long lhs)
{
    assert(std::numeric_limits<size_t>::max() >= lhs);
    return MemorySize(static_cast<size_t>(lhs) * Pow<1, size_t>(1024));
}



constexpr inline MemorySize operator""_MiB(unsigned long long lhs)
{
    assert(std::numeric_limits<size_t>::max() >= lhs);
    return MemorySize(static_cast<size_t>(lhs) * Pow<2, size_t>(1024));
}



constexpr inline MemorySize operator""_GiB(unsigned long long lhs)
{
    assert(std::numeric_limits<size_t>::max() >= lhs);
    return MemorySize(static_cast<size_t>(lhs) * Pow<3, size_t>(1024));
}



constexpr inline MemorySize operator""_TiB(unsigned long long lhs)
{
    assert(std::numeric_limits<size_t>::max() >= lhs);
    return MemorySize(static_cast<size_t>(lhs) * Pow<4, size_t>(1024));
}



} // namespace GDL
