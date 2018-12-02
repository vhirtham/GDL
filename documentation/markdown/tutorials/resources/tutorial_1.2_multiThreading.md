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

