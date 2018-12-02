#include "gdl/resources/cpu/thread.h"

#include <iostream>

using namespace GDL;


void HelloFromThread()
{
    std::cout << "Hello from thread with ID " << std::this_thread::get_id() << "!" << std::endl;
}


int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    Thread thread(HelloFromThread);
}
