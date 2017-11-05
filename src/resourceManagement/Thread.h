#pragma once

#include <atomic>
#include <thread>


namespace GDL
{
class ThreadPool;

//! @brief Class that manages a single std::thread of a thread pool.
class Thread
{
    friend class ThreadPool;


    //! @brief If TRUE, the treads endless loop funtion is left
    std::atomic_bool mClose;

    //! @brief Reference to parent Threadpool
    ThreadPool& mThreadPool;

    //! @brief //!< The managed Thread of the class
    //! @remark Never move this over the thread pool reference. Otherwise segmentation faults during initialization can
    //! occur. (See commit badf19d55b40ea924b0538d54eda421c57aa8ae8)
    std::thread mThread;


public:
    //! @brief Constructor
    Thread() = delete;

    //! @brief Copy constructor
    //! @param other: Object that should be copied
    //! @remark This class should not be copyable since each copy spawns another thread, which might cause problems. For
    //! example if a std::vector<Thread> needs to reallocate on a push_back, it copies all members to the new memory,
    //! effectively doubling the number of threads.
    Thread(const Thread& other) = delete;

    //! @brief Move constructor
    //! @param other: Object that should be moved
    //! @remark This class should not be moveable because the atomic_bool mClose can not be moved, only copied. If we
    //! move a std::thread from A to B, the run() function of the thread will still depend on mClose of A, which will be
    //! set to true during the destruction of A. Therefore the thread will stop working after the move.
    Thread(Thread&& other) = delete;

    //! @brief Copy assignment operator
    //! @param other: Object that should be copied
    Thread& operator=(const Thread& other) = delete;

    //! @brief Move assignment operator
    //! @param other: Object that should be moved
    Thread& operator=(Thread&& other) = delete;

    //! @brief Destructor
    ~Thread();


    //! @brief Constructor
    //! @param threadPool: Reference to the parent thread pool
    //! @remark At the current status it would be enough to pass only the queue where the thread should pick its tasks
    //! from. However, if it might be necessary to have multiple queues for physics, rendering, loading of assets etc.
    //! Therefore the thread should not be bound to a single queue
    Thread(ThreadPool& threadPool);

private:
    //! @brief Gives the run-function the signal to leave the endless-loop and detaches the thread.
    //! @remark This is needed for dynamic reduction of the number of threads. Joining a thread while the program is
    //! supposed to continue might lead into a deadlock if the main loop of the deinitialized thread currently waits
    //! for another job.
    void deinitializeDetach();

    //! @brief Gives the run-function the signal to leave the endless-loop and joins the thread.
    void deinitializeJoin();

    //! @brief Function that is passed to the managed thread after construction. It starts the endless-loop to pick up
    //! tasks.
    void run();
};



} // namespace GDL
