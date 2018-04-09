#include "gdl/resources/cpu/threadPool.h"


namespace GDL
{


ThreadPoolNEW::~ThreadPoolNEW()
{
    Deinitialize();
}

ThreadPoolNEW::ThreadPoolNEW(const U32 numThreads)
{
    std::lock_guard<std::mutex> lock(mMutex);
    try
    {
        for (U32 i = 0; i < numThreads; ++i)
            mThreads.emplace_back(*this);
    }
    catch (...)
    {
        Deinitialize();
        throw;
    }
}

void ThreadPoolNEW::Deinitialize()
{
    mClose = true;

    while (!mThreads.empty())
    {
        mConditionThreads.notify_all();

        // Give threads a short time to shut down. If you don't and want to use valgrind it will take forever.
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        while (!mThreads.empty() && mThreads.front().IsClosed())
            mThreads.pop_front();
    }
}


void ThreadPoolNEW::GetTask()
{
    // Put in own wait for function (WaitForTasks).
    std::unique_lock<std::mutex> lock(mMutex);
    mConditionThreads.wait(lock, [this] { return !mQueue.IsEmpty() || mClose; });
    lock.unlock();

    std::unique_ptr<TaskBase> task{nullptr};
    if (mQueue.tryPop(task))
    {
        task->execute();
    }
}

bool ThreadPoolNEW::HasTasks()
{
    return !mQueue.IsEmpty();
}



} // namespace GDL
