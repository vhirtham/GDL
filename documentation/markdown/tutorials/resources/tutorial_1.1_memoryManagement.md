# Memory Management

Dynamic memory allocation using the heap (new/delete or malloc/free) is usually quiet expensive. The GDL provides an integrated memory management system to reduce the costs of dynamic memory allocation. It prallocates a huge chunk of memory on the heap and uses it to satisfy the programs memory requirements. 

In this tutorial we will learn how to use this memory system. The good news is, that it is quiet simple, especially if you are familiar with the standard template library (STL). But lets start.

## Initialization

First thing you need to do is to include the header of the memory manager:

~~~ cpp
#include "gdl/resources/memory/memoryManager.h"
~~~

We want to avoid writing `GDL::` everytime we use a GDL class. So we add:

~~~ cpp
using namespace GDL;
~~~

The memory manager is implemented as a singleton. You can not create a local instance. Instead you have to get a reference to the singleton. You can do this by calling the static `Instance` function:

~~~ cpp
MemoryManager& memoryManager = MemoryManager::Instance();
~~~

The memory manager provides different systems to manage dynamic memory allocation, a general purpose memory, a memory pool and two types of memory stacks.
 
The behaviour of the general purpose memory is not different to an allocation on the heap. You can get a chunk of memory of arbitrary size and alignment as long as there is a sufficiently large and free piece of contigous memory left. If a pointer is freed, the memory block is joined with neighboring free memory blocks and can immidiately be reused. This is the most flexible memory system but also the slowest. The speed might decrease significantly over time due to memory fragmentation. It is recommanded to use it only for data which does not change frequently. 

The memory pool is a piece of memory which is devided into smaller equally sized memory blocks. Since each block is of the same size, allocation and deallocation can be simplyfied to a very fast constant time operation. The drawback is that smaller allocations waste memory and memory request that are larger than the block size can't be handled. Its typical use case are containers where all members need to be allocated seperately like lists, maps and sets. Because the necessary block sizes might vary a lot in a single program, you can use multiple memory pools.

A memory stack behaves at first glance like the general purpose memory. But it is a lot faster. The reason for this speed up is, that it does not have to search a sufficiently large free memory block. It only checks if there is enough space left behind the last allocated memory block and reserves the demanded piece of memory. When you free a pointer, there does not happen much either. Only the internal allocation counter is decreased, but the freed memory can not be reused at this point. Only if the counter goes to 0 the whole memory block is reset and all freed memory can be reused. There is also the possibility to use a helper class which stores the state of the stack and restores it during its destruction. We will see how this works later on in this tutorial. It should be mentioned, that this implementation is not a typical implementation of a memory stack. Usually, when you free the last element, its memory is immediately reuseable. The absence of this feature is intentional since we did not want the user to rely on it. This also made the implementation a little bit simpler and faster, but that was not the main intention.

Now that you know, which memory systems are available, we will add a general purpose memory, a memory stack and two memory pools with different block sizes to the memory manager:

~~~ cpp
memoryManager.CreateGeneralPurposeMemory(1_MB);
memoryManager.CreateMemoryStack(1_MB);
memoryManager.CreateMemoryPool(32_B,1000);
memoryManager.CreateMemoryPool(64_B,1000);
~~~

There are two things to mention in the code above. While general purpose memory and memory stack need a single parameter to be created, the memory pools need two parameters. The memory pool needs to know the size of a single memory block and additionally how many of them are needed. The other memory systems only need the total memory size as parameter.

The second noteworthy thing is that you can not pass a simple integer number to specify memory sizes. Instead you have to provide a special memory size class. The construction of this class is hidden by the use of literals like `_B` and `_MB` in combination with a preceding integer. The use of literals is the only way to construct a memory size class. The following literals are defined: `_B`,`_kB`,`_MB`,`_GB`,`_KiB`,`_MiB`,`_GiB`. Please note that the k in `_kB` is lower case in accordance with the metric prefixes. The reason for this kind of implementation is code readability.  A plain number could mean anything while the combination of number and literal is easy to read and understand. This is especially true for the creation of the memory pools, which need two parameters. Without the literals you would not see which parameter is the block size and which the number of blocks.

Before we can start using the created memory systems we still need to make a final function call:

~~~ cpp
memoryManager.Initialize();
~~~

This will allocate the necessary memory for all memory systems and prohibit any further changes to the memory manager. Now the memory is ready to be used. Even though it is not necessary, we recommand to deinitialize the memory before exiting the program:

~~~ cpp
memoryManager.Deinitialize();
~~~

This function checks if any memory system is still in use or if the internal memory is corrupted. If this is the case it will throw an exception. Otherwise it releases the occupied heap memory. Using this function will help you to find bugs related to dynamic memory usage early.

While it is possible to initialize and deinitialize the memory manager multiple times, you can only modify the memory manager before the first initialization. Before we continue, here is the complete program we have created so far:

~~~ cpp
#include "gdl/resources/memory/memoryManager.h"

using namespace GDL;

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    MemoryManager& memoryManager = MemoryManager::Instance();

    memoryManager.CreateGeneralPurposeMemory(1_MB);
    memoryManager.CreateMemoryStack(1_MB);
    memoryManager.CreateMemoryPool(32_B, 1000);
    memoryManager.CreateMemoryPool(64_B, 1000);

    memoryManager.Initialize();

    memoryManager.Deinitialize();

    return 0;
}
~~~


## Memory allocation

### STL

### Manual allocation



- thread private memory stack
- thread safety
- memory usage due to alignment and internal information that needs to be stored




The complete program of this tutorial can be found at the following path:

~~~
applications/tutorials/resources/tutorial_1.1_memoryManagement.cpp
~~~
