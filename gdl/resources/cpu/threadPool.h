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

    // put in own Waiter class
    std::condition_variable mConditionParentThreadWait;
    mutable std::mutex mMutexParentThreadWait;
    bool mParentThreadWait = true;



    mutable std::mutex mMutex;
    std::condition_variable mConditionThreads;

    std::deque<ThreadNEW> mThreads;
    std::atomic_bool mClose = false;
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

    void ParentThreadWait();
    void ParentThreadContinue();

    void GetTask();

    bool HasTasks();

    template <typename F, typename... Args>
    void submit(F&& func, Args&&... args);

    template <typename F>
    void ThisThreadWaitFor(F&& function);

    template <typename F>
    void CheckEnque(F& function, std::condition_variable& condition);
};

template <typename F>
void ThreadPoolNEW::ThisThreadWaitFor(F&& function)
{
    std::condition_variable condtition;
    std::mutex mutex;
    CheckEnque(function, condtition);
}

template <typename F>
void ThreadPoolNEW::CheckEnque(F& function, std::condition_variable& condition)
{
    function();
}


// http://en.cppreference.com/w/cpp/language/template_specialization
template <typename T> // primary template
struct is_void : std::false_type
{
};
template <> // explicit specialization for T = void
struct is_void<void> : std::true_type
{
};


template <typename F, typename... Args>
void ThreadPoolNEW::submit(F&& func, Args&&... args)
{
    // static assertion
    using ResultType = std::result_of_t<decltype(std::bind(std::forward<F>(func), std::forward<Args>(args)...))()>;
    using TaskType = Task<decltype(std::bind(std::forward<F>(func), std::forward<Args>(args)...))>;

    static_assert(is_void<ResultType>::value, "Used submit() with non void function!");
    mQueue.Push(std::make_unique<TaskType>(std::bind(std::forward<F>(func), std::forward<Args>(args)...)));
    mConditionThreads.notify_one();
}
}
