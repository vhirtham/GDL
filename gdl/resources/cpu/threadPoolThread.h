#pragma once

#include <gdl/base/fundamentalTypes.h>

#include <atomic>
#include <thread>

namespace GDL
{

template <I32>
class ThreadPool;

//! @brief Class for worker threads of the thread pool
//! @tparam _numThreadPoolQueues: Number of queues used by the thread pool. Necessary since there is no common base
//! class of all thread pools and this class needs to store a reference to the thread pool
template <I32 _numThreadPoolQueues>
class ThreadPoolThread
{

    std::atomic_bool mClose;
    ThreadPool<_numThreadPoolQueues>& mThreadPool;
    std::thread mThread; // <--- Always last member (initialization problems may occur if not)

public:
    ThreadPoolThread() = delete;
    ThreadPoolThread(const ThreadPoolThread&) = delete;
    ThreadPoolThread(ThreadPoolThread&&) = delete;
    ThreadPoolThread& operator=(const ThreadPoolThread&) = delete;
    ThreadPoolThread& operator=(ThreadPoolThread&&) = delete;
    ~ThreadPoolThread();

    //! @brief Constructor that takes a function that should be run in a while loop until the tread is closed
    //! @tparam _function: Function type
    //! @tparam _initFunction: Type of the initialization function
    //! @tparam _deinitFunction: Type of the deinitialization function
    //! @param threadPool: The threads thread pool
    //! @param function: Function that should be run
    //! @param initFunction: Initialization function
    //! @param deinitFunction: Deinitialization function
    template <typename _function, typename _initFunction, typename _deinitFunction>
    ThreadPoolThread(ThreadPool<_numThreadPoolQueues>& threadPool, _function&& function, _initFunction&& initFunction,
                     _deinitFunction&& deinitFunction);

    //! @brief Stops the threads while loop
    void Close();

    //! @param The threads main function which runs until Close() is called or the thread pool is closed. All
    //! exceptions are caught and written to the thread pools exception log
    //! @tparam _function: Type of the function which is executed in the threads main loop
    //! @param function: Function which is executed in the threads main loop
    template <typename _function, typename _initFunction, typename _deinitFunction>
    void Run(_function&& function, _initFunction&& initFunction, _deinitFunction&& deinitFunction);

private:
    //! @brief Wraps a try catch block around the passed function and handles occurring exceptions
    //! @tparam _function: Type of the passed function
    //! @param function: Function that needs exception handling
    template <typename _function>
    inline void HandleExceptions(_function&& function);
};
} // namespace GDL

#include "gdl/resources/cpu/threadPoolThread.inl"
