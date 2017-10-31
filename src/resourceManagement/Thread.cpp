#include "Thread.h"

#include "src/resourceManagement/ThreadTaskBase.h"
#include "src/resourceManagement/ThreadPool.h"


GDL::Thread::~Thread()
{
    deinitalize();
}

GDL::Thread::Thread(GDL::ThreadPool& threadPool)
    : mClose(false)
    , mThread(&Thread::run, this)
    , mThreadPool(threadPool)
{
}

void GDL::Thread::deinitalize()
{
    mClose = true;
    if (mThread.joinable())
    {
        mThread.join();
    }
}

void GDL::Thread::run()
{
    while (!mClose)
    {
        std::unique_ptr<ThreadTaskBase> pTask{nullptr};
        if (mThreadPool.mWorkQueue.tryWaitPop(pTask))
        {
            pTask->execute();
        }
    }
}
