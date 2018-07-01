addTest(Allocator)


addTest(allocators
    resources/memory/memoryManager.cpp
    )
addTest(globalNewCounter)

addTest(memoryPool
    resources/memory/memoryPool.cpp
    )
addTest(threadPrivateMemoryStack
    resources/memory/threadPrivateMemoryStack.cpp
    )
