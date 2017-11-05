#pragma once

#include "src/GDLTypedefs.h"
#include "src/resourceManagement/TaskFuture.h"
#include "src/resourceManagement/ThreadSafeQueue.h"
#include "src/resourceManagement/ThreadTask.h"


#include <atomic>
#include <deque>
#include <mutex>
#include <utility>


namespace GDL
{

class Thread;

//! @brief Threadpool class manages a specified number of threads.
//! @remark Original implemention and documentation can be found here:
//! http://roar11.com/2016/01/a-platform-independent-thread-pool-using-c14/
class ThreadPool
{
    friend class Thread;

    //! @brief If set to true, all threads leave their main loop. (Implementation in Thread class)
    std::atomic_bool mClose;

#ifndef NDEBUG
    //! @brief Keeps track of how many threads started by the thread pool are still running their main loop.
    //! @remark This is only used for tests and debugging.
    std::atomic_uint mRunningThreads;
#endif

    //! @brief  Queue of tasks
    ThreadSafeQueue<std::unique_ptr<ThreadTaskBase>> mWorkQueue;

    //! @brief Protects the thread container from data races on multiple add and kill requests
    mutable std::mutex mMutexThreads;

    //! @brief Container for managed threads
    //! @remark: Since the Thread class is not copyable and
    //! also not moveable (see Thread docomentation for reason) a vector can not be used. A deque is one possible option
    //! that does not a copy ore move constructor of its containing object.
    std::deque<Thread> mThreads;



public:
    //! @brief Constructor
    ThreadPool() = delete;

    //! @brief Copy constructor
    //! @param other: Object that should be copied
    ThreadPool(const ThreadPool& other) = delete;

    //! @brief Move constructor
    //! @param other: Object that should be moved
    ThreadPool(ThreadPool&& other) = delete;

    //! @brief Copy assignment operator
    //! @param other: Object that should be copied
    ThreadPool& operator=(const ThreadPool& other) = delete;

    //! @brief Move assignment operator
    //! @param other: Object that should be moved
    ThreadPool& operator=(ThreadPool&& other) = delete;

    //! @brief Destructor
    ~ThreadPool();

    //! @brief Constructor wich spawns the specified number of threads
    //! @param numThreads: Number of threads that should be spawned
    explicit ThreadPool(const U32 numThreads);


    //! @brief Submits a function of any signature and the arguments for the function call to the thread pool.
    //! After submission the function is wrapped to match the function signature used by the tasks e.g void F().
    //! The results are returned after the tasks execution
    //! @tparam F: Function signature
    //! @tparam Args: Argument list that matches the function signature
    //! @param func: Function that should be enqueued
    //! @param args: Arguments that should be passed to the function
    //! @return The value/object which is returned by the submitted function
    template <typename F, typename... Args>
    auto submit(F&& func, Args&&... args);

    //! @brief Adds more threads to the thread pool
    //! @param numThreads: Number of threads that should be added
    void addThreads(U32 numThreads);

    //! @brief Removes threads from the thread pool
    //! @param numThreads: Number of Threads that should be removed
    void killThreads(U32 numThreads);

    //! @brief Gets the current number of threads, that are managed by the thread pool
    U32 getNumThreads() const;

#ifndef NDEBUG
    //! @brief Gets the number of threads, spawned by the thread pool, that are still running their main loop
    //! @return Number of running threads
    U32 getNumRunningThreads() const;
#endif

private:
    //! @brief Deinitializes the thread pool
    void deinitialize();
};



template <typename F, typename... Args>
auto ThreadPool::submit(F&& func, Args&&... args)
{
    auto boundTask = std::bind(std::forward<F>(func), std::forward<Args>(args)...);
    using ResultType = std::result_of_t<decltype(boundTask)()>;
    using PackagedTask = std::packaged_task<ResultType()>;
    using TaskType = ThreadTask<PackagedTask>;

    PackagedTask task{std::move(boundTask)};
    TaskFuture<ResultType> result{task.get_future()};
    mWorkQueue.push(std::make_unique<TaskType>(std::move(task)));
    return result;
}


} // namespace GDL
