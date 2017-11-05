#include "ThreadPool.h"


#include "src/base/Exception.h"
#include "src/resourceManagement/Thread.h"

GDL::ThreadPool::~ThreadPool()
{
    deinitialize();
}

GDL::ThreadPool::ThreadPool(const U32 numThreads)
    : mClose(false)
#ifndef NDEBUG
    , mRunningThreads(0)
#endif
    , mWorkQueue()
{
    addThreads(numThreads);
}

void GDL::ThreadPool::addThreads(GDL::U32 numThreads)
{
    std::lock_guard<std::mutex> lock(mMutexThreads);
    try
    {
        for (U32 iThreads = 0; iThreads < numThreads; ++iThreads)
        {
            mThreads.emplace_back(*this);
        }
    }
    catch (...)
    {
        deinitialize();
        throw;
    }
}

void GDL::ThreadPool::killThreads(GDL::U32 numThreads)
{
    std::lock_guard<std::mutex> lock(mMutexThreads);
    for (U32 iThreads = 0; iThreads < numThreads; ++iThreads)
    {
        if (mThreads.size() == 1)
        {
            // deinitialize();
            std::terminate();
            throw Exception(__PRETTY_FUNCTION__, "No more threads left! Deinitialized thread pool.");
        }

        mThreads.back().deinitializeDetach();
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        mThreads.pop_back();
    }
}


GDL::U32 GDL::ThreadPool::getNumThreads() const
{
    std::lock_guard<std::mutex> lock(mMutexThreads);
    return mThreads.size();
}

#ifndef NDEBUG
GDL::U32 GDL::ThreadPool::getNumRunningThreads() const
{
    return mRunningThreads;
}
#endif

void GDL::ThreadPool::deinitialize()
{
    mClose = true;
    std::lock_guard<std::mutex> lock(mMutexThreads);
    mWorkQueue.invalidate();
    mThreads.clear();
}
