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
    std::atomic_bool mValid = true;
    std::condition_variable mCondition;
    mutable std::mutex mMutex; //!< Mutex
    std::queue<T> mQueue; //!< data container


public:
    //! @brief Constructor
    ThreadSafeQueue() = default;

    //! @brief Copy constructor
    ThreadSafeQueue(const ThreadSafeQueue& other) = delete;

    //! @brief Move constructor
    ThreadSafeQueue(ThreadSafeQueue&& other) = delete;

    //! @brief Copy assignment operator
    ThreadSafeQueue& operator=(const ThreadSafeQueue& other) = delete;

    //! @brief Move assignment operator
    ThreadSafeQueue& operator=(ThreadSafeQueue&& other) = delete;

    //! @brief Destructor
    ~ThreadSafeQueue()
    {
        invalidate();
    }



    //! @brief Tries to get the next value of the queue
    //! @param out: reference to a variable that stores fetched value
    //! @return TRUE if the operation was successful, FALSE if not
    bool tryPop(T& out)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        if (mQueue.empty() || !mValid)
        {
            return false;
        }
        out = std::move(mQueue.front());
        mQueue.pop();
        return true;
    }



    //! @brief Waits until the queue contains values to fetch and then tries to get the next one
    //! @param out: reference to a variable that stores fetched value
    //! @return TRUE if the operation was successful, FALSE if not
    bool tryWaitPop(T& out)
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mCondition.wait(lock, [this]() { return !mQueue.empty() || !mValid; });
        if (!mValid)
        {
            return false;
        }
        out = std::move(mQueue.front());
        mQueue.pop();
        return true;
    }



    //! @brief Pushes a new value into the queue and informs a waiting thread
    //! @param value New value
    void push(T value)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mQueue.push(std::move(value));
        mCondition.notify_one();
    }



    //! @brief Returns if the queue is empty or not
    //! @return TRUE if the queue is empty, FALSE if not
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mMutex);
        return mQueue.empty();
    }



    //! @brief Deletes all elements and informs all waiting threads
    void clear()
    {
        std::lock_guard<std::mutex> lock(mMutex);
        while (!mQueue.empty())
            mQueue.pop();
        mCondition.notify_all();
    }

    //! @brief Invalidates the queue and informs all threads
    //! @remark This is used to free all waiting threads, when the queue should not be used any more
    void invalidate()
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mValid = false;
        mCondition.notify_all();
    }

    //! @brief Returns if the queue is valid or not
    //! @return TRUE if the queue is valid, FALSE if not
    bool isValid()
    {
        std::lock_guard<std::mutex> lock(mMutex);
        return mValid;
    }
};


} // namespace GDL
