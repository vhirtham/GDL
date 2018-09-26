#pragma once

#include "gdl/resources/cpu/threadPoolThread.h"

#include <mutex>


namespace GDL
{


template <I32 _numThreadPoolQueues>
ThreadPoolThread<_numThreadPoolQueues>::~ThreadPoolThread()
{
    mThread.join();
}



template <I32 _numThreadPoolQueues>
template <typename _Func, typename _InitFunction, typename _DeinitFunction>
ThreadPoolThread<_numThreadPoolQueues>::ThreadPoolThread(ThreadPool<_numThreadPoolQueues>& threadPool, _Func&& function,
                                                         _InitFunction&& initFunction, _DeinitFunction&& deinitFunction)
    : mClose{false}
    , mFinished{false}
    , mThreadPool(threadPool)
    , mThread(&ThreadPoolThread::Run<_Func, _InitFunction, _DeinitFunction>, this, function, initFunction,
              deinitFunction)
{
}



template <I32 _numThreadPoolQueues>
void ThreadPoolThread<_numThreadPoolQueues>::Close()
{
    mClose = true;
}



template <I32 _numThreadPoolQueues>
bool ThreadPoolThread<_numThreadPoolQueues>::HasFinished() const
{
    return mFinished;
}



template <I32 _numThreadPoolQueues>
template <typename _Func, typename _InitFunction, typename _DeinitFunction>
void ThreadPoolThread<_numThreadPoolQueues>::Run(_Func&& function, _InitFunction&& initFunction,
                                                 _DeinitFunction&& deinitFunction)
{
    // Initialization
    HandleExceptions([&]() { initFunction(); });

    // Main loop
    HandleExceptions([&]() {
        while (!mClose)
            function();
    });

    // Deinitialization
    HandleExceptions([&]() { deinitFunction(); });

    mFinished = true;
}



template <I32 _numThreadPoolQueues>
template <typename _Func>
void ThreadPoolThread<_numThreadPoolQueues>::HandleExceptions(_Func&& function)
{
    try
    {
        function();
    }
    catch (const std::exception& e)
    {
        std::lock_guard<std::mutex> lock(mThreadPool.mMutexExceptionLog);
        mThreadPool.mExceptionLog.append("\n");
        mThreadPool.mExceptionLog.append("Thread caught the following Excption:\n");
        mThreadPool.mExceptionLog.append(e.what());
        mThreadPool.mExceptionLog.append("\n");
        mClose = true;
    }
    catch (...)
    {
        std::lock_guard<std::mutex> lock(mThreadPool.mMutexExceptionLog);
        mThreadPool.mExceptionLog.append("\n");
        mThreadPool.mExceptionLog.append("Thread caught UNKNOWN exception");
        mThreadPool.mExceptionLog.append("\n");
        mClose = true;
    }
}


} // namespace GDL
