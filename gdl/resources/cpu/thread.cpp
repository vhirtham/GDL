#include "gdl/resources/cpu/thread.h"

#include "gdl/resources/cpu/threadPool.h"
#include "gdl/resources/cpu/thread.h"

namespace GDL
{


ThreadNEW::~ThreadNEW()
{
    mClose = true;
    if (mThread.joinable())
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
        while (!mClose)
        {
            std::unique_ptr<TaskBase> pTask{nullptr};
            if (mThreadPool.mQueue.tryPop(pTask))
            {
                pTask->execute();
            }
        }
    }
    catch (std::exception)
    {
    }
    catch (...)
    {
        std::terminate();
    }
}

} // namespace GDL
