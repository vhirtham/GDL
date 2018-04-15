addTest(threadPool
    resources/cpu/threadPool.cpp
    resources/cpu/threadPoolQueue.cpp
    )

addTest(Allocator)


addTest(allocators
    resources/memory/memoryManager.cpp
    )
addTest(globalNewCounter)
