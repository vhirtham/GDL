#pragma once

#include "gdl/resources/cpu/thread.h"

namespace GDL
{

template <typename _function>
Thread::Thread(_function&& function)
    : mThread{function}
{
}

Thread::~Thread()
{
    Close();
}

void Thread::Close()
{
    if (mThread.joinable())
        mThread.join();
}

}
