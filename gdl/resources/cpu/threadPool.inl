#pragma once

#include "gdl/resources/cpu/threadPool.h"


#include "gdl/base/Exception.h"


#include <cassert>

namespace GDL
{

template <int _NumQueues>
template <typename _Func>
ThreadPool<_NumQueues>::Thread::Thread(ThreadPool& threadPool, _Func&& function)
    : mClose{false}
    , mFinished{false}
    , mThreadPool(threadPool)
    , mThread(&Thread::Run<_Func>, this, function)
{
}



template <int _NumQueues>
ThreadPool<_NumQueues>::Thread::~Thread()
{
    mThread.join();
}



template <int _NumQueues>
void ThreadPool<_NumQueues>::Thread::Close()
{
    mClose = true;
}



template <int _NumQueues>
bool ThreadPool<_NumQueues>::Thread::HasFinished() const
{
    return mFinished;
}



template <int _NumQueues>
template <typename _Func>
void ThreadPool<_NumQueues>::Thread::Run(_Func&& function)
{
    while (!mClose && !mThreadPool.mCloseThreads)
    {
        try
        {
            function();
        }
        catch (const std::exception& e)
        {
            mThreadPool.mExceptionLog.append("\n");
            mThreadPool.mExceptionLog.append("Thread caught the following Excption:\n");
            mThreadPool.mExceptionLog.append(e.what());
            mThreadPool.mExceptionLog.append("\n");
        }
        catch (...)
        {
            mThreadPool.mExceptionLog.append("\n");
            mThreadPool.mExceptionLog.append("Thread caught UNKNOWN exception");
            mThreadPool.mExceptionLog.append("\n");
        }
    }
    mFinished = true;
}



template <int _NumQueues>
ThreadPool<_NumQueues>::~ThreadPool()
{
    Deinitialize();
}



template <int _NumQueues>
ThreadPool<_NumQueues>::ThreadPool(const U32 numThreads)
{
    Initialize();
    StartThreads(numThreads);
}



template <int _NumQueues>
void ThreadPool<_NumQueues>::Deinitialize()
{
    CloseAllThreads();
    PropagateExceptions();
}



template <int _NumQueues>
void ThreadPool<_NumQueues>::Initialize()
{
    std::lock_guard<std::mutex> lock(mMutex);
    mExceptionLog.reserve(200);
}



template <int _NumQueues>
U32 ThreadPool<_NumQueues>::GetNumThreads() const
{
    std::lock_guard<std::mutex> lock(mMutex);
    return mThreads.size();
}



template <int _NumQueues>
void ThreadPool<_NumQueues>::StartThreads(U32 numThreads)
{
    StartThreads(numThreads, [this] { TryExecuteTaskWait(); });
}



template <int _NumQueues>
template <typename _Func, typename... _Args>
void ThreadPool<_NumQueues>::StartThreads(U32 numThreads, _Func&& function, _Args&&... args)
{
    std::lock_guard<std::mutex> lock(mMutex);
    for (U32 i = 0; i < numThreads; ++i)
        mThreads.emplace_back(*this, std::bind(std::forward<_Func>(function), std::forward<_Args>(args)...));
}



template <int _NumQueues>
void ThreadPool<_NumQueues>::CloseThreads(U32 numThreads)
{
    std::lock_guard<std::mutex> lock(mMutex);

    if (mThreads.size() < numThreads)
        numThreads = mThreads.size();

    mCloseThreads = true;
    for (U32 i = 0; i < numThreads; ++i)
    {
        mThreads.back().Close();
        while (!mThreads.back().HasFinished())
        {
            mConditionThreads.notify_all();
            // Give thread a short time to shut down. If you don't and want to use valgrind it will take forever.
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
        mThreads.pop_back();
    }
    mCloseThreads = false;
}



template <int _NumQueues>
void ThreadPool<_NumQueues>::CloseAllThreads()
{
    std::lock_guard<std::mutex> lock(mMutex);
    mCloseThreads = true;
    while (!mThreads.empty())
    {
        mConditionThreads.notify_all();

        // Give threads a short time to shut down. If you don't and want to use valgrind it will take forever.
        std::this_thread::sleep_for(std::chrono::microseconds(1));

        while (!mThreads.empty() && mThreads.back().HasFinished())
            mThreads.pop_back();
    }
    mCloseThreads = false;
}



template <int _NumQueues>
bool ThreadPool<_NumQueues>::HasTasks() const
{
    for (U32 i = 0; i < mQueue.size(); ++i)
        if (!mQueue[i].IsEmpty())
            return true;
    return false;
}



template <int _NumQueues>
U32 ThreadPool<_NumQueues>::GetNumTasks() const
{
    U32 numTasks = 0;
    for (U32 i = 0; i < mQueue.size(); ++i)
        numTasks += mQueue[i].GetSize();
    return numTasks;
}



template <int _NumQueues>
void ThreadPool<_NumQueues>::TryExecuteTask()
{
    std::unique_ptr<TaskBase> task{nullptr};
    for (U32 i = 0; i < mQueue.size(); ++i)
        if (mQueue[i].tryPop(task))
        {
            task->execute();
            break;
        }
}



template <int _NumQueues>
void ThreadPool<_NumQueues>::TryExecuteTaskWait()
{
    WaitForTask();
    TryExecuteTask();
}



template <int _NumQueues>
void ThreadPool<_NumQueues>::WaitForTask()
{
    std::unique_lock<std::mutex> lock(mMutexCondition);
    mConditionThreads.wait(lock, [this] { return HasTasks() || mCloseThreads; });
}



template <int _NumQueues>
template <typename _F, typename... _Args>
void ThreadPool<_NumQueues>::Submit(_F&& function, _Args&&... args)
{
    Submit(0, function, std::forward(args)...);
}


template <int _NumQueues>
template <typename _F, typename... _Args>
void ThreadPool<_NumQueues>::Submit(I32 queueNum, _F&& function, _Args&&... args)
{
    using ResultType =
            std::result_of_t<decltype(std::bind(std::forward<_F>(function), std::forward<_Args>(args)...))()>;
    using TaskType = Task<decltype(std::bind(std::forward<_F>(function), std::forward<_Args>(args)...))>;

    static_assert(std::is_same<void, ResultType>::value, "Used submit() with non void function!");
    assert(queueNum < _NumQueues && queueNum >=0);

    mQueue[queueNum].Push(
            std::make_unique<TaskType>(std::bind(std::forward<_F>(function), std::forward<_Args>(args)...)));
    mConditionThreads.notify_one();
}



template <int _NumQueues>
void ThreadPool<_NumQueues>::ClearExceptionLog()
{
    std::lock_guard<std::mutex> lock(mMutex);
    mExceptionLog.clear();
}



template <int _NumQueues>
void ThreadPool<_NumQueues>::PropagateExceptions() const
{
    std::lock_guard<std::mutex> lock(mMutex);
    if (!mExceptionLog.empty())
        throw Exception(__PRETTY_FUNCTION__, mExceptionLog);
}

} // namespace GDL
