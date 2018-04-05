#include "gdl/resources/cpu/threadPoolQueue.h"
#include "gdl/resources/cpu/taskBase.h"

#include <memory>

namespace GDL
{

template class ThreadPoolQueue<std::unique_ptr<TaskBase>>;

template <typename T>
ThreadPoolQueue<T>::ThreadPoolQueue()
    : mValid{true}
    , mMutex{}
{
}

template <typename T>
ThreadPoolQueue<T>::~ThreadPoolQueue()
{
    Deinitialize();
}

template <typename T>
void ThreadPoolQueue<T>::Deinitialize()
{
    std::lock_guard<std::mutex> lock(mMutex);
    mValid = false;
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
    if (mQueue.empty() || !mValid)
    {
        return false;
    }
    out = std::move(mQueue.front());
    mQueue.pop();
    return true;
}
}
