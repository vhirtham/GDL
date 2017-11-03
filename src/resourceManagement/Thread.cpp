#include "Thread.h"

#include "src/resourceManagement/ThreadTaskBase.h"
#include "src/resourceManagement/ThreadPool.h"


GDL::Thread::~Thread()
{
    deinitalize();
}

GDL::Thread::Thread(GDL::ThreadPool& threadPool)
    : mClose(false)
    , mThreadPool(threadPool)
    , mThread(&Thread::run, this) // <--- never move this in front of thread pool reference! Reason in class docu
{
}

void GDL::Thread::deinitalize()
{
    mClose = true;
    mThread.join();
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
