#include "gdl/resources/memory/heapMemory.h"


namespace GDL
{

void* HeapMemory::Allocate(size_t size, size_t alignment)
{
    return ::operator new(size, static_cast<std::align_val_t>(alignment));
}



void HeapMemory::Deallocate(void* address, size_t alignment)
{
    ::operator delete(address, static_cast<std::align_val_t>(alignment));
}

} // namespace GDL
