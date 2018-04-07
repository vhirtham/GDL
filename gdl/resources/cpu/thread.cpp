#include "gdl/resources/cpu/thread.h"

#include "gdl/resources/cpu/threadPool.h"
#include "gdl/resources/cpu/thread.h"

namespace GDL
{


ThreadNEW::~ThreadNEW()
{
    mClose = true;
    mThread.join();
}

ThreadNEW::ThreadNEW(ThreadPoolNEW& threadPool)
    : mClose{false}
    , mThreadPool(threadPool)
    , mThread(&ThreadNEW::Run, this)
{
}

void ThreadNEW::Run()
{
    try
    {
        while (!mClose && !mThreadPool.mClose)
        {
            mThreadPool.GetTask();
        }
    }
    catch (std::exception)
    {
    }
    catch (...)
    {
        std::terminate();
    }
    std::cout << "Thread closed" << std::endl;
}

} // namespace GDL
