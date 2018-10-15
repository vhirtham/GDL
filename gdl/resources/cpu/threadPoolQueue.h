#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/container/queue.h"
#include "gdl/resources/cpu/spinlock.h"

#include <mutex>


namespace GDL
{
template <typename _type>
class ThreadPoolQueue
{
    mutable SpinLock mSpinLock; //!< Spinlock to protect internal data from data races
    Queue<_type> mQueue; //!< Queue filled with tasks


public:
    ThreadPoolQueue();
    ThreadPoolQueue(const ThreadPoolQueue& other) = delete;
    ThreadPoolQueue(ThreadPoolQueue&& other) = delete;
    ThreadPoolQueue& operator=(const ThreadPoolQueue& other) = delete;
    ThreadPoolQueue& operator=(ThreadPoolQueue&& other) = delete;
    ~ThreadPoolQueue();



    //! @brief Pushes a new value into the queue and informs a waiting thread
    //! @param value New value
    void Push(_type value);


    //! @brief Tries to get the next value of the queue
    //! @param out: reference to a variable that stores fetched value
    //! @return TRUE if the operation was successful, FALSE if not
    bool TryPop(_type& out);


    //! @brief Returns if the queue is empty or not
    //! @return TRUE if the queue is empty, FALSE if not
    bool IsEmpty() const;

    //! @brief Gets the size of the queue
    //! @return Size
    U64 GetSize() const;
};
}
