#pragma once

#include "gdl/resources/cpu/threadPool.h"

#include "gdl/base/exception.h"


#include <cassert>
#include <functional>


namespace GDL
{

template <I32 _numQueues>
ThreadPool<_numQueues>::~ThreadPool()
{
    CloseAllThreads();
}



template <I32 _numQueues>
ThreadPool<_numQueues>::ThreadPool(U32 numThreads)
{
    static_assert(_numQueues == 1, "This thread pool has multiple queues. Can't start threads. There is no default "
                                   "main loop for this case. Use the default constructor and add threads with a "
                                   "specific main loop function ---> \"StartThreads(numThreads, function)\".");
    StartThreads(numThreads);
}



template <I32 _numQueues>
void ThreadPool<_numQueues>::Deinitialize()
{
    CloseAllThreads();
    PropagateExceptions();
}



template <I32 _numQueues>
U32 ThreadPool<_numQueues>::GetNumThreads() const
{
    std::lock_guard<std::mutex> lock(mMutexThreads);
    return mThreads.size();
}



template <I32 _numQueues>
void ThreadPool<_numQueues>::StartThreads(U32 numThreads)
{
    static_assert(_numQueues == 1, "This thread pool has multiple queues. Can't start threads. There is no default "
                                   "main loop for this case. Use \"StartThreads(numThreads, function)\" and provide a "
                                   "main loop function.");

    StartThreads(numThreads, [this] { TryExecuteTask(); });
}



template <I32 _numQueues>
template <typename _function>
void ThreadPool<_numQueues>::StartThreads(U32 numThreads, _function&& function)
{
    StartThreads(numThreads, function, []() {}, []() {});
}



template <I32 _numQueues>
template <typename _function, typename _initFunction, typename _deinitFunction>
void ThreadPool<_numQueues>::StartThreads(U32 numThreads, _function&& function, _initFunction&& initFunction,
                                          _deinitFunction&& deinitFunction)
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



template <I32 _numQueues>
void ThreadPool<_numQueues>::CloseThreads(I32 numThreadsToClose)
{
    std::lock_guard<std::mutex> lock(mMutexThreads);

    const I32 numRunningThreads = static_cast<I32>(mThreads.size());

    if (numRunningThreads < numThreadsToClose)
        numThreadsToClose = numRunningThreads;
    assert(numRunningThreads >= numThreadsToClose && numThreadsToClose > 0);

    // Notify threads to exit main loop
    for (I32 i = numRunningThreads; i > numRunningThreads - numThreadsToClose; --i)
        mThreads[i - 1].Close();

    // Pop threads from the back of the container
    for (I32 i = 0; i < numThreadsToClose; ++i)
        mThreads.pop_back();
}



template <I32 _numQueues>
void ThreadPool<_numQueues>::CloseAllThreads()
{
    std::lock_guard<std::mutex> lock(mMutexThreads);

    for (auto& thread : mThreads)
        thread.Close();
    mThreads.clear();
}



template <I32 _numQueues>
bool ThreadPool<_numQueues>::HasTasks() const
{
    static_assert(_numQueues == 1, "This thread pool has multiple queues. Use the corresponding function overload to "
                                   "specify which one you want to use.");
    return !mQueue[0].IsEmpty();
}



template <I32 _numQueues>
bool ThreadPool<_numQueues>::HasTasks(const I32 queueNum) const
{
    assert(queueNum < _numQueues && queueNum >= 0);
    return !mQueue[queueNum].IsEmpty();
}



template <I32 _numQueues>
U64 ThreadPool<_numQueues>::GetNumTasks() const
{
    static_assert(_numQueues == 1, "This thread pool has multiple queues. Use the corresponding function overload to "
                                   "specify which one you want to use.");
    return mQueue[0].GetSize();
}



template <I32 _numQueues>
U32 ThreadPool<_numQueues>::GetNumTasks(const I32 queueNum) const
{
    assert(queueNum < _numQueues && queueNum >= 0);
    return mQueue[queueNum].GetSize();
}



template <I32 _numQueues>
bool ThreadPool<_numQueues>::TryExecuteTask()
{
    static_assert(_numQueues == 1, "This thread pool has multiple queues. Use the corresponding function overload to "
                                   "specify which one you want to use.");
    return TryExecuteTask(0);
}



template <I32 _numQueues>
bool ThreadPool<_numQueues>::TryExecuteTask(const I32 queueNum)
{
    assert(queueNum < _numQueues && queueNum >= 0);
    UniquePtr<TaskBase> task{nullptr};
    if (mQueue[queueNum].TryPop(task))
    {
        task->execute();
        return true;
    }
    return false;
}



template <I32 _numQueues>
template <typename _function, typename... _args>
void ThreadPool<_numQueues>::Submit(const I32 queueNum, _function&& function, _args&&... args)
{
    using ResultType =
            std::result_of_t<decltype(std::bind(std::forward<_function>(function), std::forward<_args>(args)...))()>;
    using TaskType = Task<decltype(std::bind(std::forward<_function>(function), std::forward<_args>(args)...))>;


    static_assert(std::is_same<void, ResultType>::value, "Used submit() with non void function!");
    assert(queueNum < _numQueues && queueNum >= 0);


    mQueue[queueNum].Push(
            MakeUnique<TaskType>(std::bind(std::forward<_function>(function), std::forward<_args>(args)...)));
}



template <I32 _numQueues>
template <typename _function, typename... _args>
void ThreadPool<_numQueues>::Submit(_function&& function, _args&&... args)
{
    static_assert(_numQueues == 1, "This thread pool has multiple queues. Use the corresponding function overload to "
                                   "specify which one you want to use.");
    Submit(0, function, args...);
}



template <I32 _numQueues>
void ThreadPool<_numQueues>::ClearExceptionLog()
{
    std::lock_guard<std::mutex> lock(mMutexExceptionLog);
    mExceptionLog.clear();
}



template <I32 _numQueues>
U32 ThreadPool<_numQueues>::GetExceptionLogSize() const
{
    std::lock_guard<std::mutex> lock(mMutexExceptionLog);
    return mExceptionLog.size();
}



template <I32 _numQueues>
void ThreadPool<_numQueues>::PropagateExceptions() const
{
    std::lock_guard<std::mutex> lock(mMutexExceptionLog);
    EXCEPTION(!mExceptionLog.empty(), mExceptionLog.c_str());
}



template <I32 _numQueues>
template <typename... _args>
void ThreadPool<_numQueues>::AddMessageToExceptionLog(const _args&... args)
{
    std::lock_guard<std::mutex> lock(mMutexExceptionLog);
    AppendToString(mExceptionLog, args..., "\n\n");
}

} // namespace GDL
