addTest(Allocator)


addTest(allocators
    resources/memory/memoryManager.cpp
    )
addTest(globalNewCounter)

addTest(memoryPool
    resources/memory/memoryPool.cpp
    )
addTest(memoryStack
    resources/memory/memoryStack.cpp
    )
