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

    using QueueArray = std::array<ThreadPoolQueue<std::unique_ptr<TaskBase>>, _NumQueues>;

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



    // Member Variables --------------------------------------------------

    //!@brief As long as this variable is true, threads won't wait for empty queues to be filled. This avoids deadlocks
    //! due to waiting for threads to close which are currently waiting for tasks.
    std::atomic<bool> mCloseThreads = false;

    std::condition_variable mConditionThreads;
    std::mutex mMutexCondition;

    mutable std::mutex mMutex;
    std::string mExceptionLog;

    std::deque<Thread> mThreads;
    QueueArray mQueue;


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
    //! @param numThreads: Number of threads that should be started
    //! @param function: Alternative main loop function
    //! @remark The function signature should always be void()
    template <typename _Func>
    void StartThreads(U32 numThreads, _Func&& function);

    //! @brief Closes a specified number of thread
    //! @param numThreads: Number of threads that should be closed
    void CloseThreads(U32 numThreads);

    //! @brief Closes all threads
    void CloseAllThreads();

    //! @brief Returns true if the thread pool has tasks in one or more queues
    //! @return true/false
    bool HasTasks() const;

    //! @brief Returns true if the thread pool has tasks in the specified queue
    //! @param queueNum: Array number of the queue
    //! @return true/false
    bool HasTasks(I32 queueNum) const;

    //! @brief Gets the total number of tasks in all queues
    //! @return Number of tasks in all queues
    U32 GetNumTasks() const;

    //! @brief Gets the number of tasks in the specified queue
    //! @param queueNum: Array number of the queue
    //! @return Number of tasks in the queue
    U32 GetNumTasks(I32 queueNum) const;

    //! @brief Tries to fetch and execute a task from one of the thread pools queue
    void TryExecuteTask();

    //! @brief Tries to fetch and execute a task from a specific queue
    //! @param queueNum: Array number of the queue
    void TryExecuteTask(I32 queueNum);

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
    //! @param queueNum: Array number of the queue that should store the task
    //! @param function: Function that should be executed
    //! @param args: Function arguments
    template <typename _F, typename... _Args>
    void SubmitToQueue(I32 queueNum, _F&& function, _Args&&... args);



    //! @brief Clears the exception log
    void ClearExceptionLog();

    //! @brief Gets the size of the exception log
    //! @return Size of the exception log
    U32 ExceptionLogSize() const;

    //! @brief Checks if a worker thread has caught an exception and rethrows it.
    void PropagateExceptions() const;



private:
    //! @brief Lets the current thread wait until there are tasks in the queue
    void WaitForTask();
};
}

#include "gdl/resources/cpu/threadPool.inl"
