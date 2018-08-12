addTest(heapAllocationCounter)

addTest(generalPurposeMemory
    resources/memory/generalPurposeMemory.cpp)

addTest(memoryPool
    resources/memory/memoryPool.cpp)

addTest(memoryStack
    resources/memory/memoryStack.cpp)

addTest(memorySizeLiterals)

set(MemoryManagerSources
    resources/memory/generalPurposeMemory.cpp
    resources/memory/heapMemory.cpp
    resources/memory/memoryManager.cpp
    resources/memory/memoryPool.cpp
    resources/memory/memoryStack.cpp)

addTest(memoryManager
    ${MemoryManagerSources})

addTest(allocators
    ${MemoryManagerSources})

addTest(allocators-no_memory_pool_no_memory_stack
    ${MemoryManagerSources}
    -DNO_MEMORY_POOL
    -DNO_MEMORY_STACK)

addTest(allocators-no_general_purpose_memory
    ${MemoryManagerSources}
    -DNO_GENERAL_PURPOSE_MEMORY)

addTest(allocators-only_heap_allocation
    ${MemoryManagerSources}
    -DNO_GENERAL_PURPOSE_MEMORY
    -DNO_MEMORY_POOL
    -DNO_MEMORY_STACK)
