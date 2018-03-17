#pragma once

#include "gdl/GDLTypedefs.h"
#include "gdl/base/Exception.h"

#include <atomic>
#include <iostream>
#include <memory>

// http://www.josuttis.com/cppcode/allocator.html
// http://modernescpp.com/index.php/memory-management-with-std-allocator
namespace GDL
{
template <class T>
class MyAlloc
{
public:
    // type definitions
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    // rebind allocator to type U
    template <class U>
    struct rebind
    {
        typedef MyAlloc<U> other;
    };

    // return address of values
    pointer address(reference value) const
    {
        return &value;
    }
    const_pointer address(const_reference value) const
    {
        return &value;
    }

    /* constructors and destructor
     * - nothing to do because the allocator has no state
     */
    MyAlloc() throw()
    {
    }
    MyAlloc(const MyAlloc&) throw()
    {
    }
    template <class U>
    MyAlloc(const MyAlloc<U>&) throw()
    {
    }
    ~MyAlloc() throw()
    {
    }

    // return maximum number of elements that can be allocated
    size_type max_size() const throw()
    {
        return std::numeric_limits<std::size_t>::max() / sizeof(T);
    }

    // allocate but don't initialize num elements of type T
    pointer allocate(size_type num, const void* = 0)
    {
        // print message and allocate memory with global new
        std::cout << "allocate " << num << " element(s)"
                  << " of size " << sizeof(T) << std::endl;
        pointer ret = (pointer)(::operator new(num * sizeof(T)));
        std::cout << " allocated at: " << (void*)ret << std::endl;
        return ret;
    }

    // initialize elements of allocated storage p with value value
    void construct(pointer p, const T& value)
    {
        // initialize memory with placement new
        new ((void*)p) T(value);
    }

    // destroy elements of initialized storage p
    void destroy(pointer p)
    {
        // destroy objects by calling their destructor
        p->~T();
    }

    // deallocate storage p of deleted elements
    void deallocate(pointer p, size_type num)
    {
        // print message and deallocate memory with global delete
        std::cout << "deallocate " << num << " element(s)"
                  << " of size " << sizeof(T) << " at: " << (void*)p << std::endl;
        ::operator delete((void*)p);
    }
};

// return that all specializations of this allocator are interchangeable
template <class T1, class T2>
bool operator==(const MyAlloc<T1>&, const MyAlloc<T2>&) throw()
{
    return true;
}
template <class T1, class T2>
bool operator!=(const MyAlloc<T1>&, const MyAlloc<T2>&) throw()
{
    return false;
}


class MemoryManager
{
    U64 mMemorySize;
    std::unique_ptr<U8[]> mMemory;
    std::atomic<U8*> mCurrentPosition;

    MemoryManager(U64 memorySize);

public:
    MemoryManager() = delete;
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager(MemoryManager&&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;
    MemoryManager& operator=(MemoryManager&&) = delete;
    ~MemoryManager() = default;

    static MemoryManager& Instance(U64 memorySize = 0);

    void* Allocate(U32 size);
};

MemoryManager::MemoryManager(U64 memorySize)
    : mMemorySize(memorySize)
    , mMemory(std::make_unique<U8[]>(mMemorySize))
    , mCurrentPosition(mMemory.get())
{
    if (memorySize < 1)
        throw Exception(__PRETTY_FUNCTION__,
                        "Can't construct memory Manager with a memory size of 0! You need to call "
                        "MemoryManager::Instance(U64 memorySize) with the desired memory size before the first "
                        "allocation takes place!");
    std::cout << "Constructed memory manager with " << memorySize
              << " bytes of memory. Starting address: " << static_cast<void*>(mCurrentPosition) << std::endl;
}

void* MemoryManager::Allocate(U32 size)
{
    if (mCurrentPosition.load() + size > mMemory.get() + mMemorySize)
        throw Exception(__PRETTY_FUNCTION__, "Out of memory!");
    std::cout << "Memory Manager allocates " << size << " bytes at address"
              << static_cast<void*>(mCurrentPosition.load()) << std::endl;
    return mCurrentPosition.fetch_add(size);
}

MemoryManager& MemoryManager::Instance(U64 memorySize)
{
    static MemoryManager memoryManager(memorySize);
    return memoryManager;
}


// http://www.cplusplus.com/reference/memory/allocator_traits/
template <class T>
struct custom_allocator
{
    typedef T value_type;
    custom_allocator() noexcept
    {
    }
    template <class U>
    custom_allocator(const custom_allocator<U>&) noexcept
    {
    }
    T* allocate(std::size_t n)
    {
        MemoryManager& memoryManager = MemoryManager::Instance();
        return static_cast<T*>(memoryManager.Allocate(n * sizeof(T)));
        //        std::cout << "allocate " << n << " element(s)"
        //                  << " of size " << sizeof(T) << std::endl;
        //        auto ret = static_cast<T*>(::operator new(n * sizeof(T)));

        //        std::cout << " allocated at: " << (void*)ret << std::endl;
        //        return ret;
    }
    void deallocate(T* p, std::size_t n)
    {
        // print message and deallocate memory with global delete
        //        std::cout << "deallocate " << n << " element(s)"
        //                  << " of size " << sizeof(T) << " at: " << (void*)p << std::endl;
        //::delete (p);
    }
};

template <class T, class U>
constexpr bool operator==(const custom_allocator<T>&, const custom_allocator<U>&) noexcept
{
    return true;
}

template <class T, class U>
constexpr bool operator!=(const custom_allocator<T>&, const custom_allocator<U>&) noexcept
{
    return false;
}
}
