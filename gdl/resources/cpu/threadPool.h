#pragma once

#include "gdl/GDLTypedefs.h"
//#include "gdl/resources/cpu/thread.h"
#include "gdl/resources/cpu/task.h"
#include "gdl/resources/cpu/taskBase.h"
#include "gdl/resources/cpu/threadPoolQueue.h"

#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>

#include <iostream>

namespace GDL
{

//! @brief Class that manages multiple worker threads.
//! @remark Worker thread exceptions are caught and the messages are stored in an exception message buffer that can be
//! checked. This pool does not use futures to avoid an additional source of dedlocks. If you need to wait for results
//! use self submitting (when result is not ready) proceed funtions.
class ThreadPoolNEW
{
    class ThreadNEW
    {

        std::atomic_bool mClose;
        std::atomic_bool mFinished = false;
        ThreadPoolNEW& mThreadPool;
        std::thread mThread; // <--- Always last member (initialization problems may occur if not)

    public:
        ThreadNEW() = delete;
        ThreadNEW(const ThreadNEW&) = delete;
        ThreadNEW(ThreadNEW&&) = delete;
        ThreadNEW& operator=(const ThreadNEW&) = delete;
        ThreadNEW& operator=(ThreadNEW&&) = delete;
        ~ThreadNEW();

        template <typename _Func>
        ThreadNEW(ThreadPoolNEW& threadPool, _Func&& function);

        void Close();
        bool HasFinished() const;

        template <typename _Func>
        void Run(_Func&& function);
    };



    std::atomic<bool> mCloseThreads = false;
    mutable std::mutex mMutex;
    std::mutex mMutexCondition;
    std::condition_variable mConditionThreads;

    std::string mExceptionLog;

    std::deque<ThreadNEW> mThreads;
    ThreadPoolQueue<std::unique_ptr<TaskBase>> mQueue;


public:
    ThreadPoolNEW() = delete;
    ThreadPoolNEW(const ThreadPoolNEW&) = delete;
    ThreadPoolNEW(ThreadPoolNEW&&) = delete;
    ThreadPoolNEW& operator=(const ThreadPoolNEW&) = delete;
    ThreadPoolNEW& operator=(ThreadPoolNEW&&) = delete;
    ~ThreadPoolNEW();

    //! @brief Constructs the thread pool and starts the passed number of threads
    //! @param numThreads: Number of threads that should be started
    explicit ThreadPoolNEW(const U32 numThreads);

    //! @brief Deinitializes the thread pool
    void Deinitialize();

    //! @brief Initializes the thread pool
    void Initialize();

    //! @brief Gets the current number of threads
    //! @return Current number of threads
    U32 GetNumThreads() const;

    //! @brief Starts a specified number of thread
    //! @param numThreads: Number of threads that should be started
    void StartThreads(U32 numThreads);

    //! @brief Closes a specified number of thread
    //! @param numThreads: Number of threads that should be closed
    void CloseThreads(U32 numThreads);

    //! @brief Closes all threads
    void CloseAllThreads();

    //! @brief Returns true if the thread pool has tasks in its queue
    //! @return true/false
    bool HasTasks() const;

    //! @brief Gets the number of enqued tasks
    //! @return Number of enqued tasks
    U32 GetNumTasks() const;

    //! @brief Tries to fetch and execute a task from the thread pools queue
    void TryExecuteTask();

    //! @brief Tries to fetch and execute a task from the thread pools queue. If no tasks are available, the current
    //! thread waits until new tasks are submitted
    void TryExecuteTaskWait();

    //! @brief Submits a task to the thread pool
    //! @tparam _F: Function or functor type
    //! @tparam _Args: Parameter pack of the functions argument types
    //! @param function: Function that should be executed
    //! @param args: Function arguments
    template <typename _F, typename... _Args>
    void Submit(_F&& function, _Args&&... args);



    //! @brief Clears the exception log
    void ClearExceptionLog();

    //! @brief Checks if a worker thread has caught an exception and rethrows it.
    void PropagateExceptions() const;



private:
    //! @brief Lets the current thread wait until there are tasks in the queue
    void WaitForTask();
};

template <typename _Func>
ThreadPoolNEW::ThreadNEW::ThreadNEW(ThreadPoolNEW& threadPool, _Func&& function)
    : mClose{false}
    , mFinished{false}
    , mThreadPool(threadPool)
    , mThread(&ThreadNEW::Run<_Func>, this, function)
{
}


template <typename _Func>
void ThreadPoolNEW::ThreadNEW::Run(_Func&& function)
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


template <typename _F, typename... _Args>
void ThreadPoolNEW::Submit(_F&& function, _Args&&... args)
{
    // static assertion
    using ResultType =
            std::result_of_t<decltype(std::bind(std::forward<_F>(function), std::forward<_Args>(args)...))()>;
    using TaskType = Task<decltype(std::bind(std::forward<_F>(function), std::forward<_Args>(args)...))>;

    static_assert(std::is_same<void, ResultType>::value, "Used submit() with non void function!");
    mQueue.Push(std::make_unique<TaskType>(std::bind(std::forward<_F>(function), std::forward<_Args>(args)...)));
    mConditionThreads.notify_one();
}
}
