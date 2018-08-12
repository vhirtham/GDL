#pragma once

#include "gdl/resources/memory/memoryInterface.h"


namespace GDL
{

//! @brief Memory system which uses new and delete and is used as backup in all custom allocators.
class HeapMemory : public MemoryInterface
{


public:
    HeapMemory() = default;
    HeapMemory(const HeapMemory&) = delete;
    HeapMemory(HeapMemory&&) = delete;
    HeapMemory& operator=(const HeapMemory&) = delete;
    HeapMemory& operator=(HeapMemory&&) = delete;
    ~HeapMemory() = default;

    //! @brief Allocates memory
    //! @param size: Size of the memory that should be allocated
    //! @param alignment: Alignment of the memory that should be allocated
    //! @return Pointer to memory
    virtual void* Allocate(size_t size, size_t alignment = 1) override;


    //! @brief Deallocates memory at the passed address
    //! @param address: Adress that should be freed
    //! @param alignment: Alignment of the memory that should be allocated
    virtual void Deallocate(void* address, size_t alignment = 1) override;
};

}// namespace GDL
