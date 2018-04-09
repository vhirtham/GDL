#include "gdl/resources/cpu/threadPoolQueue.h"
#include "gdl/resources/cpu/taskBase.h"

#include <memory>

namespace GDL
{


template <typename T>
ThreadPoolQueue<T>::ThreadPoolQueue()
    : mMutex{}
{
}

template <typename T>
ThreadPoolQueue<T>::~ThreadPoolQueue()
{
}



template <typename T>
void ThreadPoolQueue<T>::Push(T value)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mQueue.push(std::move(value));
}

template <typename T>
bool ThreadPoolQueue<T>::tryPop(T& out)
{
    std::lock_guard<std::mutex> lock(mMutex);
    if (mQueue.empty())
    {
        return false;
    }
    out = std::move(mQueue.front());
    mQueue.pop();
    return true;
}



template <typename T>
bool ThreadPoolQueue<T>::IsEmpty() const
{
    std::lock_guard<std::mutex> lockGuard(mMutex);
    return mQueue.empty();
}


template class ThreadPoolQueue<std::unique_ptr<TaskBase>>;
}