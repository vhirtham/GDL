#include "gdl/resources/cpu/thread.h"

#include "gdl/resources/cpu/threadPool.h"
#include "gdl/resources/cpu/thread.h"

namespace GDL
{


ThreadNEW::~ThreadNEW()
{
    mThread.join();
}

ThreadNEW::ThreadNEW(ThreadPoolNEW& threadPool)
    : mClose{false}
    , mIsClosed{false}
    , mThreadPool(threadPool)
    , mThread(&ThreadNEW::Run, this)
{
}

bool ThreadNEW::IsClosed() const
{
    return mIsClosed;
}

void ThreadNEW::Run()
{
    try
    {
        while (!mClose && !mThreadPool.mClose)
        {
            mThreadPool.TryExecuteTaskWait();
        }
    }
    catch (std::exception)
    {
    }
    catch (...)
    {
        std::terminate();
    }
    mIsClosed = true;
}

} // namespace GDL
