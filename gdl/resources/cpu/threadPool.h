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
//! checked. This pool does not use futures to avoid an additional source of deadlocks. If you need to wait for results,
//! resubmit the function until the necessary value is ready.
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
    ThreadPool() = default;
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;
    ~ThreadPool();

    //! @brief Constructs the thread pool and starts the passed number of threads which try to fetch tasks from the
    //! queue
    //! @param numThreads: Number of threads that should be started
    //! @remark This function can only be used by thread pools with a single queue.
    explicit ThreadPool(U32 numThreads);

    //! @brief Deinitializes the thread pool
    void Deinitialize();

    //! @brief Gets the current number of threads
    //! @return Current number of threads
    U32 GetNumThreads() const;

    //! @brief Starts a specified number of threads that try to fetch tasks from the queue
    //! @param numThreads: Number of threads that should be started
    //! @remark This function can only be used by thread pools with a single queue.
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

    //! @brief Returns true if the thread pool has tasks in the specified queue
    //! @param queueNum: Array number of the queue
    //! @return true/false
    bool HasTasks(const I32 queueNum) const;

    //! @brief Returns true if the thread pool has tasks
    //! @return true/false
    //! @remark This function can only be used by thread pools with a single queue.
    bool HasTasks() const;

    //! @brief Gets the number of tasks in the specified queue
    //! @param queueNum: Array number of the queue
    //! @return Number of tasks in the queue
    U32 GetNumTasks(const I32 queueNum) const;

    //! @brief Gets the number of tasks in the queue
    //! @return Number of tasks in the queue
    //! @remark This function can only be used by thread pools with a single queue.
    U64 GetNumTasks() const;

    //! @brief Tries to fetch and execute a task from a specific queue
    //! @param queueNum: Array number of the queue
    //! @return TRUE if task was executed, FALSe if not
    bool TryExecuteTask(const I32 queueNum);

    //! @brief Tries to fetch and execute a task from the queue
    //! @return TRUE if task was executed, FALSe if not
    //! @remark This function can only be used by thread pools with a single queue.
    bool TryExecuteTask();

    //! @brief Submits a task to a certain queue of the thread pool
    //! @tparam _function: Function or functor type
    //! @tparam _args: Parameter pack of the functions argument types
    //! @param queueNum: Array number of the queue that should store the task
    //! @param function: Function that should be executed
    //! @param args: Function arguments
    template <typename _function, typename... _args>
    void Submit(const I32 queueNum, _function&& function, _args&&... args);

    //! @brief Submits a task to the queue of the thread pool
    //! @tparam _function: Function or functor type
    //! @tparam _args: Parameter pack of the functions argument types
    //! @param function: Function that should be executed
    //! @param args: Function arguments
    //! @remark This function can only be used by thread pools with a single queue.
    template <typename _function, typename... _args>
    void Submit(_function&& function, _args&&... args);

    //! @brief Clears the exception log
    void ClearExceptionLog();

    //! @brief Gets the size of the exception log
    //! @return Size of the exception log
    U32 ExceptionLogSize() const;

    //! @brief Checks if a worker thread has caught an exception and rethrows it.
    void PropagateExceptions() const;

private:
    //! @brief Adds a new message to the exception log
    //! @tparam _args: Parameter pack for variable number of parameters of different types
    //! @param args: The new message is constructed from all the passed arguments.
    template <typename... _args>
    void AddMessageToExceptionLog(const _args&... args);
};
} // namespace GDL

#include "gdl/resources/cpu/threadPoolThread.h"
#include "gdl/resources/cpu/threadPool.inl"
