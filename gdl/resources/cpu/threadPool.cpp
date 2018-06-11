#include "gdl/resources/cpu/threadPool.h"


#include "gdl/base/Exception.h"

#include <cassert>

namespace GDL
{


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



bool ThreadPool::HasTasks() const
{
    return !mQueue.IsEmpty();
}

U32 ThreadPool::GetNumTasks() const
{
    return mQueue.GetSize();
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

void ThreadPool::WaitForTask()
{
    std::unique_lock<std::mutex> lock(mMutexCondition);
    mConditionThreads.wait(lock, [this] { return !mQueue.IsEmpty() || mCloseThreads; });
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



} // namespace GDL
