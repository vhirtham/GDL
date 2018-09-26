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
    std::atomic_bool mFinished = false;
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
    //! @tparam _Func: Function type
    //! @tparam _InitFunction: Type of the initialization function
    //! @tparam _DeinitFunction: Type of the deinitialization function
    //! @param threadPool: The threads thread pool
    //! @param function: Function that should be run
    //! @param initFunction: Initialization function
    //! @param deinitFunction: Deinitialization function
    template <typename _Func, typename _InitFunction, typename _DeinitFunction>
    ThreadPoolThread(ThreadPool<_numThreadPoolQueues>& threadPool, _Func&& function, _InitFunction&& initFunction,
                     _DeinitFunction&& deinitFunction);

    //! @brief Stops the threads while loop
    void Close();

    //! @brief Returns if the thread has left its while loop
    //! @return True/false
    bool HasFinished() const;

    //! @param The threads main function which runs until Close() is called or the thread pool is closed. All
    //! exceptions are caught and written to the thread pools exception log
    //! @tparam _Func: Type of the function which is executed in the threads main loop
    //! @param function: Function which is executed in the threads main loop
    template <typename _Func, typename _InitFunction, typename _DeinitFunction>
    void Run(_Func&& function, _InitFunction&& initFunction, _DeinitFunction&& deinitFunction);

private:
    template <typename _Func>
    void HandleExceptions(_Func&& function);
};
} // namespace GDL

#include "gdl/resources/cpu/threadPoolThread.inl"
