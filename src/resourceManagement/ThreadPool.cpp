#include "ThreadPool.h"


#include "src/resourceManagement/Thread.h"

GDL::ThreadPool::~ThreadPool()
{
    deinitialize();
}

GDL::ThreadPool::ThreadPool(const U32 numThreads)
    : mWorkQueue()
{
    try
    {
        for (U32 i = 0u; i < numThreads; ++i)
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

void GDL::ThreadPool::deinitialize()
{
    mWorkQueue.invalidate();
    mThreads.clear();
}
