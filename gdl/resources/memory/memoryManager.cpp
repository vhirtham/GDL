#include "gdl/resources/memory/memoryManager.h"

#include "gdl/base/Exception.h"

namespace GDL
{

MemoryManager::MemoryManager()
    : mGeneralPurposeMemory{nullptr}
{
}

void MemoryManager::Initialize()
{
    EXCEPTION(mGeneralPurposeMemory == nullptr, "Can't initialize. No memory added to memory manager.");
    if (mGeneralPurposeMemory != nullptr)
        mGeneralPurposeMemory->Initialize();
}

MemoryManager& MemoryManager::Instance()
{
    static MemoryManager memoryManager;
    return memoryManager;
}

void MemoryManager::Deinitialize()
{
    EXCEPTION(mGeneralPurposeMemory == nullptr, "Can't deinitialize. No memory added to memory manager.");
    if (mGeneralPurposeMemory != nullptr)
        mGeneralPurposeMemory->Deinitialize();
}

GeneralPurposeMemory& GDL::MemoryManager::GetGeneralPurposeMemory()
{
    EXCEPTION(mGeneralPurposeMemory == nullptr, "No general purpose memory added to memory manager.");
    return *mGeneralPurposeMemory;
}

void MemoryManager::CreateGeneralPurposeMemory(size_t memorySize)
{
    DEV_EXCEPTION(mGeneralPurposeMemory != nullptr, "Genaral purpose memory already created");
    mGeneralPurposeMemory.reset(new GeneralPurposeMemory{memorySize});
}
}
