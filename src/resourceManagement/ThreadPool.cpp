#include "ThreadPool.h"


#include "src/base/Exception.h"
#include "src/resourceManagement/Thread.h"

GDL::ThreadPool::~ThreadPool()
{
    deinitialize();
}

GDL::ThreadPool::ThreadPool(const U32 numThreads)
    : mWorkQueue()
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
        mThreads.pop_back();
    }
}


GDL::U32 GDL::ThreadPool::getNumThreads() const
{
    std::lock_guard<std::mutex> lock(mMutexThreads);
    return mThreads.size();
}

void GDL::ThreadPool::deinitialize()
{
    mWorkQueue.invalidate();
    std::lock_guard<std::mutex> lock(mMutexThreads);
    mThreads.clear();
}
