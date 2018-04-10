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
    while (!mClose && !mThreadPool.mClose)
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
    mIsClosed = true;
}

} // namespace GDL
