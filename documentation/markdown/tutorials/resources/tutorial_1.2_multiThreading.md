# Multi Threading

The GDL offers two classes that will help you running multiple threads in parallel.However, multi threading is a complex topic and if you are not familiar with using `std::thread`s you should probably learn about using them first.

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

That' it. Except for the varying thread ID the output of this program looks like this:

~~~ Shell
Hello from thread with ID 140314797389568!
~~~

All this program does so far is starting a thread which prints a hello world message and closing it afterwards. Keep in mind that the destructor of the Thread class tries to join the thread. So if you are using a loop that waits for an exit condition to be fulfilled, you will create a deadlock if you don't give the exit signal before the destructor is called.

## Thread pool

Starting a thread is a rather expensive operation. It is not a good idea to start a thread for every new task that should be processed. A better solution is to create a certain number of threads at the beginning of your program and feed them continuously with new tasks. A thread pool is a class that does exactly that. It manages an arbitrary number of threads and 1 or more internal queues that store the tasks that should be processed. We add the following header to our program:

~~~ cpp
#include "gdl/resources/cpu/threadPool.h"
~~~

Now we can create a thread pool:

~~~ cpp
ThreadPool<> pool;
~~~

As you can see the thread pool is a template class. The template parameter is a 32 bit integer value and determines how many queues it should have. Since we did not provide any number, it will just have a single queue by default.
The default constructor of the thread pool won't start any threads for you, so you can't process any tasks so far. To start a thread, we simply call:

~~~ cpp
pool.StartThreads(2);
~~~

This will start two threads that we can now feed with some tasks. 


### Submitting tasks

As a first exaample how to submit tasks we will enqueue our `HelloFromThread` function two times:

~~~ cpp
for (U32 i = 0; i < 2; ++i)
    pool.Submit(HelloFromThread);

std::this_thread::sleep_for(std::chrono::milliseconds{10});
~~~

`Submit` passes a function that should be executed by one of the threads to the thread pools queue. Notice that there are no parenthesis after `HelloFromThread` since we don't want to execute it. We just pass the function itself.

The sleep function prevents the main thread to continue for 10 milliseconds. Without it, the main thread might call the destructor of the thread pool while the threads are still starting. This is not a problem in general, because the destructor will wait for the threads to join, but in this case the started threads will never ask the queue for any tasks if they get the shutdown signal directly after the startup. Hence, you won't get any output.
This is a quiet bad and error prone way of synchronization that you should not use in your programs, but synchronization is an own and complicated topic and we want to focus here just on the features of the thread pool. Therefore it is sufficient. For this reason we will also not go into detail how to solve the problem that the output you get is quiet a mess due to the data race for `std::cout`.

The functions or callable objects that you pass to the queue can also have parameters. You have to provide values for them as additional parameters of the `Submit` function. Let's create another function with parameters:

~~~ cpp
void PrintValue(I32 value)
{
    std::cout << "value: " << value << std::endl;
}
~~~

Now we add:

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

### Stopping threads

Once you have started one or more threads, you can stop them whenever you like. To do so, use:

~~~ cpp
pool.CloseThreads(1);
~~~ 

Here we close one of the two started threads. It finishes its current task, is joined and will be removed afterwards. You can specify any number of threads that should be closed.
At this point it does not really matter which one of the two threads is closed, but as you will see later it might be important to know. The threads that were started last are also closed first (last in, first out).

You can also close all running threads by calling:

~~~ cpp
pool.CloseAllThreads();
~~~

### Tread pool threads


### Multiple queues

- how does a thread work
- custom thread functions
- ctor with I32
- Exception handling
- multiple queues
