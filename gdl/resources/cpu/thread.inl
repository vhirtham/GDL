#pragma once

#include "gdl/resources/cpu/thread.h"

namespace GDL
{

template <typename _Func>
Thread::Thread(_Func&& function)
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
