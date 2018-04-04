#pragma once


#include <cstdlib>
#include <new>

namespace GDL
{

//! @brief This class counts the number of new and delete calls by overloading the global new and delete operators
class GlobalNewCounter
{
    friend void* ::operator new(std::size_t size);
    friend void* ::operator new(std::size_t size, const std::nothrow_t&) noexcept;
    friend void* ::operator new[](std::size_t size);
    friend void* ::operator new[](std::size_t size, const std::nothrow_t&) noexcept;
    friend void ::operator delete(void* ptr) noexcept;
    friend void ::operator delete(void* ptr, const std::nothrow_t&)noexcept;
    friend void ::operator delete[](void* ptr) noexcept;
    friend void ::operator delete[](void* ptr, const std::nothrow_t&) noexcept;

    int mNewCallsInstance = -1;
    int mDeleteCallsInstance = -1;

    static int mTotalNewCalls;
    static int mTotalDeleteCalls;

public:
    GlobalNewCounter()
        : mNewCallsInstance(mTotalNewCalls)
        , mDeleteCallsInstance(mTotalDeleteCalls)
    {
    }

    int GetNumNewCalls() const
    {
        return mTotalNewCalls - mNewCallsInstance;
    }
    int GetNumDeleteCalls() const
    {
        return mTotalDeleteCalls - mDeleteCallsInstance;
    }

    static int GetTotalNumNewCalls()
    {
        return mTotalNewCalls;
    }
    static int GetTotalNumDeleteCalls()
    {
        return mTotalDeleteCalls;
    }
};

int GlobalNewCounter::mTotalNewCalls = 0;
int GlobalNewCounter::mTotalDeleteCalls = 0;
}


void* operator new(std::size_t size)
{
    ++GDL::GlobalNewCounter::mTotalNewCalls;
    void* p = malloc(size);
    if (!p)
        throw std::bad_alloc(); // LCOV_EXCL_LINE
    return p;
}

void* operator new[](std::size_t size)
{
    ++GDL::GlobalNewCounter::mTotalNewCalls;
    void* p = malloc(size);
    if (!p)
        throw std::bad_alloc(); // LCOV_EXCL_LINE
    return p;
}

void* operator new(std::size_t size, const std::nothrow_t&) noexcept
{
    ++GDL::GlobalNewCounter::mTotalNewCalls;
    return malloc(size);
}

void* operator new[](std::size_t size, const std::nothrow_t&) noexcept
{
    ++GDL::GlobalNewCounter::mTotalNewCalls;
    return malloc(size);
}


void operator delete(void* ptr) noexcept
{
    ++GDL::GlobalNewCounter::mTotalDeleteCalls;
    free(ptr);
}
void operator delete(void* ptr, const std::nothrow_t&)noexcept
{
    ++GDL::GlobalNewCounter::mTotalDeleteCalls;
    free(ptr);
}
void operator delete[](void* ptr) noexcept
{
    ++GDL::GlobalNewCounter::mTotalDeleteCalls;
    free(ptr);
}
void operator delete[](void* ptr, const std::nothrow_t&) noexcept
{
    ++GDL::GlobalNewCounter::mTotalDeleteCalls;
    free(ptr);
}
