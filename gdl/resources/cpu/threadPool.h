#pragma once

#include "gdl/GDLTypedefs.h"
#include "gdl/resources/cpu/task.h"
#include "gdl/resources/cpu/taskBase.h"
#include "gdl/resources/cpu/threadPoolQueue.h"

#include <array>
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
template <int _NumQueues = 1>
class ThreadPool
{
    static_assert(_NumQueues > 0, "The threadpool needs at least 1 queue");

    class Thread
    {

        std::atomic_bool mClose;
        std::atomic_bool mFinished = false;
        ThreadPool& mThreadPool;
        std::thread mThread; // <--- Always last member (initialization problems may occur if not)

    public:
        Thread() = delete;
        Thread(const Thread&) = delete;
        Thread(Thread&&) = delete;
        Thread& operator=(const Thread&) = delete;
        Thread& operator=(Thread&&) = delete;
        ~Thread();

        //! @brief Constructor that takes a function that should be run in a while loop until the tread is closed
        //! @tparam _Func: Function type
        //! @param threadPool: The threads thread pool
        //! @param function: Function that should be run
        template <typename _Func>
        Thread(ThreadPool& threadPool, _Func&& function);

        //! @brief Stops the threads while loop
        void Close();

        //! @brief Returns if the thread has left its while loop
        //! @return True/false
        bool HasFinished() const;

        //! @param The threads main function which runs until Close() is called or the thread pool is closed. All
        //! exceptions are caught and written to the thread pools exception log
        //! @tparam _Func: Type of the function which is executed in the threads main loop
        //! @param function: Function which is executed in the threads main loop
        template <typename _Func>
        void Run(_Func&& function);
    };



    std::atomic<bool> mCloseThreads = false;
    mutable std::mutex mMutex;
    std::mutex mMutexCondition;
    std::condition_variable mConditionThreads;

    std::string mExceptionLog;

    std::deque<Thread> mThreads;
    std::array<ThreadPoolQueue<std::unique_ptr<TaskBase>>, _NumQueues> mQueue;


public:
    ThreadPool() = delete;
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;
    ~ThreadPool();

    //! @brief Constructs the thread pool and starts the passed number of threads
    //! @param numThreads: Number of threads that should be started
    explicit ThreadPool(const U32 numThreads);

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

    //! @brief Starts a specified number of thread with a alternative main loop function
    //! @tparam _Func: Alternative function type
    //! @tparam _Args: Function arguments types
    //! @param numThreads: Number of threads that should be started
    //! @param function: Alternative main loop function
    //! @param args: Arguments that should be passed to the provided function
    template <typename _Func, typename... _Args>
    void StartThreads(U32 numThreads, _Func&& function, _Args&&... args);

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

    //! @brief Submits a task to main queue ([0]) of the thread pool
    //! @tparam _F: Function or functor type
    //! @tparam _Args: Parameter pack of the functions argument types
    //! @param function: Function that should be executed
    //! @param args: Function arguments
    template <typename _F, typename... _Args>
    void Submit(_F&& function, _Args&&... args);

    //! @brief Submits a task to a certain queue of the thread pool
    //! @tparam _F: Function or functor type
    //! @tparam _Args: Parameter pack of the functions argument types
    //! @param queNum: Number of the queue that should store the task
    //! @param function: Function that should be executed
    //! @param args: Function arguments
    template <typename _F, typename... _Args>
    void Submit(I32 queueNum, _F&& function, _Args&&... args);



    //! @brief Clears the exception log
    void ClearExceptionLog();

    //! @brief Checks if a worker thread has caught an exception and rethrows it.
    void PropagateExceptions() const;



private:
    //! @brief Lets the current thread wait until there are tasks in the queue
    void WaitForTask();
};
}

#include "gdl/resources/cpu/threadPool.inl"
