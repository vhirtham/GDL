#include "gdl/resources/cpu/threadPool.h"


#include "gdl/base/Exception.h"

#include <cassert>

namespace GDL
{


ThreadPoolNEW::~ThreadPoolNEW()
{
    Deinitialize();
}

ThreadPoolNEW::ThreadPoolNEW(const U32 numThreads)
{
    Initialize();
    StartThreads(numThreads);
}

void ThreadPoolNEW::Deinitialize()
{
    CloseAllThreads();
    PropagateExceptions();
}

void ThreadPoolNEW::Initialize()
{
    std::lock_guard<std::mutex> lock(mMutex);
    mExceptionLog.reserve(200);
}

U32 ThreadPoolNEW::GetNumThreads() const
{
    std::lock_guard<std::mutex> lock(mMutex);
    return mThreads.size();
}



void ThreadPoolNEW::StartThreads(U32 numThreads)
{
    std::lock_guard<std::mutex> lock(mMutex);
    for (U32 i = 0; i < numThreads; ++i)
        mThreads.emplace_back(*this);
}

void ThreadPoolNEW::CloseThreads(U32 numThreads)
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



void ThreadPoolNEW::CloseAllThreads()
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



void ThreadPoolNEW::TryExecuteTask()
{
    std::unique_ptr<TaskBase> task{nullptr};
    if (mQueue.tryPop(task))
    {
        task->execute();
    }
}



void ThreadPoolNEW::TryExecuteTaskWait()
{
    WaitForTask();
    TryExecuteTask();
}



bool ThreadPoolNEW::HasTasks() const
{
    return !mQueue.IsEmpty();
}

U32 ThreadPoolNEW::GetNumTasks() const
{
    return mQueue.GetSize();
}



void ThreadPoolNEW::ClearExceptionLog()
{
    std::lock_guard<std::mutex> lock(mMutex);
    mExceptionLog.clear();
}



void ThreadPoolNEW::PropagateExceptions() const
{
    std::lock_guard<std::mutex> lock(mMutex);
    if (!mExceptionLog.empty())
        throw Exception(__PRETTY_FUNCTION__, mExceptionLog);
}

void ThreadPoolNEW::WaitForTask()
{
    std::unique_lock<std::mutex> lock(mMutexCondition);
    mConditionThreads.wait(lock, [this] { return !mQueue.IsEmpty() || mCloseThreads; });
}

ThreadPoolNEW::ThreadNEW::~ThreadNEW()
{
    mThread.join();
}

ThreadPoolNEW::ThreadNEW::ThreadNEW(ThreadPoolNEW& threadPool)
    : mClose{false}
    , mFinished{false}
    , mThreadPool(threadPool)
    , mThread(&ThreadNEW::Run, this)
{
}

void ThreadPoolNEW::ThreadNEW::Close()
{
    mClose = true;
}

bool ThreadPoolNEW::ThreadNEW::HasFinished() const
{
    return mFinished;
}

void ThreadPoolNEW::ThreadNEW::Run()
{
    while (!mClose && !mThreadPool.mCloseThreads)
    {
        try
        {
            mThreadPool.TryExecuteTaskWait();
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



} // namespace GDL
