#pragma once


#include "gdl/base/fundamentalTypes.h"
#include "gdl/resources/cpu/spinlock.h"
#include "gdl/resources/memory/memoryInterface.h"
#include "gdl/resources/memory/memorySize.h"

#include <memory>


namespace GDL
{

//! @brief Class which manages a piece of memory. It is capable of returning meory blocks of arbitrary size and
//! alignment. For implementation details visit: https://github.com/vhirtham/GDL/issues/4
class GeneralPurposeMemory : public MemoryInterface
{
    //! @brief Stores all data which is needed during allocation
    struct AllocationData
    {
        U8* currentMemoryPtr;
        U8* prevFreeMemoryPtr;
        U8* nextFreeMemoryPtr;
        size_t freeMemorySize;
        size_t totalAllocationSize;

        //! @brief ctor
        //! @param gpm: General purpose memory reference
        //! @param allocationSize: Allocation size
        //! @param alignment: Allocation alignment
        AllocationData(const GeneralPurposeMemory& gpm, size_t allocationSize, size_t alignment);
    };

    //! @brief Stores all data which is needed during deallocation
    struct DeallocationData
    {
        U8* currentMemoryPtr;
        U8* prevFreeMemoryPtr;
        U8* nextFreeMemoryPtr;

        //! @brief ctor
        //! @param gpm: General purpose memory reference
        DeallocationData(const GeneralPurposeMemory& gpm, void* address);
    };

    MemorySize mMemorySize;
    U8* mFirstFreeMemoryPtr;
    std::unique_ptr<U8[]> mMemory;
    mutable SpinLock mSpinLock;

public:
    //! @brief Creates the general purpose memory with <memorySize> bytes of memory
    //! @param memorySize: total amount of memory
    GeneralPurposeMemory(MemorySize memorySize);

    GeneralPurposeMemory() = delete;
    GeneralPurposeMemory(const GeneralPurposeMemory&) = delete;
    GeneralPurposeMemory(GeneralPurposeMemory&&) = delete;
    GeneralPurposeMemory& operator=(const GeneralPurposeMemory&) = delete;
    GeneralPurposeMemory& operator=(GeneralPurposeMemory&&) = delete;
    ~GeneralPurposeMemory() override;


    //! @brief Allocates memory
    //! @param size: Size of the memory that should be allocated
    //! @param alignment: Memory alignment
    //! @return Pointer to memory
    virtual void* Allocate(size_t size, size_t alignment = 1) override;

    //! @brief Counts and returns the number of allocated memory blocks
    //! @return Number of allocated memory blocks
    U32 CountAllocatedMemoryBlocks() const;

    //! @brief Counts and returns the number of free memory blocks
    //! @return Number of free memory blocks
    U32 CountFreeMemoryBlocks() const;

    //! @brief Deallocates memory at the passed address
    //! @param alignment: Memory alignment
    //! @param address: Adress that should be freed
    virtual void Deallocate(void* address, size_t alignment = 1) override;

    //! @brief Deinitializes the general purpose memory
    void Deinitialize();

    //! @brief Initializes the general purpose memory
    void Initialize();

private:
    //! @brief Increases a size_t value written in memory at the passed position.
    //! @param positionInMemory: Position in memory where the value is located
    //! @param value: Value that should be added
    void AddToWrittenSize(void* positionInMemory, const size_t value);

    //! @brief Aligns the passed address, writes the correction value to the preceding byte and returns the aligned
    //! address
    //! @param currentMemoryPtr: Address that should be aligned
    //! @param alignment: Alignment value
    //! @return Aligned address
    void* AlignAllocatedMemory(U8* currentMemoryPtr, size_t alignment);

    //! @brief Checks if the general purpose memory is constructed with valid parameters. Throws if not.
    void CheckConstructionParameters() const;

    //! Counts the total number of memory blocks (allocated and free ones)
    //! @return Number of memory blocks
    U32 CountMemoryBlocks() const;

    //! @brief Counts and returns the number of allocated memory blocks
    //! @return Number of allocated memory blocks
    U32 CountAllocatedMemoryBlocksPrivate() const;

    //! Counts the number of free memory blocks
    //! @return Number of free memory blocks
    U32 CountFreeMemoryBlocksPrivate() const;

    //! @brief Finds the addresses of the free memory blocks which enclose the address that should be freed. The passed
    //! data is modified to store the results.
    //! @param data: Deallocation data which will be modified by the function
    void FindEnclosingFreeMemoryBlocks(DeallocationData& data) const;

    //! @brief Finds a free memory block for the current allocation. The passed data is modified to store the results.
    //! @param data: Allocation data which will be modified by the function
    void FindFreeMemoryBlock(AllocationData& data) const;

    //! @brief Returns a pointer to the end of the managed memory
    //! @return Pointer to the end of the managed memory
    //! @remark The function is marked as const, but the returned pointer can still be modified afterwards. This is
    //! intentional, since this function is only meant meant to improve readability.
    inline U8* GetEndOfMemory() const;

    //! @brief Returns a pointer to the start of the managed memory
    //! @return Pointer to the start of the managed memory
    //! @remark The function is marked as const, but the returned pointer can still be modified afterwards. This is
    //! intentional, since this function is only meant to improve readability.
    inline U8* GetStartOfMemory() const;

    //! @brief Returns if the general purpose memory is initialized
    //! @return TRUE / FALSE
    bool IsInitialized() const;

    //! @brief Returns if the passed address is inside the boundaries of the menaged memory
    //! @param: address: Address that should be checked
    //! @return TRUE / FALSE
    inline bool IsAddressInsideMemory(U8* address) const;

    //! @brief Checks if the passed address points to a valid free memory block
    //! @param: address: Address that should be checked
    //! @return TRUE / FALSE
    bool IsAddressStartOfFreeMemoryBlock(U8* address) const;

    //! @brief Checks if the passed address points to a valid memory block
    //! @param: address: Address that should be checked
    //! @return TRUE / FALSE
    bool IsAddressStartOfMemoryBlock(U8* address) const;

    //! @brief Checks if the deallocated address is a valid allocated memory block.
    //! @return True / false
    //! @remark There is a rare case where the function might return true, even if the address passed to the
    //! deallocation function was never returned by the allocation function. This is the case if the read byte in front
    //! of the address is a valid alignment value and will correct the pointer to a valid allocated memory block.
    bool IsDeallocatedAddressValid(U8* address) const;

    //! @brief Updates the internal linked list during deallocation
    //! @param data: Deallocation data
    void MergeUpdateLinkedListDeallocation(DeallocationData& data);

    //! Reads the alignment byte and restores the start of the allocated memory block
    //! @param currentMemoryPtr: Pointer which was returned during allocation
    //! @return Pointer to the start of the allocated memory block
    U8* RestoreAllocatedPtr(U8* currentMemoryPtr) const;

    //! @brief Updates the internal linked list during allocation
    //! @param data: Allocation data
    void UpdateLinkedListAllocation(AllocationData& data);

    //! @brief Returns the address of the next free memory block
    //! @param currentFreeBlockPtr: Pointer to a free memory block
    //! @return Address of the next free memory block
    inline U8* ReadLinkToNextFreeBlock(U8* currentFreeBlockPtr) const;

    //! @brief Writes the address of the next free memory block to memory
    //! @param currentFreeBlockPtr: Pointer to a free memory block
    //! @param nextFreeBlockPtr: Address of the next free memory block
    inline void WriteLinkToNextFreeBlock(U8* currentFreeBlockPtr, const void* nextFreeBlockPtr);
};

} // namespace GDL
