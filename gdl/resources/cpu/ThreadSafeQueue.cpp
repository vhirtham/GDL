#include "ThreadSafeQueue.h"


#include "gdl/resources/cpu/ThreadTaskBase.h"
#include <memory>

template <typename T>
GDL::ThreadSafeQueue<T>::ThreadSafeQueue()
    : mValid(true)
    , mCondition()
    , mMutex()
    , mQueue()
{
}

template <typename T>
GDL::ThreadSafeQueue<T>::~ThreadSafeQueue()
{
    invalidate();
}

template <typename T>
bool GDL::ThreadSafeQueue<T>::tryPop(T& out)
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



template <typename T>
bool GDL::ThreadSafeQueue<T>::tryWaitPop(T& out)
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

template <typename T>
void GDL::ThreadSafeQueue<T>::push(T value)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mQueue.push(std::move(value));
    mCondition.notify_one();
}


template <typename T>
bool GDL::ThreadSafeQueue<T>::empty() const
{
    std::lock_guard<std::mutex> lock(mMutex);
    return mQueue.empty();
}


template <typename T>
void GDL::ThreadSafeQueue<T>::clear()
{
    std::lock_guard<std::mutex> lock(mMutex);
    while (!mQueue.empty())
        mQueue.pop();
    mCondition.notify_all();
}

template <typename T>
void GDL::ThreadSafeQueue<T>::invalidate()
{
    std::lock_guard<std::mutex> lock(mMutex);
    mValid = false;
    mCondition.notify_all();
}

template <typename T>
bool GDL::ThreadSafeQueue<T>::isValid()
{
    std::lock_guard<std::mutex> lock(mMutex);
    return mValid;
}


namespace GDL
{
template class ThreadSafeQueue<std::unique_ptr<ThreadTaskBase>>;
}
