#pragma once

#include "gdl/base/fundamentalTypes.h"

#include <atomic>
#include <cstdlib>
#include <iostream>
#include <new>


namespace GDL
{

//! @brief This class counts the number of new and delete calls by overloading the global new and delete operators
//! @remark The necessary new overloads interfere with valgrind. Define DISABLE_HEAP_ALLOCATION_COUNTER when you want to
//! check a file with valgrind which includes this class.
class HeapAllocationCounter
{
#ifndef DISABLE_HEAP_ALLOCATION_COUNTER
    friend void* ::operator new(std::size_t size);
    friend void* ::operator new(std::size_t size, std::align_val_t al);
    friend void* ::operator new(std::size_t size, const std::nothrow_t&) noexcept;
    friend void* ::operator new(std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept;
    friend void* ::operator new[](std::size_t size);
    friend void* ::operator new[](std::size_t size, std::align_val_t al);
    friend void* ::operator new[](std::size_t size, const std::nothrow_t&) noexcept;
    friend void* ::operator new[](std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept;
    friend void ::operator delete(void* ptr) noexcept;
    friend void ::operator delete(void* ptr, const std::nothrow_t&)noexcept;
    friend void ::operator delete[](void* ptr) noexcept;
    friend void ::operator delete[](void* ptr, const std::nothrow_t&) noexcept;
    friend void ::operator delete(void* ptr, std::align_val_t al) noexcept;
    friend void ::operator delete[](void* ptr, std::align_val_t al) noexcept;
    friend void ::operator delete(void* ptr, std::align_val_t al, const std::nothrow_t& tag) noexcept;
    friend void ::operator delete[](void* ptr, std::align_val_t al, const std::nothrow_t& tag) noexcept;
#ifdef __cpp_sized_deallocation
    friend void ::operator delete(void* ptr, std::size_t sz) noexcept;
    friend void ::operator delete[](void* ptr, std::size_t sz) noexcept;
    friend void ::operator delete(void* ptr, std::size_t sz, std::align_val_t al) noexcept;
    friend void ::operator delete[](void* ptr, std::size_t sz, std::align_val_t al) noexcept;
#endif // __cpp_sized_deallocation
#endif // DISABLE_HEAP_ALLOCATION_COUNTER

    std::atomic<I32> mNewCallsInstance = -1;
    std::atomic<I32> mDeleteCallsInstance = -1;

    static std::atomic<I32> mTotalNewCalls;
    static std::atomic<I32> mTotalDeleteCalls;

public:
    HeapAllocationCounter(const HeapAllocationCounter&) = default;
    HeapAllocationCounter(HeapAllocationCounter&&) = default;
    HeapAllocationCounter& operator=(const HeapAllocationCounter&) = default;
    HeapAllocationCounter& operator=(HeapAllocationCounter&&) = default;
    ~HeapAllocationCounter() = default;

    //! @brief ctor
    HeapAllocationCounter()
        : mNewCallsInstance(mTotalNewCalls.load())
        , mDeleteCallsInstance(mTotalDeleteCalls.load())
    {
    }


    //! @brief Checks if the new and delete calls since the construction of the class match the passed numbers. Prints
    //! the values if there is a mismatch.
    //! @param expectedNewCalls: Expected number of new calls
    //! @param expectedDeleteCalls: Expected number of delete calls
    //! @return TRUE if the expected numbers match the real count. FALSE otherwise
    //! @remark Returns always TRUE if DISABLE_HEAP_ALLOCATION_COUNTER is defined
    bool CheckNumCallsExpected([[maybe_unused]] I32 expectedNewCalls, [[maybe_unused]] I32 expectedDeleteCalls) const
    {
#ifndef DISABLE_HEAP_ALLOCATION_COUNTER
        if (!(GetNumNewCalls() == expectedNewCalls && GetNumDeleteCalls() == expectedDeleteCalls))
        {
            std::cout << "new calls (expected / count)   : " << expectedNewCalls << " / " << GetNumNewCalls()
                      << std::endl
                      << "delete calls (expected / count): " << expectedDeleteCalls << " / " << GetNumDeleteCalls()
                      << std::endl;
            return false;
        }
#endif // DISABLE_HEAP_ALLOCATION_COUNTER
        return true;
    }


    //! @brief Checks if the new and delete calls since the construction of the class match the passed numbers. Prints
    //! the values if there is a mismatch. If custom allocation is enabled the first two numbers are relevant for the
    //! test, otherwise the last two numbers.
    //! @param expectedNewCalls: Expected number of new calls if custom allocation is enabled
    //! @param expectedDeleteCalls: Expected number of delete calls if custom allocation is enabled
    //! @param expectedNewCallsSTDAlloc: Expected number of new calls if custom allocation is disabled
    //! @param expectedDeleteCallsSTDAlloc: Expected number of delete calls if custom allocation is disabled
    //! @return TRUE if the expected numbers match the real count. FALSE otherwise
    //! @remark Returns always TRUE if DISABLE_HEAP_ALLOCATION_COUNTER is defined. Also always returns TRUE if
    //! USE_STD_ALLOCATOR is defined and expectedNewCallsSTDAlloc and expectedDeleteCallsSTDAlloc are both set to -1.
    bool CheckNumCallsExpectedCustomAllocation([[maybe_unused]] I32 expectedNewCalls,
                                               [[maybe_unused]] I32 expectedDeleteCalls,
                                               [[maybe_unused]] I32 expectedNewCallsSTDAlloc = -1,
                                               [[maybe_unused]] I32 expectedDeleteCallsSTDAlloc = -1) const
    {
#ifdef USE_STD_ALLOCATOR

        if (expectedNewCallsSTDAlloc == -1 && expectedDeleteCallsSTDAlloc == -1)
            return true;
        return CheckNumCallsExpected(expectedNewCallsSTDAlloc, expectedDeleteCallsSTDAlloc);

#else // USE_STD_ALLOCATOR
        return CheckNumCallsExpected(expectedNewCalls, expectedDeleteCalls);
#endif // USE_STD_ALLOCATOR
    }

    //! @brief Gets the number of new calls since the construction of the instance
    //! @return Number of new calls since the construction of the instance
    I32 GetNumNewCalls() const
    {
        return ReturnValue(mTotalNewCalls - mNewCallsInstance);
    }


    //! @brief Gets the number of delete calls since the construction of the instance
    //! @return Number of delete calls since the construction of the instance
    I32 GetNumDeleteCalls() const
    {
        return ReturnValue(mTotalDeleteCalls - mDeleteCallsInstance);
    }


    //! @brief Gets the total number of new calls of the program
    //! @return Total number of new calls of the program
    static I32 GetTotalNumNewCalls()
    {
        return ReturnValue(mTotalNewCalls);
    }


    //! @brief Gets the total number of delete calls of the program
    //! @return Total number of delete calls of the program
    static I32 GetTotalNumDeleteCalls()
    {
        return ReturnValue(mTotalDeleteCalls);
    }


    //! @brief Prints the number of new and delete calls since the construction of the class instance
    void PrintCalls() const
    {
#ifndef DISABLE_HEAP_ALLOCATION_COUNTER
        std::cout << "Number of new calls    : " << GetNumNewCalls() << std::endl;
        std::cout << "Number of delete calls : " << GetNumDeleteCalls() << std::endl;
#else
        std::cout << "Global new counter disabled." << std::endl;
#endif // DISABLE_HEAP_ALLOCATION_COUNTER
    }


private:
    //! @brief Increases the new counter
    static inline void IncreaseTotalNewCalls()
    {
        ++mTotalNewCalls;
    }


    //! @brief Increases the delete counter
    static inline void IncreaseTotalDeleteCalls()
    {
        ++mTotalDeleteCalls;
    }


    //! @brief Helper function which returns the passed value if DISABLE_HEAP_ALLOCATION_COUNTER is not defined.
    //! Otherwise it
    //! returns -1.
    //! @param value: Value to return
    //! @return Passed value or -1
    static inline I32 ReturnValue([[maybe_unused]] I32 value)
    {
#ifndef DISABLE_HEAP_ALLOCATION_COUNTER
        return value;
#else
        return -1;
#endif // DISABLE_HEAP_ALLOCATION_COUNTER
    }
};

std::atomic<I32> HeapAllocationCounter::mTotalNewCalls = 0;
std::atomic<I32> HeapAllocationCounter::mTotalDeleteCalls = 0;
} // namespace GDL


// Overloads of new -------------------------------------------------------------------------------

#ifndef DISABLE_HEAP_ALLOCATION_COUNTER
void* operator new(std::size_t size)
{
    GDL::HeapAllocationCounter::IncreaseTotalNewCalls();
    void* p = malloc(size);
    if (!p)
        throw std::bad_alloc(); // LCOV_EXCL_LINE
    return p;
}

void* operator new[](std::size_t size)
{
    GDL::HeapAllocationCounter::IncreaseTotalNewCalls();
    void* p = malloc(size);
    if (!p)
        throw std::bad_alloc(); // LCOV_EXCL_LINE
    return p;
}

void* operator new(std::size_t size, const std::nothrow_t&) noexcept
{
    GDL::HeapAllocationCounter::IncreaseTotalNewCalls();
    return malloc(size);
}

void* operator new[](std::size_t size, const std::nothrow_t&) noexcept
{
    GDL::HeapAllocationCounter::IncreaseTotalNewCalls();
    return malloc(size);
}

void* operator new(std::size_t size, std::align_val_t al)
{
    GDL::HeapAllocationCounter::IncreaseTotalNewCalls();
    void* p = aligned_alloc(static_cast<size_t>(al), size);
    if (!p)
        throw std::bad_alloc(); // LCOV_EXCL_LINE
    return p;
}

void* operator new[](std::size_t size, std::align_val_t al)
{
    GDL::HeapAllocationCounter::IncreaseTotalNewCalls();
    void* p = aligned_alloc(static_cast<size_t>(al), size);
    if (!p)
        throw std::bad_alloc(); // LCOV_EXCL_LINE
    return p;
}

void* operator new(std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept
{
    GDL::HeapAllocationCounter::IncreaseTotalNewCalls();
    return aligned_alloc(static_cast<size_t>(al), size);
}

void* operator new[](std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept
{
    GDL::HeapAllocationCounter::IncreaseTotalNewCalls();
    return aligned_alloc(static_cast<size_t>(al), size);
}

void operator delete(void* ptr) noexcept
{
    GDL::HeapAllocationCounter::IncreaseTotalDeleteCalls();
    free(ptr);
}

void operator delete[](void* ptr) noexcept
{
    GDL::HeapAllocationCounter::IncreaseTotalDeleteCalls();
    free(ptr);
}

void operator delete(void* ptr, const std::nothrow_t&)noexcept
{
    GDL::HeapAllocationCounter::IncreaseTotalDeleteCalls();
    free(ptr);
}

void operator delete[](void* ptr, const std::nothrow_t&) noexcept
{
    GDL::HeapAllocationCounter::IncreaseTotalDeleteCalls();
    free(ptr);
}


#ifdef __cpp_sized_deallocation
void operator delete(void* ptr, [[maybe_unused]] std::size_t sz) noexcept
{
    GDL::HeapAllocationCounter::IncreaseTotalDeleteCalls();
    free(ptr);
}

void operator delete[](void* ptr, [[maybe_unused]] std::size_t sz) noexcept
{
    GDL::HeapAllocationCounter::IncreaseTotalDeleteCalls();
    free(ptr);
}

void operator delete(void* ptr, [[maybe_unused]] std::size_t sz, [[maybe_unused]] std::align_val_t al) noexcept
{
    GDL::HeapAllocationCounter::IncreaseTotalDeleteCalls();
    free(ptr);
}

void operator delete[](void* ptr, [[maybe_unused]] std::size_t sz, [[maybe_unused]] std::align_val_t al) noexcept
{
    GDL::HeapAllocationCounter::IncreaseTotalDeleteCalls();
    free(ptr);
}
#endif // __cpp_sized_deallocation

void operator delete(void* ptr, [[maybe_unused]] std::align_val_t al) noexcept
{
    GDL::HeapAllocationCounter::IncreaseTotalDeleteCalls();
    free(ptr);
}

void operator delete[](void* ptr, [[maybe_unused]] std::align_val_t al) noexcept
{
    GDL::HeapAllocationCounter::IncreaseTotalDeleteCalls();
    free(ptr);
}

void operator delete(void* ptr, [[maybe_unused]] std::align_val_t al,
                     [[maybe_unused]] const std::nothrow_t& tag) noexcept
{
    GDL::HeapAllocationCounter::IncreaseTotalDeleteCalls();
    free(ptr);
}

void operator delete[](void* ptr, [[maybe_unused]] std::align_val_t al,
                       [[maybe_unused]] const std::nothrow_t& tag) noexcept
{
    GDL::HeapAllocationCounter::IncreaseTotalDeleteCalls();
    free(ptr);
}

#endif // DISABLE_HEAP_ALLOCATION_COUNTER
