#pragma once

#include <atomic>
//#include <condition_variable>
#include <mutex>
#include <queue>


namespace GDL
{
template <typename T>
class ThreadPoolQueue
{
    std::atomic_bool mValid; //!< If set to FALSE, no more threads can pick tasks
    mutable std::mutex mMutex; //!< Mutex to protect internal data from data races
    //    std::condition_variable mCondition; //! Needed to activate threads waiting to pick up a task
    std::queue<T> mQueue; //!< Queue filled with tasks


public:
    ThreadPoolQueue();
    ThreadPoolQueue(const ThreadPoolQueue& other) = delete;
    ThreadPoolQueue(ThreadPoolQueue&& other) = delete;
    ThreadPoolQueue& operator=(const ThreadPoolQueue& other) = delete;
    ThreadPoolQueue& operator=(ThreadPoolQueue&& other) = delete;
    ~ThreadPoolQueue();


    void Deinitialize();

    //! @brief Pushes a new value into the queue and informs a waiting thread
    //! @param value New value
    void Push(T value);


    //! @brief Tries to get the next value of the queue
    //! @param out: reference to a variable that stores fetched value
    //! @return TRUE if the operation was successful, FALSE if not
    bool tryPop(T& out);


    //    //! @brief Waits until the queue contains values to fetch and then tries to get the next one
    //    //! @param out: reference to a variable that stores fetched value
    //    //! @return TRUE if the operation was successful, FALSE if not
    //    bool tryWaitPop(T& out);



    //! @brief Returns if the queue is empty or not
    //! @return TRUE if the queue is empty, FALSE if not
    bool IsEmpty() const;


    //    //! @brief Deletes all elements and informs all waiting threads
    //    void clear();


    //    //! @brief Invalidates the queue and informs all threads
    //    //! @remark This is used to free all waiting threads, when the queue should not be used any more
    //    void invalidate();


    //    //! @brief Returns if the queue is valid or not
    //    //! @return TRUE if the queue is valid, FALSE if not
    //    bool isValid();
};
}
