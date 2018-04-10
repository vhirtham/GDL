#pragma once

#include "gdl/GDLTypedefs.h"
#include "gdl/resources/cpu/thread.h"
#include "gdl/resources/cpu/task.h"
#include "gdl/resources/cpu/taskBase.h"
#include "gdl/resources/cpu/threadPoolQueue.h"

#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>

#include <iostream>

namespace GDL
{

class ThreadPoolNEW
{
    friend class ThreadNEW;



    std::atomic_bool mClose = false;
    mutable std::mutex mMutex;
    std::condition_variable mConditionThreads;

    std::deque<ThreadNEW> mThreads;
    ThreadPoolQueue<std::unique_ptr<TaskBase>> mQueue;

public:
    ThreadPoolNEW() = delete;
    ThreadPoolNEW(const ThreadPoolNEW&) = delete;
    ThreadPoolNEW(ThreadPoolNEW&&) = delete;
    ThreadPoolNEW& operator=(const ThreadPoolNEW&) = delete;
    ThreadPoolNEW& operator=(ThreadPoolNEW&&) = delete;
    ~ThreadPoolNEW();


    explicit ThreadPoolNEW(const U32 numThreads);

    void Deinitialize();

    void Initialize(const U32 numThreads);

    void TryExecuteTask();

    void TryExecuteTaskWait();

    bool HasTasks() const;

    void WaitForTask();

    template <typename F, typename... Args>
    void Submit(F&& func, Args&&... args);
};



template <typename F, typename... Args>
void ThreadPoolNEW::Submit(F&& func, Args&&... args)
{
    // static assertion
    using ResultType = std::result_of_t<decltype(std::bind(std::forward<F>(func), std::forward<Args>(args)...))()>;
    using TaskType = Task<decltype(std::bind(std::forward<F>(func), std::forward<Args>(args)...))>;

    static_assert(std::is_same<void, ResultType>::value, "Used submit() with non void function!");
    mQueue.Push(std::make_unique<TaskType>(std::bind(std::forward<F>(func), std::forward<Args>(args)...)));
    mConditionThreads.notify_one();
}
}
