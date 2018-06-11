#pragma once

#include "gdl/resources/cpu/threadPool.h"


#include "gdl/base/Exception.h"


#include <cassert>

namespace GDL
{


template <typename _Func>
ThreadPool::Thread::Thread(ThreadPool& threadPool, _Func&& function)
    : mClose{false}
    , mFinished{false}
    , mThreadPool(threadPool)
    , mThread(&Thread::Run<_Func>, this, function)
{
}



ThreadPool::Thread::~Thread()
{
    mThread.join();
}



void ThreadPool::Thread::Close()
{
    mClose = true;
}



bool ThreadPool::Thread::HasFinished() const
{
    return mFinished;
}



template <typename _Func>
void ThreadPool::Thread::Run(_Func&& function)
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



ThreadPool::~ThreadPool()
{
    Deinitialize();
}



ThreadPool::ThreadPool(const U32 numThreads)
{
    Initialize();
    StartThreads(numThreads);
}



void ThreadPool::Deinitialize()
{
    CloseAllThreads();
    PropagateExceptions();
}



void ThreadPool::Initialize()
{
    std::lock_guard<std::mutex> lock(mMutex);
    mExceptionLog.reserve(200);
}



U32 ThreadPool::GetNumThreads() const
{
    std::lock_guard<std::mutex> lock(mMutex);
    return mThreads.size();
}



void ThreadPool::StartThreads(U32 numThreads)
{
    StartThreads(numThreads, [this] { TryExecuteTaskWait(); });
}



template <typename _Func, typename... _Args>
void ThreadPool::StartThreads(U32 numThreads, _Func&& function, _Args&&... args)
{
    std::lock_guard<std::mutex> lock(mMutex);
    for (U32 i = 0; i < numThreads; ++i)
        mThreads.emplace_back(*this, std::bind(std::forward<_Func>(function), std::forward<_Args>(args)...));
}



void ThreadPool::CloseThreads(U32 numThreads)
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



void ThreadPool::CloseAllThreads()
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



bool ThreadPool::HasTasks() const
{
    return !mQueue.IsEmpty();
}



U32 ThreadPool::GetNumTasks() const
{
    return mQueue.GetSize();
}



void ThreadPool::TryExecuteTask()
{
    std::unique_ptr<TaskBase> task{nullptr};
    if (mQueue.tryPop(task))
    {
        task->execute();
    }
}



void ThreadPool::TryExecuteTaskWait()
{
    WaitForTask();
    TryExecuteTask();
}



void ThreadPool::WaitForTask()
{
    std::unique_lock<std::mutex> lock(mMutexCondition);
    mConditionThreads.wait(lock, [this] { return !mQueue.IsEmpty() || mCloseThreads; });
}



template <typename _F, typename... _Args>
void ThreadPool::Submit(_F&& function, _Args&&... args)
{
    // static assertion
    using ResultType =
            std::result_of_t<decltype(std::bind(std::forward<_F>(function), std::forward<_Args>(args)...))()>;
    using TaskType = Task<decltype(std::bind(std::forward<_F>(function), std::forward<_Args>(args)...))>;

    static_assert(std::is_same<void, ResultType>::value, "Used submit() with non void function!");
    mQueue.Push(std::make_unique<TaskType>(std::bind(std::forward<_F>(function), std::forward<_Args>(args)...)));
    mConditionThreads.notify_one();
}



void ThreadPool::ClearExceptionLog()
{
    std::lock_guard<std::mutex> lock(mMutex);
    mExceptionLog.clear();
}



void ThreadPool::PropagateExceptions() const
{
    std::lock_guard<std::mutex> lock(mMutex);
    if (!mExceptionLog.empty())
        throw Exception(__PRETTY_FUNCTION__, mExceptionLog);
}

} // namespace GDL
