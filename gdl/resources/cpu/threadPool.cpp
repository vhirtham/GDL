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
    // std::cout << "thread pool deinitializes" << std::endl;
    mClose = true;
    mConditionThreads.notify_all();
    mThreads.clear();
    // std::cout << "all threads cleared" << std::endl;
}

void ThreadPoolNEW::ParentThreadWait()
{

    std::unique_lock<std::mutex> lock(mMutexParentThreadWait);
    mConditionParentThreadWait.wait(lock, [this] { return !mParentThreadWait; });

    mParentThreadWait = true;
    // std::cout << "Parent thread done waiting" << std::endl;
}

void ThreadPoolNEW::ParentThreadContinue()
{
    mParentThreadWait = false;
    mConditionParentThreadWait.notify_one();
}

void ThreadPoolNEW::GetTask()
{
    std::unique_lock<std::mutex> lock(mMutex);
    mConditionThreads.wait(lock, [this] { return !mQueue.IsEmpty() || mClose; });

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
