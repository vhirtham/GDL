#include "gdl/resources/cpu/threadPool.h"


namespace GDL
{


ThreadPoolNEW::~ThreadPoolNEW()
{
    Deinitialize();
}

ThreadPoolNEW::ThreadPoolNEW(const U32 numThreads)
{
    std::lock_guard<std::mutex> lock(mMutexThreads);
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
    mThreads.clear();
}


} // namespace GDL
