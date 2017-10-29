#pragma once

#include "src/GDLTypedefs.h"
#include "src/resourceManagement/TaskFuture.h"
#include "src/resourceManagement/ThreadSafeQueue.h"
#include "src/resourceManagement/ThreadTask.h"

#include <atomic>
#include <thread>
#include <utility>
#include <vector>


namespace GDL
{
//! @brief Threadpool class manages a specified number of threads.
//! @remark Original implemention and documentation can be found here:
//! http://roar11.com/2016/01/a-platform-independent-thread-pool-using-c14/
class ThreadPool
{

    std::atomic_bool mClose; //!< If TRUE all threads leave their endless loop after finishing their current task
    ThreadSafeQueue<std::unique_ptr<ThreadTaskBase>> mWorkQueue; //!< Queue of tasks
    std::vector<std::thread> mThreads; //!< vector of all managed threads


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

private:
    //! @brief Deinitializes the threadpool
    void deinitialize();


    //! @brief Worker function that are used by the threads. This is probably exchanged by a thread class
    void worker();
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
