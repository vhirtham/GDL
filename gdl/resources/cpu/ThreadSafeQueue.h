#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>


namespace GDL
{
template <typename T>
class ThreadSafeQueue
{
    std::atomic_bool mValid; //!< If set to FALSE, no more threads can pick tasks
    std::condition_variable mCondition; //! Needed to activate threads waiting to pick up a task
    mutable std::mutex mMutex; //!< Mutex to protect internal data from data races
    std::queue<T> mQueue; //!< Queue filled with tasks


public:
    //! @brief Constructor
    ThreadSafeQueue();

    //! @brief Copy constructor
    ThreadSafeQueue(const ThreadSafeQueue& other) = delete;

    //! @brief Move constructor
    ThreadSafeQueue(ThreadSafeQueue&& other) = delete;

    //! @brief Copy assignment operator
    ThreadSafeQueue& operator=(const ThreadSafeQueue& other) = delete;

    //! @brief Move assignment operator
    ThreadSafeQueue& operator=(ThreadSafeQueue&& other) = delete;

    //! @brief Destructor
    ~ThreadSafeQueue();



    //! @brief Tries to get the next value of the queue
    //! @param out: reference to a variable that stores fetched value
    //! @return TRUE if the operation was successful, FALSE if not
    bool tryPop(T& out);


    //! @brief Waits until the queue contains values to fetch and then tries to get the next one
    //! @param out: reference to a variable that stores fetched value
    //! @return TRUE if the operation was successful, FALSE if not
    bool tryWaitPop(T& out);


    //! @brief Pushes a new value into the queue and informs a waiting thread
    //! @param value New value
    void push(T value);


    //! @brief Returns if the queue is empty or not
    //! @return TRUE if the queue is empty, FALSE if not
    bool empty() const;


    //! @brief Deletes all elements and informs all waiting threads
    void clear();


    //! @brief Invalidates the queue and informs all threads
    //! @remark This is used to free all waiting threads, when the queue should not be used any more
    void invalidate();


    //! @brief Returns if the queue is valid or not
    //! @return TRUE if the queue is valid, FALSE if not
    bool isValid();
};


} // namespace GDL
