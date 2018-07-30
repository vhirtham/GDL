#pragma once


#include <memory>

namespace GDL
{

class MemoryInterface
{
public:
    virtual ~MemoryInterface() = default;

    virtual void* Allocate( size_t size, size_t alignment) = 0;
    virtual void Deallocate(void* address) = 0;
};

} // namespace GDL
