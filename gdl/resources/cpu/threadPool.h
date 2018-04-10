#pragma once

#include "gdl/GDLTypedefs.h"
#include "gdl/resources/cpu/thread.h"
#include "gdl/resources/cpu/task.h"
#include "gdl/resources/cpu/taskBase.h"
#include "gdl/resources/cpu/threadPoolQueue.h"

#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>

#include <iostream>

namespace GDL
{

class ThreadPoolNEW
{
    friend class ThreadNEW;


    std::atomic<bool> mClose = false;
    mutable std::mutex mMutex;
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

    //! @brief Initializes the thread pool and starts the passed number of threads
    //! @param numThreads: Number of threads that should be started
    void Initialize(const U32 numThreads);

    //! @brief Tries to fetch and execute a task from the thread pools queue
    void TryExecuteTask();

    //! @brief Tries to fetch and execute a task from the thread pools queue. If no tasks are available, the current
    //! thread waits until new tasks are submitted
    void TryExecuteTaskWait();

    //! @brief Returns true if the thread pool has tasks in its queue
    //! @return true/false
    bool HasTasks() const;


    //! @brief Submits a task to the thread pool
    //! @tparam _F: Function or functor type
    //! @tparam _Args: Parameter pack of the functions argument types
    //! @param function: Function that should be executed
    //! @param args: Function arguments
    template <typename _F, typename... _Args>
    void Submit(_F&& function, _Args&&... args);


    void ClearExceptionLog();
    void CheckExceptions();

private:
    //! @brief Lets the current thread wait until there are tasks in the queue
    void WaitForTask();
};



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
