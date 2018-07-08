addTest(Allocator)


addTest(allocators
    resources/memory/memoryManager.cpp
    )
addTest(globalNewCounter)

addTest(generalPurposeMemory
    resources/memory/generalPurposeMemory.cpp
    )
addTest(memoryPool
    resources/memory/memoryPool.cpp
    )
addTest(memoryStack
    resources/memory/memoryStack.cpp
    )
