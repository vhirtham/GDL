#pragma once

#include "gdl/base/container/deque.h"
#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/string.h"
#include "gdl/base/uniquePtr.h"
#include "gdl/resources/cpu/task.h"
#include "gdl/resources/cpu/threadPoolQueue.h"


#include <array>
#include <mutex>
#include <thread>


namespace GDL
{

template <I32>
class ThreadPoolThread;

//! @brief Class that manages multiple worker threads.
//! @remark Worker thread exceptions are caught and the messages are stored in an exception message buffer that can be
//! checked. This pool does not use futures to avoid an additional source of dedlocks. If you need to wait for results
//! use self submitting (when result is not ready) proceed funtions.
template <I32 _numQueues = 1>
class ThreadPool
{
    static_assert(_numQueues > 0, "The threadpool needs at least 1 queue");

    template <I32>
    friend class ThreadPoolThread;

    using QueueArray = std::array<ThreadPoolQueue<UniquePtr<TaskBase>>, _numQueues>;


    mutable std::mutex mMutexThreads;
    mutable std::mutex mMutexExceptionLog;
    String mExceptionLog;
    Deque<ThreadPoolThread<_numQueues>> mThreads;
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
    explicit ThreadPool(U32 numThreads);

    //! @brief Deinitializes the thread pool
    void Deinitialize();

    //! @brief Gets the current number of threads
    //! @return Current number of threads
    U32 GetNumThreads() const;

    //! @brief Starts a specified number of thread
    //! @param numThreads: Number of threads that should be started
    void StartThreads(U32 numThreads);

    //! @brief Starts a specified number of thread with a alternative main loop function
    //! @tparam _function: Alternative main loop function type
    //! @param numThreads: Number of threads that should be started
    //! @param function: Alternative main loop function
    //! @remark The function signature should always be void()
    template <typename _function>
    void StartThreads(U32 numThreads, _function&& function);


    //! @brief Starts a specified number of thread with a alternative main loop function
    //! @tparam _function: Alternative main loop function type
    //! @tparam _initFunction: Type of the initialization function
    //! @tparam _deinitFunction: Type of the deinitialization function
    //! @param numThreads: Number of threads that should be started
    //! @param function: Alternative main loop function
    //! @param initFunction: Initialization function
    //! @param deinitFunction: Deinitialization function
    //! @remark The signature of all passed functions should always be void()
    template <typename _function, typename _initFunction, typename _deinitFunction>
    void StartThreads(U32 numThreads, _function&& function, _initFunction&& initFunction,
                      _deinitFunction&& deinitFunction);

    //! @brief Closes a specified number of thread
    //! @param numThreadsToClose: Number of threads that should be closed
    //! @remark Negative values close all threads
    void CloseThreads(I32 numThreadsToClose);

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
    //! @return TRUE if task was executed, FALSe if not
    bool TryExecuteTask();

    //! @brief Tries to fetch and execute a task from a specific queue
    //! @param queueNum: Array number of the queue
    //! @return TRUE if task was executed, FALSe if not
    bool TryExecuteTask(I32 queueNum);

    //! @brief Submits a task to a certain queue of the thread pool
    //! @tparam _function: Function or functor type
    //! @tparam _args: Parameter pack of the functions argument types
    //! @param queueNum: Array number of the queue that should store the task
    //! @param function: Function that should be executed
    //! @param args: Function arguments
    template <typename _function, typename... _args>
    void Submit(I32 queueNum, _function&& function, _args&&... args);

    //! @brief Submits a task to main queue ([0]) of the thread pool
    //! @tparam _function: Function or functor type
    //! @tparam _args: Parameter pack of the functions argument types
    //! @param function: Function that should be executed
    //! @param args: Function arguments
    template <typename _function, typename... _args>
    void Submit(_function&& function, _args&&... args);

    //! @brief Clears the exception log
    void ClearExceptionLog();

    //! @brief Gets the size of the exception log
    //! @return Size of the exception log
    U32 ExceptionLogSize() const;

    //! @brief Checks if a worker thread has caught an exception and rethrows it.
    void PropagateExceptions() const;
};
}

#include "gdl/resources/cpu/threadPoolThread.h"
#include "gdl/resources/cpu/threadPool.inl"
