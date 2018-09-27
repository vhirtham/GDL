#include "gdl/resources/cpu/threadPoolQueue.h"

#include "gdl/base/uniquePtr.h"
#include "gdl/resources/cpu/taskBase.h"


namespace GDL
{


template <typename _type>
ThreadPoolQueue<_type>::ThreadPoolQueue()
    : mSpinLock{}
{
}

template <typename _type>
ThreadPoolQueue<_type>::~ThreadPoolQueue()
{
}



template <typename _type>
void ThreadPoolQueue<_type>::Push(_type value)
{
    std::lock_guard<SpinLock> lock(mSpinLock);
    mQueue.push(std::move(value));
}

template <typename _type>
bool ThreadPoolQueue<_type>::TryPop(_type& out)
{
    std::lock_guard<SpinLock> lock(mSpinLock);
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
    std::lock_guard<SpinLock> lockGuard(mSpinLock);
    return mQueue.empty();
}

template <typename _type>
U32 ThreadPoolQueue<_type>::GetSize() const
{
    std::lock_guard<SpinLock> lockGuard(mSpinLock);
    return mQueue.size();
}


template class ThreadPoolQueue<UniquePtr<TaskBase>>;
}
