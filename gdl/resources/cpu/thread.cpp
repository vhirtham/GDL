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
    , mFinished{false}
    , mThreadPool(threadPool)
    , mThread(&ThreadNEW::Run, this)
{
}

void ThreadNEW::Close()
{
    mClose = true;
}

bool ThreadNEW::HasFinished() const
{
    return mFinished;
}

void ThreadNEW::Run()
{
    while (!mClose && !mThreadPool.mCloseThreads)
    {
        try
        {
            mThreadPool.TryExecuteTaskWait();
        }
        catch (const std::exception& e)
        {
            mThreadPool.mExceptionLog.append("\n");
            mThreadPool.mExceptionLog.append("Thread caught the following Excption:\n");
            mThreadPool.mExceptionLog.append(e.what());
            mThreadPool.mExceptionLog.append("\n");
        }
        catch (...)
        {
            mThreadPool.mExceptionLog.append("\n");
            mThreadPool.mExceptionLog.append("Thread caught UNKNOWN exception");
            mThreadPool.mExceptionLog.append("\n");
        }
    }
    mFinished = true;
}

} // namespace GDL
