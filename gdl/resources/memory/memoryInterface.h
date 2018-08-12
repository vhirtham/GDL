#pragma once


#include <memory>

namespace GDL
{

class MemoryInterface
{
public:
    virtual ~MemoryInterface() = default;

    //! @brief Allocates memory
    //! @param size: Size of the memory that should be allocated
    //! @param alignment: Alignment of the memory that should be allocated
    //! @return Pointer to memory
    virtual void* Allocate(size_t size, size_t alignment) = 0;

    //! @brief Deallocates memory at the passed address
    //! @param address: Adress that should be freed
    //! @param alignment: Alignment of the memory that should be allocated
    virtual void Deallocate(void* address, size_t alignment) = 0;
};

} // namespace GDL
