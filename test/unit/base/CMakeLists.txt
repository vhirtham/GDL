add_subdirectory(simd)

addTest(approx)
addTest(freeFunctions)
addTest(tolerance)
addTest(outputFile)

addTest(string
    resources/memory/generalPurposeMemory.cpp
    resources/memory/heapMemory.cpp
    resources/memory/memoryManager.cpp
    resources/memory/memoryPool.cpp
    resources/memory/memoryStack.cpp)

addTest(timer)

