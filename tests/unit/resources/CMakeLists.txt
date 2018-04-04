addTest(ThreadPool
    resources/cpu/Thread.cpp
    resources/cpu/ThreadPool.cpp
    resources/cpu/ThreadSafeQueue.cpp)

addTest(Allocator)


addTest(allocators
    resources/memory/memoryManager.cpp
    )
addTest(globalNewCounter)
