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

This function checks if any memory system is still in use or if the internal memory is corrupted. If this is the case it will throw an exception. Otherwise it releases the occupied heap memory. Using this function will help you to find bugs related to dynamic memory usage.

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

### Container and smart pointers

If you are already using STL containers, there is not much you need to learn. You will find a GDL version of most of the STL containers in the directory

~~~
gdl/base/container
~~~

The headers of smart pointers and the string class are located at

~~~
gdl/base
~~~

To use a vector, we have to include its header:

~~~ cpp
#include "gdl/base/container/vector.h"
~~~

Then we add:

~~~ cpp
Vector<U32> myVec;
~~~

Now you can use it excactly the same as a `std::vector`. 

~~~ cpp
myVec.push_back(1);
myVec.clear();
myVec.shrink_to_fit();
~~~


This is because all those classes are typedefs of the STL version with the `std::allocator` exchanged by a GDL allocator. Depending on the container type, the employed default allocator gets its memory from the memory pool or the general purpose memory. If you want to use a specific memory system, that is not the default one, just add a corresponding suffix to the containers name. There are the suffixes P for the memory Pool, S for the memory stack and TPS for the thread private memory stack. You can create a vector that uses the memory stack as follows:

~~~ cpp
VectorS<U32> myVecS;
~~~

Since using the the memory pool with contigous memory containers does not make much sense (the block size is usually to small), you won't find a `VectorP`.

It is important to know, that all the allocators are implemented in a way, that they will also work if the corresponding memory system was not added to the memory manager. If you don't have a memory stack, a stack allocator will try to get its memory from the general purpose memory instead. If there is no general purpose memory either, it will get its memory from the heap. This fallback mechanism does not come with a performance penalty, since a reference to the used memory system is stored in a static variable during the first allocation.

Please keep in mind that all the GDL containers will free their memory during destruction, if you did not do it manually before. If you wan't to deinitialize the memory manager, all memory has to be freed, otherwise you will get an exception. So make sure that all containers have been destroyed or freed their memory, before you call the deinitialize function. As we said before, there is no need to call deinitialize, but it is strongly recommanded.

All the things we discussed for containers also apply to smart pointers. However, there is one little difference here. During construction of a `UniquePtr` you provide a pointer that should be freed during the destruction of the smart pointer. The type of the smart pointer specifies where the memory should be freed. A `UniquePtrP` will free the memory from the memory pool or its fallback memory systems. This does not prevent you from passing a pointer that was allocated on the memory stack. To avoid problems concerning this issue, we recommand to always use the corresponding `MakeUnique` function. Here is an example for memory allocated on the memory pool:

~~~ cpp
UniquePtrP<U32> myPtr = MakeUniqueP<U32>(5);
myPtr.reset(MakeUniqueP<U32>(77).release());
myPtr.reset(nullptr);
~~~

The `MakeUnique` functions allocate the memory from the corresponding memory system and construct the managed object. You can not construct a `UniquePtrP` with  any other `MakeUnique` function than `MakeUniqueP`. However, if you reset the pointer like in the second line above, you can still mess things up. Something like this will compile:

~~~ cpp
UniquePtrP<U32> myPtr = MakeUniqueP<U32>(5);
myPtr.reset(MakeUniqueS<U32>(2).release());
myPtr.reset(nullptr);
~~~

An exception for releasing a pointer from the wrong memory system is only thrown in debug mode. If you used the `Deinitialize` function of the memory manager, you will at least get an exception from the memory stack because there is some memory left that was not freed. So be careful when resetting smart pointers.


### Manual allocation



- thread private memory stack
- thread safety
- memory usage due to alignment and internal information that needs to be stored
- typedef to use std::allocator
- heap allocation counter



The complete program of this tutorial can be found at the following path:

~~~
applications/tutorials/resources/tutorial_1.1_memoryManagement.cpp
~~~
