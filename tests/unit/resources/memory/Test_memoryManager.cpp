#include <boost/test/unit_test.hpp>

#include "gdl/base/exception.h"
#include "memoryManagerSetup.h"


using namespace GDL;


BOOST_AUTO_TEST_CASE(General)
{
    MemoryManager& memoryManager = GetMemoryManager();


// Custom memory enabled
#if !(defined(NO_GENERAL_PURPOSE_MEMORY) && defined(NO_MEMORY_POOL) && defined(NO_MEMORY_STACK) &&                     \
      defined(NO_THREAD_PRIVATE_MEMORY_STACK))

// Already created/enabled exceptions
#ifndef NO_GENERAL_PURPOSE_MEMORY
    BOOST_CHECK_THROW(memoryManager.CreateGeneralPurposeMemory(1_MB), Exception);
#endif
#ifndef NO_MEMORY_STACK
    BOOST_CHECK_THROW(memoryManager.CreateMemoryStack(1_MB), Exception);
#endif
#ifndef NO_MEMORY_POOL
    BOOST_CHECK_THROW(memoryManager.CreateMemoryPool(32_B, 100), Exception);
#endif
#ifndef NO_THREAD_PRIVATE_MEMORY_STACK
    BOOST_CHECK_THROW(memoryManager.EnableThreadPrivateMemory(), Exception);
    // Not Initialized
    BOOST_CHECK_THROW(memoryManager.CreatePrivateMemoryStackForThisThread(1_MB), Exception);
#endif



    // Initialize
    BOOST_CHECK_NO_THROW(memoryManager.Initialize());



// Addition and deletion of thread private stacks
#ifndef NO_THREAD_PRIVATE_MEMORY_STACK
    BOOST_CHECK_THROW(memoryManager.DeletePrivateMemoryStackForThisThread(), Exception);
    BOOST_CHECK(memoryManager.GetThreadPrivateMemoryStack() == nullptr);
    BOOST_CHECK_NO_THROW(memoryManager.CreatePrivateMemoryStackForThisThread(1_MB));
#endif

    BOOST_CHECK_THROW(memoryManager.CreatePrivateMemoryStackForThisThread(1_MB), Exception);

#ifndef NO_THREAD_PRIVATE_MEMORY_STACK
    BOOST_CHECK_THROW(memoryManager.Deinitialize(), Exception);
    BOOST_CHECK_NO_THROW(memoryManager.DeletePrivateMemoryStackForThisThread());
#endif

    BOOST_CHECK_THROW(memoryManager.DeletePrivateMemoryStackForThisThread(), Exception);



    // Deinitialize
    BOOST_CHECK_NO_THROW(memoryManager.Deinitialize());



// Only heap allocations
#else

    // No memory added
    BOOST_CHECK_THROW(memoryManager.Initialize(), Exception);
    BOOST_CHECK_THROW(memoryManager.Deinitialize(), Exception);



    // Setup yet not finished
    memoryManager.CreateGeneralPurposeMemory(1_MB);
    memoryManager.CreateMemoryStack(1_MB);
    memoryManager.CreateMemoryPool(32_B, 100);
    memoryManager.EnableThreadPrivateMemory();



    // These checked functions end the setup process and make the memory manager uninitializeable;
    BOOST_CHECK(memoryManager.GetGeneralPurposeMemory() == nullptr);
    BOOST_CHECK(memoryManager.GetMemoryStack() == nullptr);
    BOOST_CHECK(memoryManager.GetMemoryPool(12, 4) == nullptr);
    BOOST_CHECK(memoryManager.GetThreadPrivateMemoryStack() == nullptr);



    // Memory was requested before initialization (fallback to heap allocation)
    BOOST_CHECK_THROW(memoryManager.Initialize(), Exception);
    BOOST_CHECK_THROW(memoryManager.Deinitialize(), Exception);
#endif



    // Exceptions because memory setup is already finished
    BOOST_CHECK_THROW(memoryManager.CreateGeneralPurposeMemory(1_MB), Exception);
    BOOST_CHECK_THROW(memoryManager.CreateMemoryStack(1_MB), Exception);
    BOOST_CHECK_THROW(memoryManager.CreateMemoryPool(32_B, 100), Exception);
    BOOST_CHECK_THROW(memoryManager.EnableThreadPrivateMemory(), Exception);
}
