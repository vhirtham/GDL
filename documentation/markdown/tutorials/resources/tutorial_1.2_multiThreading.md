# Multi Threading

The GDL offers two classes that will help you running multiple threads in parallel. However, multi threading is a complex topic and if you are not familiar with using `std::thread`s you should probably learn about using them first before you continue with this tutorial.

The complete program of this tutorial can be found in the following location:

~~~
applications/tutorials/resources/tutorial_1.2_multiThreading.cpp
~~~

## Table of content

- [**The Thread class**](#the-thread-class)
- [**Thread pool**](#thread-pool)
    - [Submitting tasks](#submitting-tasks)
    - [Stopping threads and deinitialization](#stopping-threads-and-deinitialization)
    - [Tread pool threads](#tread-pool-threads)
    - [Exception handling](#exception-handling)
    - [Multiple queues](#multiple-queues)
- [**Things you should keep in mind**](#things-you-should-keep-in-mind)



## The Thread class

The thread class is not much more than a small wrapper for `std::thread`. All it does is that it automatically tries to join the managed thread when the destructor is called. You can pass an arbitrary function to its constructor. We will create a small "Hello World" example using the `GDL::Thread`. First add the following lines 

~~~ cpp
#include "gdl/resources/cpu/thread.h"
#include <iostream>

using namespace GDL;
~~~

Afterwards we define our function that should be processed by the thread:

~~~ cpp
void HelloFromThread()
{
    std::cout << "Hello from thread with ID " << std::this_thread::get_id() << "!" << std::endl;
}
~~~

Now we are ready to start our thread:

~~~ cpp
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    Thread thread(HelloFromThread);
}
~~~

That's it. Except for the varying thread ID the output of this program should look like this:

~~~ Shell
Hello from thread with ID 140314797389568!
~~~

All this program does so far is starting a thread which prints a hello world message and closing it afterwards. Keep in mind that the destructor of the Thread class tries to join the thread. So if you are using a loop that waits for an exit condition to be fulfilled, you will create a deadlock if you don't give the exit signal before the destructor is called.

## Thread pool

Starting a thread is a rather expensive operation. It is not a good idea to start a thread for every new task that should be processed. A better solution is to create a certain number of threads at the beginning of your program and feed them continuously with new tasks. A thread pool is a class that does exactly that. It manages an arbitrary number of threads and has one or more internal queues that store the tasks that should be processed. Add the following header to the program:

~~~ cpp
#include "gdl/resources/cpu/threadPool.h"
~~~

Now we can create a thread pool:

~~~ cpp
ThreadPool<> pool;
~~~

As you can see the thread pool is a template class. The template parameter is a 32 bit integer value and determines how many queues it should have. Since we did not provide any number, it will just have a single queue by default.
The default constructor of the thread pool does not start any threads. To start one or more threads we use:

~~~ cpp
pool.StartThreads(2);
~~~

This will start two threads that can now be fed with some tasks.

Alternatively to creating a thread pool and starting threads afterwards, you can also use another constructor that takes an integer number as parameter:

~~~ cpp
ThreadPool<> pool(2);
~~~

This will start the desired number of threads during construction and safes you an additional line of code.

### Submitting tasks

As a first example how to submit tasks we will enqueue our `HelloFromThread` function two times:

~~~ cpp
for (U32 i = 0; i < 2; ++i)
    pool.Submit(HelloFromThread);

std::this_thread::sleep_for(std::chrono::milliseconds{10});
~~~

`Submit` passes a function that should be executed by one of the threads to the thread pools queue. Notice that there are no parenthesis after `HelloFromThread` since we don't want to execute it and pass its return value. We want to pass the function itself.

The sleep function prevents the main thread to continue for 10 milliseconds. Without it, the main thread might call the destructor of the thread pool while the threads are still starting. This is not a problem in general, because the destructor will wait for the threads to join, but in this case the started threads will never ask the queue for any tasks if they get the shutdown signal directly after the startup. Hence, you won't get any output.
This is a quiet bad and error prone way of synchronization that you should avoid in your programs, but synchronization is an own and complicated topic and we want to focus here just on the features of the thread pool. Therefore it is sufficient enough.

The functions or callable objects that you pass to the queue can also have parameters. You have to provide values that should be used during the function call as additional parameters of the `Submit` function. Let us create another function that has a input parameter:

~~~ cpp
void PrintValue(I32 value)
{
    std::cout << "value: " << value << std::endl;
}
~~~

Now we can do the following:

~~~ cpp
pool.Submit(PrintValue, 42);
~~~

Simple as that. However, be careful if one of your parameters is a reference:

~~~ cpp
void SetValueTo1337(I32& value)
{
    value = 1337;
}
~~~

If you submit the previously shown function like this:

~~~ cpp
I32 referencedValue = 0;
pool.Submit(SetValueTo1337, referencedValue);
std::this_thread::sleep_for(std::chrono::milliseconds{10});
std::cout << "Referenced value: " << referencedValue << std::endl;
~~~

the value of the passed variable won't be changed. This is a result of the way how parameters are forwarded. To fix this, wrap all referenced objects into `std::ref`:

~~~ cpp
pool.Submit(SetValueTo1337, std::ref(referencedValue));
~~~

Pointers can be used normally.

There is one restriction to the functions that you can pass to the queue: It is not allowed to have a return value since it can never be used. If a thread finishes a task, it immediately tries to get the next one. It is not supposed to do something with the result of the task. You will get a static assertion from your compiler if you try to pass a function with a return value to `Submit`. So you have to use references/pointers to objects or global variables / singletons to store the results.
Another approach that you could choose is to pass the results to another function and enqueue it again.
In case you want to pass  a function that has a return value and you simply don't want to use it, wrap the function call into a lambda function that does not return a value.

### Stopping threads and deinitialization

Once you have started one or more threads, you can stop them whenever you like. To do so use:

~~~ cpp
pool.CloseThreads(1);
~~~ 

Here we close one of the two started threads. It finishes its current task, is joined and will be removed afterwards. You can specify any number of threads that should be closed.
At this point it does not really matter which one of the two threads is closed, but as you will see later it might be important to know. The threads that were started last are also closed first (last in, first out).

If you want to know how many threads are currently running you can use:

~~~ cpp
pool.GetNumThreads()
~~~


You can also close all running threads by calling:

~~~ cpp
pool.CloseAllThreads();
~~~

Even though it is not necessary and there is no initialize function for the thread pool we recommend using

~~~ cpp
pool.Deinitialize();
~~~ 

before you destroy the thread pool or exit your program. This will shut down all running threads and check if there has been any unhandled exception inside of one of the threads. We will talk in detail about exception handling in a later sections.

### Tread pool threads

Each started thread of a thread pool runs a while loop internally until it gets the signal to shut down. All it does inside that loop is calling the same function over and over again. This is by default the `TryExecuteTask()` function of the thread pool. You can exchange this function by any other function or functional that has no parameters and no return type. Therefore you have to pass the desired function as additional parameter to the `StartThreads` function:

~~~ cpp
U32 counter = 0;
auto threadMainFunc = [&]() { ++counter; };

pool.StartThreads(1, threadMainFunc);
std::this_thread::sleep_for(std::chrono::milliseconds{10});

pool.CloseAllThreads();
std::cout << "Thread main loop iterations: " << counter << std::endl;
~~~

Here we pass a lambda function that just counts the number of function calls. Note that this thread does not process any tasks of the thread pools queue. You have to explicitly tell it to do so by adding `pool.TryExecuteTask()` to the passed lambda function.

You should also know that there is no internal load balancing mechanism. Each thread will repeatedly execute its main loop function as fast as possible. In case of the default function this means that each thread will contiguously try to fetch tasks, even if there are none in the queue (busy waiting). This can be a huge waste of energy if your program is idle, but with the opportunity of passing custom main loop functions you can easily change this behavior by using common synchronization mechanisms like a `std::condition_variable`.

In addition to the main loop function you can also pass a initialization and deinitialization function. These are called before and after the execution of the threads main loop. The default versions don't do anything. If you want to specify them, you have to add them as third and fourth parameter to the `StartThreads` function:

~~~ cpp
auto threadInitFunc = [&]() { std::cout << "Start thread" << std::endl; };
auto threadDeinitFunc = [&]() { std::cout << "Stop thread" << std::endl; };
pool.StartThreads(1, threadMainFunc, threadInitFunc, threadDeinitFunc);
pool.CloseAllThreads();
~~~


Note that there is no way to pass just a initialization or deinitialization function to the thread. If you want to use them, you have to provide all three functions.

REMARK: If you use the GDL memory system, the appropriate way to add thread private memory stacks to a thread pool thread is to create and delete them inside the initialization and deinitialization function of a thread.

### Exception handling

If one of the three customizable functions of a thread pool thread throws an exception it will be caught and the thread will shut down. The shutdown procedure includes calling the deinitialize function (unless it was the throwing function) which might throw another exception that would be caught too. For each caught exception, the thread will add an entry to the thread pools exception log.

A thread that was shut down because of an exception is not automatically removed from the thread pool. So you won't notice that a thread has stopped running by checking the number of threads of a thread pool. There is also no function to check how many threads are still running since there are only two ways for a thread to get to this point: You called `CloseThreads` or `CloseAllThreads` in which case the thread is also removed by the calling thread or the thread caught an unhandled exception. Use the following function to check for the latter case:

~~~ cpp
pool.GetExceptionLogSize();
~~~

Any number greater than 0 tells you that one or more threads have thrown an exception and stopped running as a consequence. A not handled exception that causes a thread to shut down is probably a serious problem that leaves your program in an unstable state and you should not continue its execution. The function

~~~ cpp
pool.PropagateExceptions();
~~~

will throw an exception if the exception log is not empty. It contains the messages of all caught exceptions. You should call this function regularly inside your main thread to notice errors inside your program as soon as possible.

### Multiple queues

So far we have only dealt with a single queue but sometimes it comes in handy to have more than just one. For example if you have a type of tasks that should always be processed by the same thread (OpenGL API) or if tasks are not that important so that they can wait until more urgent tasks are finished. To create a thread pool with multiple queues, just specify the desired number of queues as template parameter:

~~~ cpp
ThreadPool<2> pool2Queues;
~~~

If you have multiple queues you have to specify the queue that should be accessed when using the functions `HasTasks`, `GetNumTasks`, `TryExecuteTask` or `Submit`.  If you don't do this, a static assertion will fail during the programs compilation. Furthermore there is no default main loop function for threads of thread pools with multiple queues. So using `StartThreads` without providing a main loop function will also trigger a static assertion. Same goes for the constructor that starts threads.

Here is a short example of a thread pool with two queues that prioritize tasks from the first queue:

~~~ cpp
ThreadPool<2> pool2Queues;

auto prioritizeFirstQueue = [&]() {
    if (pool2Queues.HasTasks(0))
        pool2Queues.TryExecuteTask(0);
    else
        pool2Queues.TryExecuteTask(1);
};

pool2Queues.StartThreads(2, prioritizeFirstQueue);

for (U32 i = 0; i < 10; ++i)
    pool2Queues.Submit(1, []() {
        std::cout << "Task from second queue processed" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds{1});
    });

for (U32 i = 0; i < 10; ++i)
    pool2Queues.Submit(0, []() {
        std::cout << "Task from first queue processed" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds{1});
    });

while (pool2Queues.HasTasks(0) || pool2Queues.HasTasks(1))
{
    std::this_thread::sleep_for(std::chrono::milliseconds{10});
}

pool2Queues.Deinitialize();
~~~


The output you will get most of the time is that all messages of the first queue are printed in front of the messages of the second queue even though we fill the second queue first. The reason for this is the slow startup process of the threads. The main thread has already finished filling the queues before the worker threads start asking for tasks. However, occasionally it might happen that you see a different ordering of the output messages due to temporary slowdowns in one or more threads.


## Things you should keep in mind

- it is your responsibility to do all necessary synchronization (avoiding data races and deadlocks)
- thread pool threads perform busy waiting by default
- regularly check the thread pools exception log inside your main thread to detect errors as soon as possible
- always call `Deinitialize` before the thread pool is destroyed