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
    U8* mFirstFreeMemory;
    U8* mLastFreeMemory;
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

    //! @brief Deinitializes the general purpose memory
    void Deinitialize();

    //! @brief Initializes the general purpose memory
    void Initialize();

private:

    //! @brief Checks if the general purpose memory is constructed with valid parameters. Throws if not.
    void CheckConstructionParameters() const;


    //! @brief Returns if the general purpose memory is initialized
    //! @return TRUE / FALSE
    bool IsInitialized() const;
};

} // namespace GDL
