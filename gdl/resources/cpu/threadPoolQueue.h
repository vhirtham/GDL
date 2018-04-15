#pragma once

#include "gdl/GDLTypedefs.h"

#include <atomic>
#include <mutex>
#include <queue>


namespace GDL
{
template <typename T>
class ThreadPoolQueue
{
    mutable std::mutex mMutex; //!< Mutex to protect internal data from data races
    std::queue<T> mQueue; //!< Queue filled with tasks


public:
    ThreadPoolQueue();
    ThreadPoolQueue(const ThreadPoolQueue& other) = delete;
    ThreadPoolQueue(ThreadPoolQueue&& other) = delete;
    ThreadPoolQueue& operator=(const ThreadPoolQueue& other) = delete;
    ThreadPoolQueue& operator=(ThreadPoolQueue&& other) = delete;
    ~ThreadPoolQueue();



    //! @brief Pushes a new value into the queue and informs a waiting thread
    //! @param value New value
    void Push(T value);


    //! @brief Tries to get the next value of the queue
    //! @param out: reference to a variable that stores fetched value
    //! @return TRUE if the operation was successful, FALSE if not
    bool tryPop(T& out);


    //! @brief Returns if the queue is empty or not
    //! @return TRUE if the queue is empty, FALSE if not
    bool IsEmpty() const;

    //! @brief Gets the size of the queue
    //! @return Size
    U32 GetSize() const;
};
}
