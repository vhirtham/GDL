#pragma once

#include "gdl/resources/cpu/threadPoolThread.h"
#include "gdl/base/string.h"
#include <mutex>


namespace GDL
{


template <I32 _numThreadPoolQueues>
ThreadPoolThread<_numThreadPoolQueues>::~ThreadPoolThread()
{
    mThread.join();
}



template <I32 _numThreadPoolQueues>
template <typename _function, typename _initFunction, typename _DeinitFunction>
ThreadPoolThread<_numThreadPoolQueues>::ThreadPoolThread(ThreadPool<_numThreadPoolQueues>& threadPool,
                                                         _function&& function, _initFunction&& initFunction,
                                                         _DeinitFunction&& deinitFunction)
    : mClose{false}
    , mThreadPool(threadPool)
    , mThread(&ThreadPoolThread::Run<_function, _initFunction, _DeinitFunction>, this, function, initFunction,
              deinitFunction)
{
}



template <I32 _numThreadPoolQueues>
void ThreadPoolThread<_numThreadPoolQueues>::Close()
{
    mClose = true;
}



template <I32 _numThreadPoolQueues>
template <typename _function, typename _initFunction, typename _DeinitFunction>
void ThreadPoolThread<_numThreadPoolQueues>::Run(_function&& function, _initFunction&& initFunction,
                                                 _DeinitFunction&& deinitFunction)
{
    // INFO:
    // The 3 functions have individual exception handling to ensure that the deinitialize function is called

    // Initialization
    HandleExceptions(initFunction);

    // Main loop
    HandleExceptions([&]() {
        while (!mClose)
            function();
    });

    // Deinitialization
    HandleExceptions(deinitFunction);
}



template <I32 _numThreadPoolQueues>
template <typename _function>
void ThreadPoolThread<_numThreadPoolQueues>::HandleExceptions(_function&& function)
{
    try
    {
        function();
    }
    catch (const std::exception& e)
    {
        std::lock_guard<std::mutex> lock(mThreadPool.mMutexExceptionLog);
        AppendToString(mThreadPool.mExceptionLog,"\nThread caught the following Excption:\n",e.what(),"\n");
        mClose = true;
    }
    catch (...)
    {
        std::lock_guard<std::mutex> lock(mThreadPool.mMutexExceptionLog);
        mThreadPool.mExceptionLog.append("\nThread caught UNKNOWN exception\n");
        mClose = true;
    }
}


} // namespace GDL
