#pragma once


#include "gdl/GDLTypedefs.h"

#include <memory>
#include <mutex>

namespace GDL
{

class GeneralPurposeMemory
{
    size_t mMemorySize;
    U32 mNumAllocations;
    U8* mFirstFreeMemoryPtr;
    U8* mLastFreeMemoryPtr;
    std::unique_ptr<U8[]> mMemory;
    mutable std::mutex mMutex;

public:
    //! @brief Creates the general purpose memory with <memorySize> bytes of memory
    //! @param memorySize: total amount of memory
    GeneralPurposeMemory(size_t memorySize);

    GeneralPurposeMemory() = delete;
    GeneralPurposeMemory(const GeneralPurposeMemory&) = delete;
    GeneralPurposeMemory(GeneralPurposeMemory&&) = delete;
    GeneralPurposeMemory& operator=(const GeneralPurposeMemory&) = delete;
    GeneralPurposeMemory& operator=(GeneralPurposeMemory&&) = delete;
    ~GeneralPurposeMemory();


    //! @brief Allocates memory
    //! @param size: Size of the memory that should be allocated
    //! @param alignment: Memory alignment
    //! @return Pointer to memory
    void* Allocate(size_t size, size_t alignment = 1);

    //! @brief Counts and returns the number of allocated memory blocks
    //! @return Number of allocated memory blocks
    U32 CountAllocatedMemoryBlocks() const;

    //! @brief Counts and returns the number of free memory blocks
    //! @return Number of free memory blocks
    U32 CountFreeMemoryBlocks() const;

    //! @brief Deallocates memory at the passed address
    //! @param address: Adress that should be freed
    void Deallocate(void* address);

    //! @brief Deinitializes the general purpose memory
    void Deinitialize();

    //! @brief Initializes the general purpose memory
    void Initialize();

private:
    void AddToWrittenSize(void* positionInMemory, const size_t value);

    void* AlignAllocatedMemory(U8* currentMemoryPtr, size_t alignment);

    //! @brief Checks if the general purpose memory is constructed with valid parameters. Throws if not.
    void CheckConstructionParameters() const;


    void FindEnclosingFreeMemoryBlocks(U8*& currentMemoryPtr, U8*& prevFreeMemoryPtr, U8*& nextFreeMemoryPtr) const;

    void FindFreeMemoryBlock(U8*& currentMemoryPtr, U8*& prevFreeMemoryPtr, U8*& nextFreeMemoryPtr,
                             size_t& freeMemorySize, size_t totalAllocationSize) const;

    //! @brief Returns if the general purpose memory is initialized
    //! @return TRUE / FALSE
    bool IsInitialized() const;

    void MergeUpdateLinkedListDeallocation(U8*& currentMemoryPtr, U8*& prevFreeMemoryPtr, U8*& nextFreeMemoryPtr);

    U8* RestoreAllocatedPtr(U8* currentMemoryPtr);

    void UpdateLinkedListAllocation(U8*& currentMemoryPtr, U8*& prevFreeMemoryPtr, U8*& nextFreeMemoryPtr,
                                    size_t freeMemorySize, size_t& totalAllocationSize);

    //! @brief Reads an address at the given position in memory and returns a pointer to that address
    //! @param positionInMemory: Position in memory where the address should be read.
    //! @return Pointer to read address
    inline U8* ReadAddressFromMemory(const U8* positionInMemory) const;


    //! @brief Reads a value of size_t at the given position in memory and returns it
    //! @param positionInMemory: Position in memory where the value should be read.
    //! @return Read value
    inline size_t ReadSizeFromMemory(const void* positionInMemory) const;

    //! @brief Writes a pointer address into the memory at the passed position. This is used to traverse between all
    //! free memory blocks.
    //! @param positionInMemory: Position in memory where the value should be written
    //! @param addressToWrite: Address that should be written to the passed position
    inline void WriteAddressToMemory(U8* positionInMemory, const void* addressToWrite);

    //! @brief Writes a size_t value into the memory at the passed position.
    //! @param positionInMemory: Position in memory where the value should be written
    //! @param value: Value that should be written to the passed position
    inline void WriteSizeToMemory(void* positionInMemory, const size_t value);
};

} // namespace GDL
