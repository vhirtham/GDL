#pragma once

#include "gdl/resources/cpu/threadPool.h"


#include "gdl/base/exception.h"


#include <cassert>

namespace GDL
{

template <I32 _NumQueues>
ThreadPool<_NumQueues>::~ThreadPool()
{
    Deinitialize();
}



template <I32 _NumQueues>
ThreadPool<_NumQueues>::ThreadPool(U32 numThreads)
{
    Initialize();
    StartThreads(numThreads);
}



template <I32 _NumQueues>
void ThreadPool<_NumQueues>::Deinitialize()
{
    CloseAllThreads();
    PropagateExceptions();
}



template <I32 _NumQueues>
void ThreadPool<_NumQueues>::Initialize()
{
    std::lock_guard<std::mutex> lock(mMutexExceptionLog);
    mExceptionLog.reserve(200);
}



template <I32 _NumQueues>
U32 ThreadPool<_NumQueues>::GetNumThreads() const
{
    std::lock_guard<std::mutex> lock(mMutexThreads);
    return mThreads.size();
}



template <I32 _NumQueues>
void ThreadPool<_NumQueues>::StartThreads(U32 numThreads)
{
    StartThreads(numThreads, [this] { TryExecuteTaskWait(); });
}



template <I32 _NumQueues>
template <typename _Func>
void ThreadPool<_NumQueues>::StartThreads(U32 numThreads, _Func&& function)
{
    StartThreads(numThreads, function, []() {}, []() {});
}



template <I32 _NumQueues>
template <typename _Func, typename _InitFunction, typename _DeinitFunction>
void ThreadPool<_NumQueues>::StartThreads(U32 numThreads, _Func&& function, _InitFunction&& initFunction,
                                          _DeinitFunction&& deinitFunction)
{
    static_assert(std::is_same<void, std::result_of_t<decltype(function)()>>::value,
                  "The threads main loop function should not return any value.");
    static_assert(std::is_same<void, std::result_of_t<decltype(initFunction)()>>::value,
                  "The threads initialization function should not return any value.");
    static_assert(std::is_same<void, std::result_of_t<decltype(initFunction)()>>::value,
                  "The threads deinitialization function should not return any value.");

    std::lock_guard<std::mutex> lock(mMutexThreads);
    for (U32 i = 0; i < numThreads; ++i)
        mThreads.emplace_back(*this, std::move(function), std::move(initFunction), std::move(deinitFunction));
}



template <I32 _NumQueues>
void ThreadPool<_NumQueues>::CloseThreads(I32 numThreadsToClose)
{
    std::lock_guard<std::mutex> lock(mMutexThreads);
    mCloseThreads = true;

    const I32 numRunningThreads = static_cast<I32>(mThreads.size());
    if (numRunningThreads < numThreadsToClose || numThreadsToClose < 0)
        numThreadsToClose = numRunningThreads;
    assert(numRunningThreads >= numThreadsToClose);


    // Notify threads to exit main loop
    for (I32 i = numRunningThreads; i > numRunningThreads - numThreadsToClose; --i)
        mThreads[i - 1].Close();


    // Pop threads from the back of the container
    for (I32 i = 0; i < numThreadsToClose; ++i)
    {
        mConditionThreads.notify_all();

        // Give thread a short time to shut down. If you don't and want to use valgrind it will take forever.
        while (!mThreads.back().HasFinished())
            std::this_thread::yield();

        mThreads.pop_back();
    }
    mCloseThreads = false;
}


template <I32 _NumQueues>
void ThreadPool<_NumQueues>::CloseAllThreads()
{
    CloseThreads(-1);
}



template <I32 _NumQueues>
bool ThreadPool<_NumQueues>::HasTasks() const
{
    for (U32 i = 0; i < mQueue.size(); ++i)
        if (!mQueue[i].IsEmpty())
            return true;
    return false;
}

template <I32 _NumQueues>
bool ThreadPool<_NumQueues>::HasTasks(I32 queueNum) const
{
    assert(queueNum < _NumQueues && queueNum >= 0);
    return !mQueue[queueNum].IsEmpty();
}



template <I32 _NumQueues>
U32 ThreadPool<_NumQueues>::GetNumTasks() const
{
    U32 numTasks = 0;
    for (U32 i = 0; i < mQueue.size(); ++i)
        numTasks += mQueue[i].GetSize();
    return numTasks;
}

template <I32 _NumQueues>
U32 ThreadPool<_NumQueues>::GetNumTasks(I32 queueNum) const
{
    assert(queueNum < _NumQueues && queueNum >= 0);
    return mQueue[queueNum].GetSize();
}



template <I32 _NumQueues>
bool ThreadPool<_NumQueues>::TryExecuteTask()
{
    UniquePtr<TaskBase> task{nullptr};
    for (U32 i = 0; i < mQueue.size(); ++i)
        if (mQueue[i].TryPop(task))
        {
            task->execute();
            return true;
        }
    return false;
}

template <I32 _NumQueues>
bool ThreadPool<_NumQueues>::TryExecuteTask(I32 queueNum)
{
    assert(queueNum < _NumQueues && queueNum >= 0);
    UniquePtr<TaskBase> task{nullptr};
    if (mQueue[queueNum].TryPop(task))
    {
        task->execute();
        return true;
    }
    return false;
}



template <I32 _NumQueues>
bool ThreadPool<_NumQueues>::TryExecuteTaskWait()
{
    WaitForTask();
    return TryExecuteTask();
}



template <I32 _NumQueues>
void ThreadPool<_NumQueues>::WaitForTask()
{
    std::unique_lock<std::mutex> lock(mMutexCondition);
    mConditionThreads.wait(lock, [this] { return HasTasks() || mCloseThreads; });
}



template <I32 _NumQueues>
template <typename _F, typename... _Args>
void ThreadPool<_NumQueues>::Submit(_F&& function, _Args&&... args)
{
    SubmitToQueue(0, function, args...);
}


template <I32 _NumQueues>
template <typename _F, typename... _Args>
void ThreadPool<_NumQueues>::SubmitToQueue(I32 queueNum, _F&& function, _Args&&... args)
{
    using ResultType =
            std::result_of_t<decltype(std::bind(std::forward<_F>(function), std::forward<_Args>(args)...))()>;
    using TaskType = Task<decltype(std::bind(std::forward<_F>(function), std::forward<_Args>(args)...))>;

    static_assert(std::is_same<void, ResultType>::value, "Used submit() with non void function!");
    assert(queueNum < _NumQueues && queueNum >= 0);


    mQueue[queueNum].Push(MakeUnique<TaskType>(std::bind(std::forward<_F>(function), std::forward<_Args>(args)...)));
    mConditionThreads.notify_one();
}



template <I32 _NumQueues>
void ThreadPool<_NumQueues>::ClearExceptionLog()
{
    std::lock_guard<std::mutex> lock(mMutexExceptionLog);
    mExceptionLog.clear();
}

template <I32 _NumQueues>
U32 ThreadPool<_NumQueues>::ExceptionLogSize() const
{
    std::lock_guard<std::mutex> lock(mMutexExceptionLog);
    return mExceptionLog.size();
}



template <I32 _NumQueues>
void ThreadPool<_NumQueues>::PropagateExceptions() const
{
    std::lock_guard<std::mutex> lock(mMutexExceptionLog);
    if (!mExceptionLog.empty())
        throw Exception(__PRETTY_FUNCTION__, mExceptionLog.c_str());
}

} // namespace GDL
