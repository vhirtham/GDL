#pragma once

#include "gdl/base/functions/pow.h"

#include <cassert>
#include <limits>
#include <memory>

namespace GDL
{

class MemorySize
{
    // Memory size literals
    friend constexpr inline MemorySize operator""_B(unsigned long long lhs);
    friend constexpr inline MemorySize operator""_kB(unsigned long long lhs);
    friend constexpr inline MemorySize operator""_MB(unsigned long long lhs);
    friend constexpr inline MemorySize operator""_GB(unsigned long long lhs);
    friend constexpr inline MemorySize operator""_TB(unsigned long long lhs);
    friend constexpr inline MemorySize operator""_KiB(unsigned long long lhs);
    friend constexpr inline MemorySize operator""_MiB(unsigned long long lhs);
    friend constexpr inline MemorySize operator""_GiB(unsigned long long lhs);
    friend constexpr inline MemorySize operator""_TiB(unsigned long long lhs);


    size_t mNumBytes;

    //! @brief Private ctor which can only be called by the literal operators and this class
    //! @param numBytes: Size in bytes
    constexpr MemorySize(size_t numBytes);


public:
    constexpr MemorySize(const MemorySize&) = default;
    constexpr MemorySize(MemorySize&&) = default;
    ~MemorySize() = default;

    //! @brief Copy assignment operator
    //! @param other: Rhs instance
    //! @return Reference to this instance
    constexpr MemorySize& operator=(const MemorySize& other);

    //! @brief Move assignment operator
    //! @param other: Rhs instance
    //! @return Reference to this instance
    constexpr MemorySize& operator=(MemorySize&& other);

    //! @brief Comparison operator for two instances of the class
    //! @param rhs: Rhs instance
    //! @return TRUE / FALSE
    constexpr bool operator==(const MemorySize& rhs) const;

    //! @brief Comparison operator for two instances of the class
    //! @param rhs: Rhs instance
    //! @return TRUE / FALSE
    constexpr bool operator!=(const MemorySize& rhs) const;

    //! @brief Comparison operator for comparison with a size_t
    //! @param rhs: Rhs
    //! @return TRUE / FALSE
    constexpr bool operator==(size_t rhs) const;

    //! @brief Comparison operator for comparison with a size_t
    //! @param rhs: Rhs
    //! @return TRUE / FALSE
    constexpr bool operator!=(size_t rhs) const;

    //! @brief Multiplication operator
    //! @param rhs: Rhs
    //! @return Result of multiplication
    constexpr MemorySize operator*(size_t rhs) const;

    //! @brief Multiplication operator
    //! @param lhs: Lhs
    //! @param rhs: Rhs
    //! @return Result of multiplication
    friend constexpr MemorySize operator*(size_t lhs, const MemorySize& rhs);

    //! @brief Returns the number of bytes
    //! @return Number of bytes
    constexpr inline size_t GetNumBytes() const;
};

} // namespace GDL

#include "gdl/resources/memory/memorySize.inl"
