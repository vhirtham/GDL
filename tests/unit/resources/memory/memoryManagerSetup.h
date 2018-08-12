#include "gdl/resources/memory/memoryManager.h"
#include "gdl/resources/memory/memorySize.h"

using namespace GDL;


//! @brief Adds memory systems to the memory manager
//! @return Memory manager
MemoryManager& SetupMemoryManager()
{
    MemoryManager& memoryManager = MemoryManager::Instance();
#ifndef NO_GENERAL_PURPOSE_MEMORY
    memoryManager.CreateGeneralPurposeMemory(1_MiB);
#endif
#ifndef NO_MEMORY_POOL
    memoryManager.CreateMemoryPool((32_B).GetNumBytes(), 1000);
    memoryManager.CreateMemoryPool((64_B).GetNumBytes(), 1000);
    memoryManager.CreateMemoryPool((128_B).GetNumBytes(), 1000);
#endif
#ifndef NO_MEMORY_STACK
    memoryManager.CreateMemoryStack((1_MiB).GetNumBytes());
#endif
    return memoryManager;
}


//! @brief Helper function which gets the memory menager and ensures it is only set up once
//! @return Memory manager
MemoryManager& GetMemoryManager()
{
    static MemoryManager& memoryManager = SetupMemoryManager();
    return memoryManager;
}


void InitializeMemoryManager()
{
#if !(defined(NO_GENERAL_PURPOSE_MEMORY) && defined(NO_MEMORY_POOL) && defined(NO_MEMORY_STACK))
    GetMemoryManager().Initialize();
#endif
}

void DeinitializeMemoryManager()
{
#if !(defined(NO_GENERAL_PURPOSE_MEMORY) && defined(NO_MEMORY_POOL) && defined(NO_MEMORY_STACK))
    GetMemoryManager().Deinitialize();
#endif
}
