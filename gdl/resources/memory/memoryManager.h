#pragma once

#include "gdl/resources/memory/generalPurposeMemory.h"


#include <memory>

namespace GDL
{

enum class EMemoryModel
{
    GENERAL_PURPOSE,
    POOL,
    STACK,
    STD
};


class MemoryManager
{
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

    void Deinitialize();

    void Initialize();

    GeneralPurposeMemory& GetGeneralPurposeMemory();

    void CreateGeneralPurposeMemory(size_t memorySize);
};
}
