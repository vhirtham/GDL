#pragma once

#include "gdl/resources/memory/generalPurposeMemory.h"

#include <atomic>
#include <memory>
#include <mutex>

namespace GDL
{

class MemoryManager
{
    std::atomic_bool mInitialized;
    mutable std::mutex mMutex;
    mutable bool mSetupFinished;
    mutable bool mMemoryRequestedUninitialized;
    std::unique_ptr<GeneralPurposeMemory> mGeneralPurposeMemory;



    //! @brief Private ctor since this class should only be used as a singleton
    MemoryManager();

public:
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager(MemoryManager&&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;
    MemoryManager& operator=(MemoryManager&&) = delete;
    ~MemoryManager() = default;



    //! @brief Gets the memory manager instance
    //! @return Memory manager Instance
    static MemoryManager& Instance();

    //! @brief Deinitializes the memory manager
    void Deinitialize();

    //! @brief Initializes the memory manager
    void Initialize();

    //! @brief Returns an memory interface pointer to the general purpose memory
    //! @return Pointer to the general purpose memory if it exists. Otherwise nullptr
    MemoryInterface* GetGeneralPurposeMemory() const;


    //! @brief Creates a general purpose memory
    //! @param memorySize: Size of the general purpose memory
    void CreateGeneralPurposeMemory(size_t memorySize);
};
}
