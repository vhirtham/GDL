#include "gdl/resources/cpu/threadPoolQueue.h"

#include "gdl/base/uniquePtr.h"
#include "gdl/resources/cpu/taskBase.h"


namespace GDL
{


template <typename _type>
ThreadPoolQueue<_type>::ThreadPoolQueue()
    : mMutex{}
{
}

template <typename _type>
ThreadPoolQueue<_type>::~ThreadPoolQueue()
{
}



template <typename _type>
void ThreadPoolQueue<_type>::Push(_type value)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mQueue.push(std::move(value));
}

template <typename _type>
bool ThreadPoolQueue<_type>::TryPop(_type& out)
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



template <typename _type>
bool ThreadPoolQueue<_type>::IsEmpty() const
{
    std::lock_guard<std::mutex> lockGuard(mMutex);
    return mQueue.empty();
}

template <typename _type>
U32 ThreadPoolQueue<_type>::GetSize() const
{
    std::lock_guard<std::mutex> lockGuard(mMutex);
    return mQueue.size();
}


template class ThreadPoolQueue<UniquePtr<TaskBase>>;
}
