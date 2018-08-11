#pragma once

#include "gdl/GDLTypedefs.h"

#include <cstdlib>
#include <iostream>
#include <new>

namespace GDL
{

//! @brief This class counts the number of new and delete calls by overloading the global new and delete operators
//! @remark The necessary new overloads interfere with valgrind. Define DISABLE_GLOBAL_NEW_COUNTER when you want to
//! check a file with valgrind which includes this class.
class GlobalNewCounter
{
#ifndef DISABLE_GLOBAL_NEW_COUNTER
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
#endif // DISABLE_GLOBAL_NEW_COUNTER

    I32 mNewCallsInstance = -1;
    I32 mDeleteCallsInstance = -1;

    static I32 mTotalNewCalls;
    static I32 mTotalDeleteCalls;

public:
    GlobalNewCounter(const GlobalNewCounter&) = default;
    GlobalNewCounter(GlobalNewCounter&&) = default;
    GlobalNewCounter& operator=(const GlobalNewCounter&) = default;
    GlobalNewCounter& operator=(GlobalNewCounter&&) = default;
    ~GlobalNewCounter() = default;

    //! @brief ctor
    GlobalNewCounter()
        : mNewCallsInstance(mTotalNewCalls)
        , mDeleteCallsInstance(mTotalDeleteCalls)
    {
    }


    //! @brief Checks if the new and delete calls since the construction of the class match the passed numbers. Prints
    //! the values if there is a mismatch.
    //! @param expectedNewCalls: Expected number of new calls
    //! @param expectedDeleteCalls: Expected number of delete calls
    //! @return TRUE if the expected numbers match the real count. FALSE otherwise
    //! @remark Returns always TRUE if DISABLE_GLOBAL_NEW_COUNTER is defined
    bool CheckNumCallsExpected(I32 expectedNewCalls, I32 expectedDeleteCalls) const
    {
#ifndef DISABLE_GLOBAL_NEW_COUNTER
        if (!(GetNumNewCalls() == expectedNewCalls && GetNumDeleteCalls() == expectedDeleteCalls))
        {
            std::cout << "new calls (expected / count)   : " << expectedNewCalls << " / " << GetNumNewCalls()
                      << std::endl
                      << "delete calls (expected / count): " << expectedDeleteCalls << " / " << GetNumDeleteCalls()
                      << std::endl;
            return false;
        }
#endif // DISABLE_GLOBAL_NEW_COUNTER
        return true;
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
#ifndef DISABLE_GLOBAL_NEW_COUNTER
        std::cout << "Number of new calls    : " << GetNumNewCalls() << std::endl;
        std::cout << "Number of delete calls : " << GetNumDeleteCalls() << std::endl;
#else
        std::cout << "Global new counter disabled." << std::endl;
#endif // DISABLE_GLOBAL_NEW_COUNTER
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


    //! @brief Helper function which returns the passed value if DISABLE_GLOBAL_NEW_COUNTER is not defined. Otherwise it
    //! returns -1.
    //! @param value: Value to return
    //! @return Passed value or -1
    static inline I32 ReturnValue(I32 value)
    {
#ifndef DISABLE_GLOBAL_NEW_COUNTER
        return value;
#else
        return -1;
#endif // DISABLE_GLOBAL_NEW_COUNTER
    }
};

I32 GlobalNewCounter::mTotalNewCalls = 0;
I32 GlobalNewCounter::mTotalDeleteCalls = 0;
} // namespace GDL


// Overloads of new -------------------------------------------------------------------------------

#ifndef DISABLE_GLOBAL_NEW_COUNTER
void* operator new(std::size_t size)
{
    GDL::GlobalNewCounter::IncreaseTotalNewCalls();
    void* p = malloc(size);
    if (!p)
        throw std::bad_alloc(); // LCOV_EXCL_LINE
    return p;
}

void* operator new[](std::size_t size)
{
    GDL::GlobalNewCounter::IncreaseTotalNewCalls();
    void* p = malloc(size);
    if (!p)
        throw std::bad_alloc(); // LCOV_EXCL_LINE
    return p;
}

void* operator new(std::size_t size, const std::nothrow_t&) noexcept
{
    GDL::GlobalNewCounter::IncreaseTotalNewCalls();
    return malloc(size);
}

void* operator new[](std::size_t size, const std::nothrow_t&) noexcept
{
    GDL::GlobalNewCounter::IncreaseTotalNewCalls();
    return malloc(size);
}

void* operator new(std::size_t size, std::align_val_t al)
{
    GDL::GlobalNewCounter::IncreaseTotalNewCalls();
    void* p = aligned_alloc(static_cast<size_t>(al), size);
    if (!p)
        throw std::bad_alloc(); // LCOV_EXCL_LINE
    return p;
}

void* operator new[](std::size_t size, std::align_val_t al)
{
    GDL::GlobalNewCounter::IncreaseTotalNewCalls();
    void* p = aligned_alloc(static_cast<size_t>(al), size);
    if (!p)
        throw std::bad_alloc(); // LCOV_EXCL_LINE
    return p;
}

void* operator new(std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept
{
    GDL::GlobalNewCounter::IncreaseTotalNewCalls();
    return aligned_alloc(static_cast<size_t>(al), size);
}

void* operator new[](std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept
{
    GDL::GlobalNewCounter::IncreaseTotalNewCalls();
    return aligned_alloc(static_cast<size_t>(al), size);
}

void operator delete(void* ptr) noexcept
{
    GDL::GlobalNewCounter::IncreaseTotalDeleteCalls();
    free(ptr);
}
void operator delete(void* ptr, const std::nothrow_t&)noexcept
{
    GDL::GlobalNewCounter::IncreaseTotalDeleteCalls();
    free(ptr);
}
void operator delete[](void* ptr) noexcept
{
    GDL::GlobalNewCounter::IncreaseTotalDeleteCalls();
    free(ptr);
}
void operator delete[](void* ptr, const std::nothrow_t&) noexcept
{
    GDL::GlobalNewCounter::IncreaseTotalDeleteCalls();
    free(ptr);
}

void operator delete(void* ptr, [[maybe_unused]] std::align_val_t al) noexcept
{
    GDL::GlobalNewCounter::IncreaseTotalDeleteCalls();
    free(ptr);
}

void operator delete[](void* ptr, [[maybe_unused]] std::align_val_t al) noexcept
{
    GDL::GlobalNewCounter::IncreaseTotalDeleteCalls();
    free(ptr);
}

void operator delete(void* ptr, [[maybe_unused]] std::align_val_t al, const std::nothrow_t& tag) noexcept
{
    GDL::GlobalNewCounter::IncreaseTotalDeleteCalls();
    free(ptr);
}

void operator delete[](void* ptr, [[maybe_unused]] std::align_val_t al, const std::nothrow_t& tag) noexcept
{
    GDL::GlobalNewCounter::IncreaseTotalDeleteCalls();
    free(ptr);
}

#endif // DISABLE_GLOBAL_NEW_COUNTER
