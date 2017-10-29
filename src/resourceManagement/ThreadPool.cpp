#include "ThreadPool.h"

GDL::ThreadPool::~ThreadPool()
{
    deinitialize();
}

GDL::ThreadPool::ThreadPool(const GDL::U32 numThreads)
    : mClose(false)
    , mWorkQueue()
    , mThreads()
{
    try
    {
        for (std::uint32_t i = 0u; i < numThreads; ++i)
        {
            mThreads.emplace_back(&ThreadPool::worker, this);
        }
    }
    catch (...)
    {
        deinitialize();
        throw;
    }
}

void GDL::ThreadPool::deinitialize()
{
    mClose = true;
    mWorkQueue.invalidate();
    for (auto& thread : mThreads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}

void GDL::ThreadPool::worker()
{
    while (!mClose)
    {
        std::unique_ptr<ThreadTaskBase> pTask{nullptr};
        if (mWorkQueue.tryWaitPop(pTask))
        {
            pTask->execute();
        }
    }
}
