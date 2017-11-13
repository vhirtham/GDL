#include "Thread.h"

#include "resourceManagement/ThreadTaskBase.h"
#include "resourceManagement/ThreadPool.h"



GDL::Thread::~Thread()
{
    deinitializeJoin();
}

GDL::Thread::Thread(GDL::ThreadPool& threadPool)
    : mClose(false)
    , mThreadPool(threadPool)
    , mThread(&Thread::run, this) // <--- never move this in front of thread pool reference! Reason in class doc
{
#ifndef NDEBUG
    ++mThreadPool.mRunningThreads;
#endif
}

void GDL::Thread::deinitializeDetach()
{
    mClose = true;
    mThread.detach();
}

void GDL::Thread::deinitializeJoin()
{
    mClose = true;
    if (mThread.joinable())
        mThread.join();
}

void GDL::Thread::run()
{
    while (!mClose && !mThreadPool.mClose)
    {
        std::unique_ptr<ThreadTaskBase> pTask{nullptr};
        if (mThreadPool.mWorkQueue.tryWaitPop(pTask))
        {
            pTask->execute();
        }
    }
#ifndef NDEBUG
    --mThreadPool.mRunningThreads;
#endif
}
