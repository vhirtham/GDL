# Memory Management

Dynamic memory allocation using the heap (new/delete or malloc/free) is usually quiet expensive. The GDL provides an integrated memory management system to reduce the costs of dynamic memory allocation. It preallocates a huge chunk of memory on the heap and uses it to satisfy the programs memory requirements. 

In this tutorial we will learn how to use this memory system. The good news is, that it is quiet simple, especially if you are familiar with the standard template library (STL).

The complete program of this tutorial can be found in the following location:

~~~
applications/tutorials/resources/tutorial_1.1_memoryManagement.cpp
~~~


***


## Table of content

1. [Initialization](#Initialization)
    - [Thread safety](#thread-safety)
    - [Thread private memory stack](#thread-private-memory-stack)
2. [Memory allocation](#memory-allocation)
    - [Container and smart pointer](#container-and-smart-pointer)
    - [Manual allocation with allocators](#manual-allocation-with-allocators)
    - [Manual allocation without allocators](#manual-allocation-without-allocators)
    - [Aligned allocations](#aligned-allocations)
    - [Memory stack deallocator](#memory-stack-deallocator)
    - [Checking the number of heap allocations](#checking-the-number-of-heap-allocations)
3. [Additional information](#additional-information)
    - [Effective memory consumption](#effective-memory-consumption)
    - [Multiple memory pools](#multiple-memory-pools)
    - [Internal memory allocation of GDL classes](#internal-memory-allocation-of-gdl-classes)
    - [Disable GDL allocators](#disable-gdl-allocators)
    - [Reading and writing to invalid addresses](#reading-and-writing-to-invalid-addresses)
    - [Strings](#strings)


***


## Initialization

First thing you need to do is including the header of the memory manager:

~~~ cpp
#include "gdl/resources/memory/memoryManager.h"
~~~

We want to avoid writing `GDL::` everytime we use a GDL class. Therefore we add:

~~~ cpp
using namespace GDL;
~~~

The memory manager is implemented as a singleton. You can not create a local instance. Instead you have to get a reference to the singleton. You can do this by calling the static `Instance` function:

~~~ cpp
MemoryManager& memoryManager = MemoryManager::Instance();
~~~

The memory manager provides different systems to manage dynamic memory allocation, a general purpose memory, a memory pool and two types of memory stacks.
 
The behaviour of the general purpose memory is not different to an allocation on the heap. You can get a chunk of memory of arbitrary size and alignment as long as there is a sufficiently large and free piece of contiguous memory left. If a pointer is freed, the memory block is joined with neighboring free memory blocks and can immediately be reused. This is the most flexible memory system but also the slowest. The speed might decrease significantly over time due to memory fragmentation. It is recommended to use it only for data which does not change frequently. 

The memory pool is a piece of memory which is divided into smaller equally sized memory blocks. Since each block is of the same size, allocation and deallocation can be simplified to a very fast constant time operation. The drawback is that smaller allocations waste memory and memory request that are larger than the block size can't be satisfied. Its typical use case are containers where all members need to be allocated separately like lists, maps and sets. Because the necessary block sizes might vary a lot in a single program, you can create multiple memory pools.

A memory stack behaves at first glance like the general purpose memory. But it is a lot faster. The reason for this speed up is, that it does not have to search a sufficiently large free memory block. It only checks if there is enough space left behind the last allocated memory block and reserves the demanded piece of memory. When you free a pointer, there does not happen much either. Only the internal allocation counter is decreased, but the freed memory can not be reused at this point. Only if the counter decreases to 0 the whole memory block is reset and all freed memory can be reused. There is also the possibility to use a helper class which stores the state of the stack and restores it during its destruction. We will see how this works later on in this tutorial. It should be mentioned, that this implementation is not a typical implementation of a memory stack. Usually, when you free the last element, its memory is immediately reusable. The absence of this feature is intentional since we did not want the user to rely on it. This also made the implementation a little bit simpler and faster, but that was not the main intention.

Now that you know, which memory systems are available, we will add a general purpose memory, a memory stack and two memory pools with different block sizes to the memory manager:

~~~ cpp
memoryManager.CreateGeneralPurposeMemory(1_MB);
memoryManager.CreateMemoryStack(1_MB);
memoryManager.CreateMemoryPool(32_B,1000);
memoryManager.CreateMemoryPool(64_B,1000);
~~~

There are two things to mention in the code above. While general purpose memory and memory stack need a single parameter to be created, the memory pools need two parameters. The memory pool needs to know the size of a single memory block and additionally how many of them are needed. These to parameters determine its total size. The other memory systems only need the total memory size as parameter.

The second noteworthy thing is that you can not pass a simple integer number to specify memory sizes. Instead you have to provide a special memory size class. The construction of this class is hidden by the use of literals like `_B` and `_MB` in combination with a preceding integer. The use of literals is the only way to construct a memory size class. The following literals are defined: `_B`,`_kB`,`_MB`,`_GB`,`_KiB`,`_MiB`,`_GiB`. Please note that the k in `_kB` is lower case in accordance with the metric prefixes. The reason for this kind of implementation is code readability.  A plain number could mean anything while the combination of number and literal is easy to read and understand. This is especially true for the creation of the memory pools, which need two parameters. Without the literals you would not see which parameter is the block size and which the number of blocks.

Before we can start using the created memory systems we still need to make a final function call:

~~~ cpp
memoryManager.Initialize();
~~~

This will allocate the necessary memory for all memory systems and prohibit any further changes to the memory manager. Now the memory is ready to be used. Even though it is not necessary, we recommend to deinitialize the memory before exiting the program:

~~~ cpp
memoryManager.Deinitialize();
~~~

This function checks if any memory system is still in use or if the internal memory is corrupted. If this is the case it will throw an exception. Otherwise it releases the occupied heap memory. Using this function will help you to find bugs related to dynamic memory usage.

While it is possible to initialize and deinitialize the memory manager multiple times, you can only modify the memory manager before the first initialization. 


### Thread safety

If you are using multiple threads in your program you have to be careful with resources that are shared among them. The pure allocation and deallocation of memory is completely safe, since the corresponding operations are protected by a spinlock (with one exception that we will talk about in the next section). You can't corrupt the memory system if multiple threads try to allocate memory at the same time. The only downside is, that just a single thread can allocate or deallocate memory from a certain memory system. All other threads that access the same memory system have to wait until the operation is finished. Note that multiple threads can still access different memory systems at the same time without any performance penalty.
Keep in mind that while the pure allocation and deallocation is thread safe, data that uses the GDL memory systems ,like STL containers for example, is not. You have to take care of that problem yourself.


### Thread private memory stack

At the beginning of this tutorial we mentioned that there are two different kinds of memory stacks. One have already been shown before. The second type is the thread private memory stack. Internally both memory stacks share the same mechanisms. The only difference is that the thread private stack is not protected by a spinlock. It is not meant to be accessed by any other thread than the owning thread. Therefore it does not need to be protected against parallel access. 
To assure that the memory stack is only accessed by the owning thread, a thread can only get a pointer to its own memory stack from the memory manager. The memory manager will never return a pointer to a memory stack that is owned by another thread. Additionally, if the `DEV_EXCEPTION` macro is enabled, the thread private memory stack will throw an exception if it is accessed by any other thread then the one that created it.

Since one can start and stop threads at any time during a programs application, it would be impractical to be forced to create all thread private memory stacks before the memory managers initialization like it is the case with the other memory types we have shown you before. For this reason the thread private memory stack is the only memory type that can and also must be added after the initialization. You also have to delete every thread private memory stack, before you can deinitialize the memory manager. Lets add a thread private stack to our program:

~~~ cpp
...

    memoryManager.EnableThreadPrivateMemory();

    memoryManager.Initialize();
    
    memoryManager.CreatePrivateMemoryStackForThisThread(1_MB);

    memoryManager.DeletePrivateMemoryStackForThisThread();
    
    memoryManager.Deinitialize();

...

~~~

As you can see there are three new lines of code. The first one enables thread private memory stacks. Without this line, you can not add thread private memory stack to the memory manager, even if it is initialized. After initialization we call the function `memoryManager.CreatePrivateMemoryStackForThisThread` to create a memory stack. From the functions name it gets clear that a thread can only create its own memory stack. The same goes for `memoryManager.DeletePrivateMemoryStackForThisThread`. Make sure that each thread that should use a private memory stack creates it after it is started and also deletes it, before the thread is closed.


Before we continue, here is the complete program we have created so far:

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
    memoryManager.EnableThreadPrivateMemory();

    memoryManager.Initialize();

    memoryManager.CreatePrivateMemoryStackForThisThread(1_MB);

    memoryManager.DeletePrivateMemoryStackForThisThread();

    memoryManager.Deinitialize();

    return 0;
}
~~~


***


## Memory allocation

### Container and smart pointer

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

Now you can use it exactly the same as a `std::vector`. 

~~~ cpp
myVec.push_back(1);
myVec.clear();
myVec.shrink_to_fit();
~~~


This is because all those classes are typedefs of the STL version with the `std::allocator` exchanged by a GDL allocator. Depending on the container type, the employed default allocator gets its memory from the memory pool or the general purpose memory. If you want to use a specific memory system, that is not the default one, just add a corresponding suffix to the containers name. There are the suffixes P for the memory Pool, S for the memory stack and TPS for the thread private memory stack. For example, you can create a vector that uses the memory stack as follows:

~~~ cpp
VectorS<U32> myVecS;
~~~

Since using the the memory pool with contiguous memory containers does not make much sense (the block size is usually to small), you won't find a `VectorP`.

It is important to know, that all the allocators are implemented in a way, that they will also work if the corresponding memory system was not added to the memory manager. If you don't have a memory stack, a stack allocator will try to get its memory from the general purpose memory instead. If there is no general purpose memory either, it will get its memory from the heap. This fallback mechanism does not come with a performance penalty, since a reference to the used memory system is stored in a static variable during the first allocation. This mechanism is also the reason why you can't make any changes to the shared memory types after the memory managers initialization and why thread private memory stacks have to be enabled first.

Please keep in mind that all the GDL containers will free their memory during destruction, if you did not do it manually before. If you wan't to deinitialize the memory manager, all memory has to be freed, otherwise you will get an exception. So make sure that all containers have been destroyed or freed their memory, before you call the deinitialize function. As we said before, there is no need to call deinitialize, but it is strongly recommended.

All the things we discussed for containers also apply to smart pointers. However, there is one little difference here. During construction of a `UniquePtr` you provide a pointer that should be freed during the destruction of the smart pointer. The type of the smart pointer specifies from which memory type the pointer should be freed. A `UniquePtrP` will free the memory from the memory pool or its fallback memory systems. This does not prevent you from passing a pointer that was allocated on the memory stack. To avoid problems concerning this issue, we recommend to always use the corresponding `MakeUnique` function instead of allocating the memory manually. Here is an example for memory allocated on the memory pool:

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

An exception for releasing a pointer from the wrong memory system is only thrown if the `DEV_EXCEPTION` macro is enabled. If you used the `Deinitialize` function of the memory manager, you will at least get an exception from the memory stack because there is some memory left that was not freed. So be careful when resetting smart pointers.


### Manual allocation with allocators

Apart from using the provided container types and smart pointers, you can also allocate and free memory manually. This can be done in two ways. The first one is to use the existing allocators that are also used by the containers and which provide a fallback system as described before. To do so you have to include the right header and create the corresponding allocator of the data type you want to allocate:

~~~ cpp
StackAllocator<F32> stackAllocator;
~~~

Afterwards you can use each allocator the same way as the std::allocator:

~~~ cpp
F32* myFloatArray = stackAllocator.allocate(5);
stackAllocator.deallocate(myFloatArray, 5);
~~~

Here we allocated and deallocated memory for 5 32-bit floats on the memory stack. Make sure that you deallocate each allocated memory block before you deinitialize the memory manager. 

### Manual allocation without allocators

The second way to allocate memory manually is by getting a pointer to the desired memory system and ask it for memory.
If you want to allocate memory on the thread private memory stack you have to get the pointer from the memory manager first:

~~~ cpp
ThreadPrivateMemoryStack* tpms = memoryManager.GetThreadPrivateMemoryStack();
if (tpms == nullptr)
    return -1;
~~~

If a certain memory system is not available, the corresponding getter of the memory manager will return a `nullptr`. Make sure that this is not the case before you try to use a memory system. Also note that the getter for the memory pool needs two arguments in contrast to the other memory systems. This is because you can have multiple memory pools and the given parameters help to select the right one (Have a look at the [Multiple memory pools](#multiple-memory-pools) section). 
Afterwards you can allocate and deallocate memory as follows:

~~~ cpp
I32* myIntArray = static_cast<I32*>(tpms->Allocate(sizeof(I32) * 5));
tpms->Deallocate(myIntArray);
~~~

Since all memory systems return `void*` with the allocated memories address, you have to cast it to the right pointer type. In contrast to the allocators, which take only the number of elements that should be allocated, you have to provide the total allocation size in bytes. During deallocation all you need to pass to the function is the pointer to the resource. All memory systems know how much memory need to be freed at the specific address. 

You might wonder, why allocators need the number of elements that were allocated, while the used memory systems already know how much memory needs to be freed at the passed address. The answer is that this is prescribed by allocator interface of the STL containers.


### Aligned allocations

Some data structures need their addresses to be aligned for maximum efficiency. For example SSE registers. If you use the provided containers and allocators, data alignment is done automatically. All you need to do is to add the `alignas` keyword to your data structures. If you use the memory systems directly, the `allocate` function provides a second optional parameter to specify the alignment of the returned address. If we want to align the array from the previous section to 32 byte addresses we just add the alignment value as second parameter:

~~~ cpp
myIntArray = static_cast<I32*>(tpms->Allocate(sizeof(I32) * 5, 32));
~~~

You can choose an arbitrary alignment value that is a power of 2 (including a value of 1) if you use the general purpose memory or one of the two memory stack variants. Just keep in mind that the necessary padding will allocate extra memory. 
The memory pool is a little bit more limited since the fixed block size does not offer much space for extra padding, This gets especially problematic if the alignment is bigger than the block size. To get around this issues you can specify the alignment of the blocks as third optional parameter during the creation of the memory pool:

~~~ cpp
memoryManager.CreateMemoryPool(128_B, 1000, 32);
~~~

Here we created a memory pool with 1000 blocks with a size of 128 bytes that are all aligned to 32 byte addresses. The alignment value that you specify during the creation of the memory pool also limits the maximum alignment request the memory pool can suffice. There is no extra padding added, even if the total allocation size (padding + data size) would fit into the block. With the `DEV_EXCEPTION` macro enabled the `allocate` function will throw an exception if your alignment request is not supported.


### Memory stack deallocator

Earlier we mentioned that there is a helper class to free memory on the memory stack even though the internal counter did not go back to zero. This class is the memory stack deallocator. It is a member class of the memory stack. It stores the current state of the memory stack during construction and restores it when its destructor is called.
To create an instance of this class you have to use the corresponding function of the memory stack. Here is a short example:

~~~ cpp
F32* v1 = stackAllocator.allocate(1);
{
    MemoryStack::MemoryStackDeallocator msd = memoryManager.GetMemoryStack()->CreateMemoryStackDeallocator();
    F32* v2 = stackAllocator.allocate(1);
}
stackAllocator.deallocate(v1, 1);
~~~

In this example we allocate a piece of memory on the memory stack before we open a new scope. Inside of this new scope we create an instance of the memory stack deallocator and allocate another piece of memory. Then we close the scope and free the memory we allocated first. Without the memory stack deallocator the memory stack would throw an exception during deinitialization because we never called `stackAllocator.deallocate(v2, 1)`. But since the memory stack deallocator restores the state from before the second allocation as soon as the extra scope is left, there will be no exception. However, fixing missing deallocations is not the main purpose of this class. The more important feature is, that the memory that was used after its creation is immediately available after its destruction.

However, don't use this class if multiple threads can access the same memory stack (which is never the case for thread private memory stacks). Every memory that is allocated by other threads during the lifetime of the memory stack deallocator will be invalidated after its destruction which will leave your program in an undefined state.

REMARK: In the previous example we used `memoryManager.GetMemoryStack()->CreateMemoryStackDeallocator()` to get an instance of the memory stack deallocator. There we ignored the fact, that `memoryManager.GetMemoryStack()` might return a `nullptr` to keep the example short. In practice you should always check for a `nullptr`.

### Checking the number of heap allocations

The whole purpose of the memory management system is to avoid expensive allocations on the heap. If you want to be sure that there are no hidden heap allocations or if you want to count the number of heap allocations of a specific code segment, you can use the `HeapAllocationCounter`. To do so include the following header:

~~~ cpp
#include "gdl/resources/memory/utility/heapAllocationCounter.h"
~~~

Then simply create an instance of the class at the point where you want to start counting the number of heap allocations:

~~~ cpp
HeapAllocationCounter hac;
~~~

You can print the number of allocations and deallocations at any point by calling:

~~~ cpp
hac.PrintCalls();
~~~

For further information about this class check the doxygen documentation.


***


## Additional information

### Effective memory consumption

The general purpose memory and the memory stacks (might) allocate more memory than requested. This has two reasons. First alignment requests have to be sufficed by adding a certain padding. Additionally, in case of the general purpose memory, the size of the allocation needs to be stored too in order to know how much memory can be freed during deallocation. The additional memory requirements for this information is `sizeof(size_t)`. Keep this in mind if you optimize for minimal sized memory systems.


### Multiple memory pools

If you use multiple memory pools you might wonder which one is returned if you call `GetMemoryPool` of the memory manager or if you use a pool allocator. The selection process works in a way that difference between the block size and the allocation size gets minimal while still fulfilling the alignment requests. 
Lets say you have three memory pools with sizes of 32, 64 and 128 bytes. The first two pools are 16 byte aligned while the one with a block size of 128 bytes is 32 byte aligned. Now you want to allocate 48 bytes with an alignment of 16 bytes. The selection routine will skip the first pool since it does not offer enough space. From the two left choices it will pick the one with the smaller block size (64 bytes) to minimize the wasted memory. Both fulfill the alignment requests since 32 byte alignment is automatically also 16 byte aligned. If we change the alignment of our allocation to 32 bytes, only the pool with a block size of 128 bytes can fulfill our request and hence it is chosen instead.
In case that there is no fitting memory pool available the `GetMemoryPool` function of the memory manager will return a `nullptr`. The pool allocator will use the next available fallback system instead (general purpose memory or heap).


### Internal memory allocation of GDL classes

All GDL classes use the best fitting allocator internally. This is usually the general purpose allocator or the pool allocator. Memory stacks are not used by default since the deallocation mechanism restricts its use cases. However, some classes offer the possibility to exchange their allocator.
Due to the fallback mechanisms of the allocators, you are not forced to create specific memory systems. They keep working even if you do not add any memory system to the memory manager.


### Disable GDL allocators

If you don't want to use the GDL memory system or if you want to compare the speed of your program with and without the memory management system, you can turn it off by disabling the cmake `ENABLE_ALLOCATORS` option. This replaces all GDL allocators with the `std::allocator`


### Reading and writing to invalid addresses

All of the offered memory types do not prevent writing and reading from invalid addresses, since their only job is to provide the memory and not to check how you use it. Writing to an invalid address, for example by accessing a non existing array element with the direct access operator, might irreversibly corrupt the memory system and result in undefined behavior. Again, if you deinitialize the memory manager a corrupted memory system will usually throw an exception which gives you at least a hint that something went wrong with dynamic memory. Turning on the `DEV_EXCEPTION` macro might help to narrow down the location of the corruption.

### Strings

The string header of the GDL also include some function in addition to the pure type definitions. Some of them are meant as replacements for STL functions that perform dynamic memory allocations but do not offer the possibility to provide an alternative allocator. For example the `std::to_string` function can only return a `std::string` which uses the `std::allocator`. Therefore the GDL has a own `ToString` function. Check the doxygen documentation of the string header for further information. 

***



