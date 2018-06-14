addTest(threadPool
    resources/cpu/threadPoolQueue.cpp
    )

addTest(Allocator)


addTest(allocators
    resources/memory/memoryManager.cpp
    )
addTest(globalNewCounter)
addTest(traceBuffer)
